/*
 **************************************************************** 
*                         GET_HORA                
 * **************************************************************
*/ 
void GET_HORA(String payload){
  String Response;
  HTTPClient http;
  //Serial.print("[HTTP] begin...\n");
  //http.begin("http://192.168.1.226:3001/apiDana/hora"); //HTTP
  http.begin(String(server_api) + "hora"); //HTTP
  
  Serial.print("[HTTP] GET...");
  Serial.println(String(server_api) + "hora");
  
  int httpCode = http.GET();

  if(httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      if(httpCode == HTTP_CODE_OK) {
          Response = http.getString();
          Serial.println("Servidor Res:" + String(Response));
      }
  } else {
    
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      Response = "FALSE";
  }
  http.end();
}
/*
 **************************************************************** 
*                         POST_JSON_REGISTRO                
 * **************************************************************
*/ 
String POST_JSON(String payload ){
  String Response;
  HTTPClient http;
  http.begin(String(server_api) + "insertData");
  Serial.println("API: " + String(server_api)+ "insertData");
  
  http.addHeader("Content-Type", "application/json");   

    int httpResponseCode = http.POST(payload);

  if(httpResponseCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpResponseCode);
      if(httpResponseCode == HTTP_CODE_OK) {
          Response = http.getString();
          
      }
  } else {
      Response = "[HTTP] POST... failed, error:" + http.errorToString(httpResponseCode);
//      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());    
  }
  http.end();
  return Response;
}

/*
 **************************************************************** 
*                         POST_HEATBEAT                
 * **************************************************************
*/ 
String POST_HEATBEAT(String payload ){
  String Response;
  HTTPClient http;
  http.begin(String(server_api) + "status");
  Serial.println("API: " + String(server_api) + "status");
  http.addHeader("Content-Type", "application/json");   
    
    int httpResponseCode = http.POST(payload);

  if(httpResponseCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpResponseCode);
      if(httpResponseCode == HTTP_CODE_OK) {
          Response = http.getString();          
      }
  } else {
      Response = "[HTTP] POST... failed, error:" + http.errorToString(httpResponseCode);
//      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());    
  }
  http.end();
  return Response;
}
