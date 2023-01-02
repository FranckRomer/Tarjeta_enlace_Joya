/*
 ************************************************************************************ 
 *    DANA LITE 2.0
 ************************************************************************************
*/
#include <creacionEnviosMQTTyCAN.h>
#include <WiFi.h>
#include <ESP32CAN.h>
#include <CAN_config.h>
#include <iostream>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "driver/can.h"

using namespace std;

/*
 ************************************************************
 * VARIABLES QUE CAMBIAN 
 ************************************************************
*/

// ACCESA
const char* ssid = "RED ACCESA";
const char* password = "037E32E7";
const char* proyect = "accesa"; 
const char* server_api = "http://192.168.1.109:3000/api/dana/servidorEsp/";

// JOYA
//const char* ssid = "DOMOTICA";
//const char* password = "de la llave 2021 domotica";
//const char* proyect = "dana"; 
//const char* server_api = "http://192.168.1.226:3000/api/dana/servidorEsp/";


int tiempoEnvio = 5000; // milisegundos
int envioStatus = tiempoEnvio;
int resetAutomatico = 12000;
int resetCont = 0;
/*
 ************************************************************
 * VARIABLES QUE CAMBIAN 
 ************************************************************
*/
const int led = 13;

/*
 *  OBJETOS
*/
EnvioCAN         CAN;
CAN_frame_t      rx_frame;
CAN_device_t     CAN_cfg;
String json;
/*
 * FUNCIONES PROTOTIPO
*/
void envioCAN(String cadenaTCP);

EnvioMqtt   cadenaMqtt;


//////////////////
WebServer server(80);

/*
 * ********************************************************************
 *                                 Setup
 * ********************************************************************
*/
void setup() {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  setup_wifi(); 
  /*
   * Conexion CAN
  */
  /* Seleccionar pines TX, RX y baudios */
  CAN_cfg.speed=CAN_SPEED_125KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_5;
  CAN_cfg.rx_pin_id = GPIO_NUM_35;
  
  /* Crear una cola de 10 mensajes en el buffer del CAN */
  CAN_cfg.rx_queue = xQueueCreate(300,sizeof(CAN_frame_t));
  
  //INICIALIZAR MODULO CAN
  ESP32Can.CANInit();

  // SEGUNDO NUCLEO
  xTaskCreatePinnedToCore(
            toggleLED,        //Funcion a llamar
            "Toggle LED",     //Nombre de la tarea
            4096,             //Bytes de memoria que puede usar la tarea
            NULL,             //Apuntador de memoria (no se usa)
            1,                //Prioridad de la tarea (de 0 a 24, siendo 24 la mayor prioridad)
            NULL,             //Apuntador de tarea
            1);         //Nucleo o procesador que vamos a usar  
}



/*
 * ********************************************************************
 * Programa principal
 * ********************************************************************
*/

void loop() {
  String data;
  String json_ouput;
  char    dataSnd[15];
  String body, Response;

  delay(1);
  envioStatus ++;
  resetCont ++;
//  if(envioStatus >= tiempoEnvio){
//    envioStatus = 0;
//    // Envio de heartBeat
//    Serial.println("**************** SerializacionHeatBeat ****************");
//    body = SerializacionHeatBeat();
//    Serial.println(body);
//    Response = POST_HEATBEAT(body);
//    Serial.println("Servidor Res: \n" + String(Response));
//  }
  if(resetCont >= resetAutomatico){ muerteEsp(); }
  /*
   * MENSAJES RECIBIDOS POR EL BUS CAN
   */
  if(xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE){
      
      if(rx_frame.FIR.B.FF==CAN_frame_std)
        printf("New standard frame");
      else
        printf("........................... NUEVO MENSAJE CAN ...........................");

      if(rx_frame.FIR.B.RTR==CAN_RTR)
        printf(" RTR from   0x%.3x, DLC %d\r\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
      else{

        if(rx_frame.MsgID == 255 || 1){


          cadenaMqtt.envioActivacion(&rx_frame, dataSnd);
          
          switch(char(rx_frame.data.u8[0])){
            case '0':  
                      
                      Serial.println("ACTIVACIÓN/DESACTIVACIÓN CONFIRMADA");
                      Serial.println(dataSnd);
                      // Envio de datos CAN
                      body = SerializeObject(dataSnd);
                      Serial.println(body);
                      Response = POST_JSON(body);
                      Serial.println("Servidor Res: \n" + String(Response));
                      resetCont = 0;
                      //
                      break;
            
            default:  break;       
          }
        memset(dataSnd, 'x', 15);
        printf("\n");
        }
      }
  }
}

/*
 * ********************************************************************
 *          SEGUNDO NUCLEO
 * ********************************************************************
*/
void toggleLED(void *parameter){
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", handleRoot);

  server.on("/can", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  while(1){
    //
    server.handleClient();
    vTaskDelay(2);
//    delay(2);//allow the cpu to switch to other tasks
  }
}
