/*
 * ********************************************************************
 *                              Setup WIFI
 * ********************************************************************
*/

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/*
 * ********************************************************************
 *                              MUERTE AL ESP
 * ********************************************************************
*/

void muerteEsp(){
      resetCont = 0; 
      Serial.println("!!!!!!!!!!!!!!!!!!! SE RESETEA !!!!!!!!!!!!!!!!!!!"); 
      //Stop the CAN driver
      if (can_stop() == ESP_OK) {
          printf("Driver stopped\n");
      } else {
          printf("Failed to stop driver\n");
          return;
      }
      digitalWrite(led, 1);
      delay(700);
      //Uninstall the CAN driver
      if (can_driver_uninstall() == ESP_OK) {
          printf("Driver uninstalled\n");
      } else {
          printf("Failed to uninstall driver\n");
          return;
      }

      delay(1000);
//    ESP.getFlashChipMode();
      ESP.restart();  
//    double muerte = 69/0;
//    Serial.println(muerte);

}
