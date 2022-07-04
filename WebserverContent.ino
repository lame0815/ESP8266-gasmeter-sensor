
/**
 * root page on /
 */
void handleRoot() {
  String text = generateHTMLHead("Gasmeter Sensor");
  
  text = text + "<div class=\"status\"><div class=\"tablerow\">";
  text = text + "<div class=\"tablecell\">";
  if (mqttclient.connected()) {
    text = text + "<p style=\"color: green;\">MQTT Connected Status: ";   
  } else {
    text = text + "<p style=\"color: red;\">MQTT disconnected Status: ";    
  }
  String errorText = "";
  switch(mqttclient.state()) {
    case(-4):
     errorText = "Connection Timeout";
     break;
     case(-3):
     errorText = "Network connection broken";
     break;
     case(-2):
     errorText = "Network connection failed";
     break;
     case(-1):
     errorText = "Disconnected";
     break;
     case(0):
     errorText = "OK";
     break;
     case(1):
     errorText = "Server dont's support MQTT version";
     break;
     case(2):
     errorText = "Bad Client ID";
     break;
     case(3):
     errorText = "Server was unavailable to connect";
     break;
     case(4):
     errorText = "bad credentials";
     break;
     case(5):
     errorText = "unauthorized";
     break;
  }
    text = text + errorText + "</p>";
  text = text + "</div>";
  text = text + "<div class=\"tablecell\">";

  
  if (currentWifiMode == WIFIMODE_AP) {
    text = text + "Access Point Mode";
  } else {
    text = text + "Client Mode";
  }
  text = text + "</div>";
  text = text + "<div class=\"tablecell\">";
  text = text + WiFi.localIP().toString();
  text = text + "</div>";
  text = text + "</div>";
  
  text = text + "<div class=\"column\"><h2>Counter:</h2></div><div class=\"counter\" id=\"counter\" ></div>";
  text = text + "</div></div>";
  text = text + "<div class=\"column\"><h2>Counts per Minute:</h2></div><div class=\"cpm\" id=\"cpm\" ></div>";
  text = text + "</div></div>";
 
  text = text + "<div class=\"menu menu-horizontal\"><ul class=\"menu-list\">";
  text = text + "<li class=\"menu-item\"><a href=\"/setupwifi\" type=\"button\">Wifi Setup</a></li>";
  text = text + "<li class=\"menu-item\"><a href=\"/setupmqtt\" type=\"button\">MQTT Setup</a></li>";
  text = text + "<li class=\"menu-item\"><a href=\"/setcounter\" type=\"button\">Set Counter value</a></li>";
  text = text + "<li class=\"menu-item\"><a href=\"/setupdebounceinterval\" type=\"button\">Set Debounce Interval</a></li>";
  text = text + "<li class=\"menu-item\"><a href=\"/update\" type=\"button\">Firmware Update</a></li>";
  text = text + "</ul></div>";
  text = text + generateHTMLFooter();
  server.send(200, "text/html", text);
}



/**
 * Wifi setup page
 */
void handleWifiSetup() {
  String text = generateHTMLHead("Wifi SETUP");
  text = text + "<div class=\"row\">";
  text = text + "<div class=\"column\"><h2>Wifi Setup</h2></div>";
     text = text + "<div class=\"column\">";
  text = text + "<form action=\"/wifistore\" class=\"cust-form cust-form-stacked\" method=\"POST\"><fieldset>";

  text = text + "<legend>Wifi Data</legend>";
  text = text + "<label for=\"ssid\">* SSID</label><input type=\"text\" id=\"ssid\" name=\"ssid\" maxlength=\"32\" value=\""+readWiFiSSID()+"\" placeholder=\"Wifis ssid\" required ></input>";
  text = text + "<label for=\"pass\">* Wifi-Pass</label><input type=\"text\" id=\"pass\" name=\"pass\" maxlength=\"64\" value=\""+readWiFiPass()+"\" placeholder=\"Wifis pass\" required ></input>";

  String wifimode = readWiFiMode();
  text = text + "<label for=\"clientmode\" class=\"radio\"><input type=\"radio\" id=\"clientmode\" name=\"wifimode\" value=\"C\" ";
  if (WIFIMODE_CLIENT.equals(wifimode)) text = text + " checked ";
  text = text + "/>* Client Mode</label>";
  text = text + "<label for=\"apmode\" class=\"radio\"><input type=\"radio\" id=\"apmode\" name=\"wifimode\" value=\"A\" ";
  if (WIFIMODE_AP.equals(wifimode)) text = text + " checked ";
  text = text + "/>Access Point Mode</label>";

   text = text + "<br/><ul><li><a href=\"/\">back</a></li><li class=\"primary\"><button type=\"submit\" class=\"cust-button cust-button-primary\">Save</button></li></ul>";
 
  text = text + "</fieldset></form>";
  text = text + "</div>";
  text = text + "</div>";
  text = text + generateHTMLFooter();
  server.send(200, "text/html", text);
}

