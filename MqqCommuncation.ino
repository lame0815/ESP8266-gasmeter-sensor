
/*
 * callback method without function
 */
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/*
 * reconnect mqqt if required 
 */
void reconnectMqtt() {
  Serial.println("disconnect mqtt...");
  if (mqttclient.connected()) {
    mqttclient.disconnect();    
  }
  connectMqtt();
}

/**
 * connect mqqt
 */
void connectMqtt() {
  //Serial.println("connect mqtt...");
  char cServer[32] ;

  /* read mqqt data from EEPROM */
  String server = readMqttServer();
  if (server.length() > 5) { // connect mqtt only if servername is given
      server.toCharArray(cServer, 32);
  
      String port = readMqttPort();
      String uname = readMqttUsername();
      String pass = readMqttPass();
  
      int iPort = port.toInt();
      if (server.length() > 0 && iPort > 0)
      {
          Serial.print("Server ");
          Serial.print(cServer);
          Serial.print(" Port: ");
          Serial.print(iPort);
          Serial.print("\n");
    
          mqttclient.setServer(cServer, iPort);
          bool success = false;  
          char cDeviceId[32];
          char cUname[32];
          char cPass[32];
          deviceId.toCharArray(cDeviceId, 32);
           uname.toCharArray(cUname, 32);
          pass.toCharArray(cPass, 32);
          mqttclient.setKeepAlive(70);
           if (uname.length() > 0) {
     
             success = mqttclient.connect(cDeviceId, cUname, cPass);
          } else {
             success = mqttclient.connect(cDeviceId);
         }    
          Serial.println("mqqt connect successful: "+success);
          mqttclient.setCallback(callback);
      }
  }
  
}

/**
 * submit usage data to mqqt
 */
void mqtttSubmitUsage(int usage) {
  char cstr[32];
      itoa(usage, cstr, 10);
      String property = "/";
      property = property + deviceId;
      property = property + "/perminuteusage";
      char cProperty[32];
      property.toCharArray(cProperty, 32);
      bool publishResult = mqttclient.publish(cProperty,cstr);
      Serial.print("Published value usage successfull: ");
      if (publishResult) {
        Serial.print("YES, value: ");
      } else {
        Serial.print("NO, value: ");
      }
      Serial.print(cstr);
      Serial.print("\n");
}

/**
 * submit counter to mqqt
 */
void mqtttSubmitCounter(int zaehlerstand) {
      char cstr[32];
      itoa(zaehlerstand, cstr, 10);
      String property = "/";
      property = property + deviceId;
      property = property + "/counter";
      char cProperty[32];
      property.toCharArray(cProperty, 32);
      bool publishResult = mqttclient.publish(cProperty,cstr);
      Serial.print("Published value zaehlerstand successfull: ");
      if (publishResult) {
        Serial.print("YES, value: ");
      } else {
        Serial.print("NO, value: ");
      }
      Serial.print(cstr);
      Serial.print("\n");
}
