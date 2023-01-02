/*
 * ************************************************************************************************
 *                                          Deserializacion - JSON
 * ************************************************************************************************
*/
// Deserializacion 
String DeserializeObject(String dato_json) {   
    String envio_json_can = dato_json;
    StaticJsonDocument<300> doc;
    String responseThis = "ERROR";
    DeserializationError error = deserializeJson(doc, envio_json_can);
    if (error) { 
      return responseThis; 
    }
    String can        = doc["can"];
    String pin        = doc["pin"];
    String percentage = doc["percentage"];
    String rgb        = doc["rgb"];
    String resetESP   = doc["reset"];
    String tipo       = doc["tipo"];
    String inst       = doc["inst"];
    String tiempo     = doc["tiempo"];
      if(resetESP == "true"){    
        responseThis = "reset ON";
        return responseThis;
      }
      if (tipo != "door"){
        inst = "FF1";
        tiempo = "00";
      }
      
    
    envio_json_can = inst+ can + pin + percentage + tiempo + rgb;

    Serial.println("------------------json desarmado------------------");
    Serial.println("INST: " + inst);
    Serial.println("CAN: " + can);
    Serial.println("PIN: " + pin);
    Serial.println("PORCENTAJE: " + percentage);
    Serial.println("TIEMPO: " + tiempo);
    Serial.println("RGB: " + rgb);
    Serial.println(envio_json_can);
    Serial.println("--------------------------------------------------\n");

    if((can == "null" || pin == "null" || percentage == "null" || rgb == "null")){
      responseThis = "el mensaje debe contar con can,pin,percentage y rbg";
      return responseThis;
    }
    
    responseThis = envio_json_can;
//    delay(100);
    
    Serial.println(envio_json_can);
    CAN.envioActivacion(envio_json_can, &rx_frame);
    Serial.println(envio_json_can);
    envioCAN(envio_json_can);
    return responseThis;
}



/*
 * ********************************************************************
 *                                   Envio CAN
 * ********************************************************************
*/

void envioCAN(String cadenaTCP){
  char destinatario[3];
  byte destinatarioByte;
  
  for(byte i=0; i<2; i++) destinatario[i] = cadenaTCP[3+i];
  destinatarioByte = CAN.x2i(destinatario);
  rx_frame.FIR.B.FF = CAN_frame_ext;
  rx_frame.MsgID = destinatarioByte;
  rx_frame.FIR.B.DLC = 8;  
  
  //ENVIO CAN
  ESP32Can.CANWriteFrame(&rx_frame);
//  delay(200);
  vTaskDelay(100);
  ESP32Can.CANStop(); 
//  delay(200);
  vTaskDelay(100);
  ESP32Can.CANInit();  
//  delay(200);
  vTaskDelay(100);
}