/**
 * MQQT Setup Page
 */
void handleMqttSetup() {
  String text = generateHTMLHead("SETUP Gasmeter Counter");
  text = text + "<div class=\"row\">";
  text = text + "<div class=\"column\"><h2>MQTT Setup</h2></div>";
     text = text + "<div class=\"column\">";
  text = text + "<form action=\"/mqttstore\" class=\"form-stacked\" method=\"POST\"><fieldset>";

  text = text + "<legend>Mqqt Setup Date</legend>";
  text = text + "<label for=\"mqttserver\">* MQQT Server</label><input type=\"text\" id=\"mqttserver\" maxlength=\"100\" name=\"mqttserver\" value=\""+readMqttServer()+"\" placeholder=\"aaa.bbb.ccc.ddd\" required ></input>";
  text = text + "<label for=\"mqttport\">* MQQT Port</label><input id=\"mqttport\" name=\"mqttport\" type=\"number\" min=\"1\" step=\"1\" max=\"65536\" value=\""+readMqttPort()+"\" placeholder=\"1883\" required ></input>";
  text = text + "<label for=\"mqttusername\">MQQT Username</label><input type=\"text\" id=\"mqttusername\" maxlength=\"20\" name=\"mqttusername\" value=\""+readMqttUsername()+"\" placeholder=\"youruser\" ></input>";
  text = text + "<label for=\"mqttpassword\">MQQT Password</label><input type=\"text\" id=\"mqttpassword\" maxlength=\"50\" name=\"mqttpassword\" value=\""+readMqttPass()+"\" placeholder=\"yourpass\" ></input>";
  text = text + "<ul><li><a href=\"/\">back</a></li><li class=\"primary\"><button type=\"submit\" class=\"custbutton\">Save</button></li></ul>";
 
  text = text + "</fieldset></form>";
  text = text + "</div>";
  text = text + "</div>";
  text = text + generateHTMLFooter();
  server.send(200, "text/html", text);
}


/* 
 *  counter form handler
 */
void handleSetCounter() {
  String text = generateHTMLHead("SETUP Gasmeter Counter");
  text = text + "<div class=\"row\">";
  text = text + "<div class=\"column\"><h2>Set counter</h2></div>";
     text = text + "<div class=\"column\">";
  text = text + "<form action=\"/counterstore\" type=\"number\" step=\"1\" required class=\"cust-form cust-form-stacked\" method=\"POST\"><fieldset>";

  text = text + "<legend>Mqqt Setup Date</legend>";
  text = text + "<label for=\"mqttserver\">* Counter Value</label><input id=\"counter\" name=\"counter\" type=\"number\" min=\"1\" max=\"2147483647\" step=\"1\" value=\""+zaehlerstand+"\" required ></input>";
    text = text + "<ul><li><a href=\"/\">back</a></li><li class=\"primary\"><button type=\"submit\" class=\"cust-button cust-button-primary\">Save</button></li></ul>";
 
  text = text + "</fieldset></form>";
  text = text + "</div>";
  text = text + "</div>";
  text = text + "<div class=\"row\">";
  text = text + "The counter value is not memory persistent. In case of a reset or power loss, counter value will be reset to 0<br/>";
  text = text + "API-REST-Call: <a href=\"/api/QueryCounter\" target=\"_blank\">/api/QueryCounter</a>";
  text = text + "</div>";
  text = text + generateHTMLFooter();
  server.send(200, "text/html", text);
}

/**
 * Debounce form handler
 */
