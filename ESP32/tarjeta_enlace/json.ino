/*
 * ************************************************************************************************
 *                                          Serializacion - JSON
 * ************************************************************************************************
*/
//Serializacion
String SerializeObject(String json) {
    String dataSnd_string;
    char buffer[1024];
    dataSnd_string = String(json);
    StaticJsonDocument<1024> doc;
    doc["can"]        = (dataSnd_string.substring(3,5));
    doc["pin"]        = (dataSnd_string.substring(5,6));
    doc["percentage"] = (dataSnd_string.substring(6,9));
    doc["rgb"]        = (dataSnd_string.substring(11,14));
    doc["proyect"]    =  proyect;
    
    size_t n = serializeJson(doc, buffer);
    return buffer;
}


/*
 * ************************************************************************************************
 *                                          SerializacionHeatBeat - JSON
 * ************************************************************************************************
*/
String SerializacionHeatBeat(){
    char buffer[1024];
    StaticJsonDocument<1024> doc;
    doc["heatBeat"]        = true;
    doc["ipEsp"]           = WiFi.localIP(); 
    doc["proyect"]         =  proyect;
    
    size_t n = serializeJson(doc, buffer);
    return buffer;
}
