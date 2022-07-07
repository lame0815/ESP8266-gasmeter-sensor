
void clearEEPROM() {
     Serial.println("clearing eeprom");
        for (int i = 0; i < 278; ++i) {
          EEPROM.write(i, ' ');
          Serial.println(".");
        }
     EEPROM.commit();
}

void writeWifiMode(String wifimode) {
  EEPROM.write(96, wifimode[0]); 
  EEPROM.commit();
}

/*
 * write Wifi Configuration to EEPROM
 */
void writeWifiConfiguration(String qsid, String qpass, String wifimode) {

        Serial.println("ssid");
        Serial.println(qsid);
        Serial.println("password");
        Serial.println(qpass);
        Serial.println("wifimode");
        Serial.println(wifimode);
        

        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }
        Serial.println("writing eeprom wifimode:");
        EEPROM.write(96, wifimode[0]);        
       
        EEPROM.commit();
        
}

/*
 * write MQQT Configuration to EEPROM
 */
void writeMqttConfiguration(String mqttserver, String mqttport, String mqtttuname, String mqttpass) {
   Serial.println("clearing eeprom");
        for (int i = 97; i < 273; ++i) {
          EEPROM.write(i, ' ');
        }        
        Serial.println("mqqtserver");
        Serial.println(mqttserver);
        Serial.println("mqqtport");
        Serial.println(mqttport);
        Serial.println("mqqt user name");
        Serial.println(mqtttuname);
        Serial.println("mqqt password");
        Serial.println(mqttpass);
        
        Serial.println("writing mqttserver:");
        for (int i = 0; i < mqttserver.length(); ++i)
        {
          EEPROM.write(97 + i, mqttserver[i]);
          Serial.print("Wrote: ");
          Serial.println(mqttserver[i]);
        }
         Serial.println("writing mqttport:");
        for (int i = 0; i < mqttport.length(); ++i)
        {
          EEPROM.write(197 + i, mqttport[i]);
          Serial.print("Wrote: ");
          Serial.println(mqttport[i]);
        }
         Serial.println("writing mqtttuname:");
        for (int i = 0; i < mqtttuname.length(); ++i)
        {
          EEPROM.write(203 + i, mqtttuname[i]);
          Serial.print("Wrote: ");
          Serial.println(mqtttuname[i]);
        }
            Serial.println("writing mqttpass:");
        for (int i = 0; i < mqttpass.length(); ++i)
        {
          EEPROM.write(223 + i, mqttpass[i]);
          Serial.print("Wrote: ");
          Serial.println(mqttpass[i]);
        }


        
        EEPROM.commit();
        
}

/*
 * write Debounce INtervalto EEPROM
 */
void writeDebounceInterval(String debounce) {
   Serial.println("clearing eeprom");
        for (int i = 273; i < 278; ++i) {
          EEPROM.write(i, ' ');
        }        
        Serial.println("debounce");
       
        
        Serial.println("writing debounce:");
        for (int i = 0; i < debounce.length(); ++i)
        {
          EEPROM.write(273 + i, debounce[i]);
          Serial.print("Wrote: ");
          Serial.println(debounce[i]);
        }                 
        EEPROM.commit();
        
}


void writeDeviceSettings(String timezone) {
   Serial.println("clearing eeprom");
        for (int i = 278; i < 281; ++i) {
          EEPROM.write(i, ' ');
        }        
        Serial.println("timezone");
       
        
        Serial.println("writing timezone:");
        for (int i = 0; i < timezone.length(); ++i)
        {
          EEPROM.write(278 + i, timezone[i]);
          Serial.print("Wrote: ");
          Serial.println(timezone[i]);
        }                 
        EEPROM.commit();
        
}

/*
 * read Debounce Interval from EEPROM
 */
String readDebounceInterval() {
  Serial.println("Reading readDebounceInterval");

  String epass = "";
  for (int i = 273; i < 278; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("readDebounceInterval: ");
  Serial.println(epass);
   epass.trim();
  return epass;
}

/*
 * read Debounce Interval from EEPROM
 */
String readDeviceSettings() {
  Serial.println("Reading readDebounceInterval");

  String epass = "";
  for (int i = 278; i < 281; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("readDebounceInterval: ");
  Serial.println(epass);
   epass.trim();
  return epass;
}

/*
 * read SSID from EEPROM
 */
String readWiFiSSID() {
  // Read eeprom for ssid and password
  Serial.println("Reading EEPROM ssid");

  String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  esid.trim();
  return esid;
}

/*
 * read Wifi Mode from EEPROM
 * returns "C" if "C" is stored on EEPROM
 * and "A" else
 */
String readWiFiMode() {
  // Read eeprom for ssid and password
  Serial.println("Reading EEPROM ssid");

  String wifimode;

  wifimode += char(EEPROM.read(96));
 
  Serial.println();
  Serial.print("wifimode:");
  Serial.print(wifimode);
  if (wifimode.equals("C")) {
    return "C";
  } else if (wifimode.equals("A")) {
    return "A";
  } else {
    return "N";
  }
  
}


/*
 * read Wifi Pass from EEPROM
 */
String readWiFiPass() {
  Serial.println("Reading EEPROM pass");

  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);
  epass.trim();
  return epass;
}

String readMqttServer() {
  //Serial.println("Reading Mqqt pass");

  String epass = "";
  for (int i = 97; i < 197; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  //Serial.print("MqqtServer: ");
   epass.trim();
  return epass;
}

/*
 * read Mqtt Port from EEPROM
 */
String readMqttPort() {

  String epass = "";
  for (int i = 197; i < 203; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("Mqtt Port: ");
   epass.trim();
  return epass;
}

/*
 * read Mqtt Username from EEPROM
 */
String readMqttUsername() {

  String epass = "";
  for (int i = 203; i < 223; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  epass.trim();
  return epass;
}

/*
 * read Mqtt Password from EEPROM
 */
String readMqttPass() {
  

  String epass = "";
  for (int i = 223; i < 273; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  
   epass.trim();
   if (epass.length() > 0) {
    Serial.print("MqttPass: ");
    Serial.println(epass);
   }
  return epass;
}
