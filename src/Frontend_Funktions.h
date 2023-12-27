/* Funktionen
void serverHandleNotFound();
void serverHandleIndex();
void serverHandleSettings();
void serverHandleNetworkScan();

void setupFrontend();
void frontendStart();
void frontendStartInAPMode(IPAddress adress);
void frontendHandleClient();

*/

  /*
  char pMQTTBrokerIP[64] = "";
  char pMQTTBrokerPort[64] = "";
  char pMQTTBrokerPw[64] = "";
  char pModbusJob[64] = "";
  char pModbusServerIp[64] = "";
  char pModbusServerPort[64] = "";
  char pModbusRegister[5][10]; //WTF Danke
  */

//debug log
void debugFrontend( String text ){
#ifdef DEBUG_FRONTEND
    Serial.println("HTTP-SERVER: " + text);
#endif
}


ESP8266WebServer frontendServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void serverHandleNotFound(){
    debugFrontend("Client request unknown adress!");

    String message = "ESP-Einrichtung - Site not found\n\n";
    message += "URI: ";
    message += frontendServer.uri();
    message += "\nMethod: ";
    message += (frontendServer.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += frontendServer.args();
    message += "\n";

    for (uint8_t i = 0; i < frontendServer.args(); i++) {
         message += " " + frontendServer.argName(i) + ": " + frontendServer.arg(i) + "\n";
    }

    frontendServer.send(404, "text/plain", message);
}

/* mqtt
void handleMQTTSettings( int& startIndex ){
  for( int index = startIndex; index <= startIndex+1; index++ ){
    if( frontendServer.argName(index).indexOf("mqtt_broker_address") != -1 ){
      debugFrontend("MQTT-Adress: " + frontendServer.arg(index) );
      strcpy( pMQTTBrokerIP, frontendServer.arg(index).c_str() );
      startIndex++;
    }
    if( frontendServer.argName(index).indexOf("mqtt_broker_port") != -1 ){
      debugFrontend("MQTT-Port: " + frontendServer.arg(index) );
      strcpy( pMQTTBrokerPort, frontendServer.arg(index).c_str() );
      startIndex++;
    }
    if( frontendServer.argName(index).indexOf("mqtt_broker_password") != -1 ){
      debugFrontend("MQTT-Password: " + frontendServer.arg(index) );
      strcpy( pMQTTBrokerPw, frontendServer.arg(index).c_str() );
      startIndex++;
    }
  }
}
*/

void handleWLANSettings( int& startIndex ){
  for( int index = startIndex; index <= startIndex; index++ ){
    if( frontendServer.argName(index).indexOf("SSID") != -1 ){
      debugFrontend("WLAN-SSID: " + frontendServer.arg(index) );
      strcpy( eepromVar1.ssid, frontendServer.arg(index).c_str() );
      startIndex++;
    }
    if( frontendServer.argName(index).indexOf("WLAN_KEY") != -1 ){
      debugFrontend("WLAN-Key: " + frontendServer.arg(index) );
      strcpy( eepromVar1.password, frontendServer.arg(index).c_str() ); 
      startIndex++;
    }
  }
}

/* modbus
void handleModbusSettings( int& startIndex ){
  for( int index = startIndex; index <= startIndex; index++ ){
    if( frontendServer.argName(index).indexOf("modbus_server_ip") != -1 ){
      debugFrontend("Modbus ServerIP: " + frontendServer.arg(index) );
      strcpy( pModbusServerIp, frontendServer.arg(index).c_str() );
      startIndex++;
    }
    if( frontendServer.argName(index).indexOf("modbus_server_port") != -1 ){
      debugFrontend("Modbus ServerPort: " + frontendServer.arg(index) );
      strcpy( pModbusServerPort, frontendServer.arg(index).c_str() );
      startIndex++;
    }
    
    if( frontendServer.argName(index).indexOf("modbus_adress1") != -1 ){
      debugFrontend("Modbus register1: " + frontendServer.arg(index) );
      strcpy( pModbusRegister[0], frontendServer.arg(index).c_str() );
      startIndex++;
    }
    if( frontendServer.argName(index).indexOf("modbus_adress2") != -1 ){
      debugFrontend("Modbus register2: " + frontendServer.arg(index) );
      strcpy( pModbusRegister[1], frontendServer.arg(index).c_str() );
      startIndex++;
    }
    if( frontendServer.argName(index).indexOf("modbus_adress3") != -1 ){
      debugFrontend("Modbus register3: " + frontendServer.arg(index) );
      strcpy( pModbusRegister[2], frontendServer.arg(index).c_str() );
      startIndex++;
    }
    if( frontendServer.argName(index).indexOf("modbus_adress4") != -1 ){
      debugFrontend("Modbus register4: " + frontendServer.arg(index) );
      strcpy( pModbusRegister[3], frontendServer.arg(index).c_str() );
      startIndex++;
    }
    if( frontendServer.argName(index).indexOf("modbus_adress5") != -1 ){
      debugFrontend("Modbus register5: " + frontendServer.arg(index) );
      strcpy( pModbusRegister[4], frontendServer.arg(index).c_str() );
      startIndex++;
    }
  }
}
*/

void serverHandleSettings(){
  debugFrontend( "client requested to push settings" );
  String message;
  bool hasError = false;

  for (int i = 0; i < frontendServer.args(); i++) {
    // check if argument can be saved
    if( frontendServer.arg(i).length() < 64 ){
      hasError = true;
      break;
    }    

    /* ATTENTION: Every handler function increase the index, if they found a parameter!! */
    if( frontendServer.argName(i).indexOf("form_mqtt_settings") != -1 ){
      // push mqtt settings
      //handleMQTTSettings( i );
    }
    else if( frontendServer.argName(i).indexOf("form_wlan_settings") != -1 ){
      // push wlan settings
      handleWLANSettings( i );
    }
    else if( frontendServer.argName(i).indexOf("form_modbus_settings") != -1 ){
      // push modbus settings
      //handleModbusSettings( i );
    }
  }


  if ( hasError )
  {
    frontendServer.sendHeader("Access-Control-Allow-Origin", "*");
    frontendServer.send( 400, "text/plain", "" );
    debugFrontend("refused given parameters!");
  }else{
    //csave(controlerConfig);
    frontendServer.sendHeader("Access-Control-Allow-Origin", "*");
    frontendServer.send(200, "text/plain", "");
  }  
}

void serverHandleNetworkScan(){
    debugFrontend( "client requested networkscan" );

    int n = WiFi.scanNetworks();
    debugFrontend( n + " network(s) found" );

    String message = "[\n";
    for (int i = 0; i < n; i++)
    {
        message += "{\n";
        message += "\"wifiName\": \"" + /*String(i) + "_" +*/ WiFi.SSID(i) + "\",\n";
        message += "\"signalStrength\": " + String( WiFi.RSSI(i) );
        message += "\n}";
        if( i < n -1 )
        message += ",\n";
    }
    message += "\n]";

    //Serial.println(message);
    //server.sendHeader("Connection", "close");
    frontendServer.sendHeader("Access-Control-Allow-Origin", "*");
    frontendServer.send( 200, "application/json", message );
}
    
void serverHandleIndex(){
    debugFrontend("client requested index site.");
    frontendServer.send(200, "text/html", index_html);
}

void frontendHandleClient(){
    frontendServer.handleClient();
}

void setupFrontend(){
  frontendServer.onNotFound( serverHandleNotFound );
  frontendServer.on( "/", serverHandleIndex );
  frontendServer.on( "/settingsinput", serverHandleSettings );
  frontendServer.on( "/networks", serverHandleNetworkScan );
}

void frontendStart(){
    frontendServer.begin();
    debugFrontend("started");

    httpUpdater.setup(&frontendServer);
}

void frontendStartInAPMode(IPAddress adress){
    debugFrontend("booting up access point");
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig( IPAddress(192,168,1,10), adress, IPAddress(255, 255, 255, 0) );
    WiFi.softAP("accesspoint", "");

    IPAddress IP = WiFi.softAPIP();
    debugFrontend("AP IP address: "+ IP.toString() );
}