void handleSetDebounceInterval() {
  String text = generateHTMLHead("SETUP Gasmeter Config");
  text = text + "<div class=\"row\">";
  text = text + "<div class=\"column\"><h2>Set Debounce Interval (millis)</h2></div>";
     text = text + "<div class=\"column\">";
  text = text + "<form action=\"/debounceintervalstore\" required class=\"cust-form cust-form-stacked\" method=\"POST\"><fieldset>";

  text = text + "<legend>Mqqt Setup Date</legend>";
  text = text + "<label for=\"mqttserver\">* Debounce Interval (millis)</label><input id=\"debounceinterval\" name=\"debounceinterval\" type=\"number\" min=\"1\" max=\"9999\" step=\"1\" value=\""+debounceinterval+"\" required ></input>";
 text = text + "<ul><li><a href=\"/\">back</a></li><li class=\"primary\"><button type=\"submit\" class=\"cust-button cust-button-primary\">Save</button></li></ul>";
 
  text = text + "</fieldset></form>";
  text = text + "</div>";
  text = text + "</div>";
  text = text + generateHTMLFooter();
  server.send(200, "text/html", text);
}


/*
 * 
 */
void handleJSONQuery() {
  String text = "{ \"count\" : \"";
  text = text + zaehlerstand;
  text = text + "\", ";
  text = text + "\"cpm\" : \"";
  text = text + prevConsumption;
  text = text +"\"";
  text = text + " }";
  server.send(200, "application/json", text);
}


/*
 * counter store handler
 */
void handleCounterStore() {
      String counter = server.arg("counter");
      Serial.println("from form: counter"+counter);
      zaehlerstand = counter.toInt();
      String text = generateHTMLHead("Gasmeter Counter");
      text = text + "<div class=\"row\">";
      text = text + "<div class=\"column\"><h1>OK</div>";
      text = text + "<div class=\"column\"><a href=\"/\">back</a></div>";
      text = text + "</div>";
      text = text + generateHTMLFooter();
      server.send(200, "text/html", text);
}


/**
 * debounce interval form handler 
 */
void handleDebounceIntervalStore() {
  
      String dbv = server.arg("debounceinterval");
      Serial.println("from form: debounceinterval"+dbv);
      debounceinterval = dbv.toInt();
        
      writeDebounceInterval(dbv);
      String text = generateHTMLHead("Gasmeter Counter");
      text = text + "<div class=\"row\">";
      text = text + "<div class=\"column\"><h1>New Debounce Interval: "+debounceinterval+" </div>";
      text = text + "<div class=\"column\"><a href=\"/\">back</a></div>";
      text = text + "</div>";
      text = text + generateHTMLFooter();
     
      server.send(200, "text/html", text);
}

/**
 * Wifi data form handler
 */
void handleWifiStore() {
  

      String wifimode = server.arg("wifimode");
      Serial.println("from form: wifimode"+wifimode);
  
      String ssid = server.arg("ssid");
      Serial.println("from form: ssid"+ssid);
      
      String pass = server.arg("pass");
      Serial.println("from form: pass"+pass);
      
      
      writeWifiConfiguration(ssid, pass, wifimode);
     
      server.send(200, "text/html", "Executed device Reset. Please reconnect to your device.");
      ESP.reset();
}


/**
 * Mqqt form handler 
 */
void handleMqttStore() {
  
      
      
      String mqttserver = server.arg("mqttserver");
      Serial.println("from form: mqttserver"+mqttserver);
      
      String mqttport = server.arg("mqttport");
Serial.println("from form: mqttport"+mqttport);
      String mqttusername = server.arg("mqttusername");
      Serial.println("from form: mqttusername"+mqttusername);
      String mqttpassword = server.arg("mqttpassword");
      Serial.println("from form: mqttpassword"+mqttpassword);
      writeMqttConfiguration(mqttserver, mqttport, mqttusername, mqttpassword);
      reconnectMqtt();
      String text = generateHTMLHead("MQTT Setup");
      text = text + "<div class=\"row\">";
      text = text + "<div class=\"column\"><h1>DONE</div>";
      text = text + "<div class=\"column\"><a href=\"/\">back</a></div>";
      text = text + "</div>";
      text = text + generateHTMLFooter();
     
      server.send(200, "text/html", text);       
}
