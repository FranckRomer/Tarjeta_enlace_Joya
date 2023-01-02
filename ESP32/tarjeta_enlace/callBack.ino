void handleRoot() {
  server.enableCORS(); //This is the magic
  String message, body = "";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    body += server.arg(i);
  }
  Serial.println("************* MENSAJE RECIBIDO *************");
  Serial.println(body);
  // Envio por CAN y Deserializcion
  message = DeserializeObject(body);
  
  server.send(200, "text/plain", "Mensaje Recibido por ESP32: " + message);
  

  ///////////////////////////////////////////

  if(message == "reset ON"){
    muerteEsp();
  }
  
  Serial.println("!!!!!!!! Fin !!!!!!!!");
//  resetCont = 0;
}

void handleNotFound() {
//  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
//  digitalWrite(led, 0);
}
