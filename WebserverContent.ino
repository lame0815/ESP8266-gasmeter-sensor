
/**
 * root page on /
 */
void handleRoot() {


  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
   String formattedTime = timeClient.getFormattedTime();
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


  server.sendContent(text);
  
  text = "<div class=\"tablecell\">";

  
  if (currentWifiMode == WIFIMODE_AP) {
    text = text + "Access Point Mode";
  } else {
    text = text + "Client Mode";
  }
  text = text + "</div>";
  text = text + "<div class=\"tablecell\">";
  text = text + WiFi.localIP().toString();
  text = text + "</div>";
    text = text + "<div class=\"tablecell\">";
  text = text + formattedTime;
  text = text + "</div>";
  text = text + "</div>";
  
  text = text + "<div class=\"column\"><h2>Counter:</h2></div><div class=\"counter\" id=\"counter\" ></div>";
  text = text + "</div></div>";
  text = text + "<div class=\"column\"><h2>Counts per Minute:</h2></div><div class=\"cpm\" id=\"cpm\" ></div>";
  text = text + "</div></div>";
  
  text = text + "<div class=\"column\"><h2>Today and Previous Day:</h2></div>";
  
  server.sendContent(text);
  
  text = renderHistory();
  
  server.sendContent(text);
 
  text = "<div class=\"menu menu-horizontal\"><ul class=\"menu-list\">";

  text = text + "<li class=\"menu-item\"><a href=\"/devicesettings\" type=\"button\">System Setup</a></li>";
  text = text + "</ul></div>";
  text = text + generateHTMLFooter();
  server.sendContent(text);
  server.sendContent("");
  //server.send(200, "text/html", text);
}


/**
 * render history as html
 */
String renderHistory() {
  String text = "<table border=\"1\">";
  text = text + "<tr><th>Hour</th><th>Today (";
  if (currday > -1) {
    text = text + currday;
  } else {
    text = text + "N.N.";
  }
  text = text + ")</th><th>Previous Day (";
  if (yesterday > -1) {
    text = text + yesterday;
  } else {
    text = text + "N.N.";
  }
  text = text + ") </th></tr>";
  for (int x= 0; x<24; x++) {
    text = text + "<tr><td>";
    text = text + x;
    text = text + "</td><td>";
    text = text + todayusage[x];
    text = text + "</td><td>";
    text = text + yesterdayusage[x];
    text = text +"</td>";
  }
  text = text + "</table>";
  return text;
}


/**
 * Debounce form handler
 */
void handleSetDeviceSettings() {

  String offset = readDeviceSettings();
  Serial.println(offset);  
  
  int iOffset = offset.toInt();  
  Serial.println("OFFSET");
  Serial.println(iOffset);

  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  
  String text = generateHTMLHead("SETUP Gasmeter Config");  

  server.sendContent(text);
  
  text = "<div class=\"row\">";

text = text + "<ul><li style=\"float: none\"><a href=\"/\">back</a></li></ul>";

text = text + "<div class=\"column\"><h2>Network Setup</h2></div>";
     text = text + "<div class=\"column\">";
  text = text + "<form action=\"/wifistore\" class=\"cust-form cust-form-stacked\" method=\"POST\"><fieldset>";

  text = text + "<legend>Wifi Setup</legend>";
  text = text + "<label for=\"ssid\">* SSID</label><input type=\"text\" id=\"ssid\" name=\"ssid\" maxlength=\"32\" value=\""+readWiFiSSID()+"\" placeholder=\"Wifis ssid\" required ></input>";
  text = text + "<label for=\"pass\">* Wifi-Pass</label><input type=\"text\" id=\"pass\" name=\"pass\" maxlength=\"64\" value=\""+readWiFiPass()+"\" placeholder=\"Wifis pass\" required ></input>";

  server.sendContent(text);

  String wifimode = readWiFiMode();
  text =  "<label for=\"clientmode\" class=\"radio\"><input type=\"radio\" id=\"clientmode\" name=\"wifimode\" value=\"C\" ";
  if (WIFIMODE_CLIENT.equals(wifimode)) text = text + " checked ";
  text = text + "/>* Client Mode</label>";
  text = text + "<label for=\"apmode\" class=\"radio\"><input type=\"radio\" id=\"apmode\" name=\"wifimode\" value=\"A\" ";
  if (WIFIMODE_AP.equals(wifimode)) text = text + " checked ";
  text = text + "/>Access Point Mode</label>";

   text = text + "<ul><li class=\"primary\"><button type=\"submit\" class=\"cust-button cust-button-primary\">Save</button></li></ul>";
 
  text = text + "</fieldset></form>";
  text = text + "</div>";

     text = text + "<div class=\"column\">";
  text = text + "<form action=\"/mqttstore\" class=\"form-stacked\" method=\"POST\"><fieldset>";

  text = text + "<legend>Mqqt Server</legend>";
  text = text + "<label for=\"mqttserver\">* MQQT Server</label><input type=\"text\" id=\"mqttserver\" maxlength=\"100\" name=\"mqttserver\" value=\""+readMqttServer()+"\" placeholder=\"aaa.bbb.ccc.ddd\" required ></input>";
  text = text + "<label for=\"mqttport\">* MQQT Port</label><input id=\"mqttport\" name=\"mqttport\" type=\"number\" min=\"1\" step=\"1\" max=\"65536\" value=\""+readMqttPort()+"\" placeholder=\"1883\" required ></input>";
  text = text + "<label for=\"mqttusername\">MQQT Username</label><input type=\"text\" id=\"mqttusername\" maxlength=\"20\" name=\"mqttusername\" value=\""+readMqttUsername()+"\" placeholder=\"youruser\" ></input>";
  text = text + "<label for=\"mqttpassword\">MQQT Password</label><input type=\"text\" id=\"mqttpassword\" maxlength=\"50\" name=\"mqttpassword\" value=\""+readMqttPass()+"\" placeholder=\"yourpass\" ></input>";
  text = text + "<ul><li class=\"primary\"><button type=\"submit\" class=\"custbutton\">Save</button></li></ul>";
 
  text = text + "</fieldset></form>";
  text = text + "</div>";

  server.sendContent(text);
  text = "<hr><div class=\"column\"><h2>Device & Sensor Settings</h2></div>";
     text = text + "<div class=\"column\">";
  text = text + "<form action=\"/devicesettingsstore\" required class=\"cust-form cust-form-stacked\" method=\"POST\"><fieldset>";

  text = text + "<legend>Timezone</legend>";
  text = text + "<label for=\"mqttserver\">* Timezone GMT </label><select id=\"timezone\" name=\"timezone\">";
  for (int y=-12; y<13; y++) {
    text = text + "<option value=\"";
    text = text + y;
    text = text +"\"";
    if (iOffset == y) {
      text = text + " selected";
    }
    text = text +" >";
    text = text + y;
    text = text + "</option>";
  }
  

  text = text + "</select>";
 text = text + "<ul><li class=\"primary\"><button type=\"submit\" class=\"cust-button cust-button-primary\">Save</button></li></ul>";
 
  text = text + "</fieldset></form>";
  server.sendContent(text);

     text = "<div class=\"column\">";
  text = text + "<form action=\"/debounceintervalstore\" required class=\"cust-form cust-form-stacked\" method=\"POST\"><fieldset>";

  text = text + "<legend>Debounce Interval</legend>";
  text = text + "<label for=\"mqttserver\">* Debounce Interval (millis)</label><input id=\"debounceinterval\" name=\"debounceinterval\" type=\"number\" min=\"1\" max=\"9999\" step=\"1\" value=\""+debounceinterval+"\" required ></input>";
 text = text + "<ul><li class=\"primary\"><button type=\"submit\" class=\"cust-button cust-button-primary\">Save</button></li></ul>";
 
  text = text + "</fieldset></form>";
  text = text + "</div>";
  server.sendContent(text);

     text = "<div class=\"column\">";
  text = text + "<form action=\"/counterstore\" type=\"number\" step=\"1\" required class=\"cust-form cust-form-stacked\" method=\"POST\"><fieldset>";

  text = text + "<legend>Counter Value</legend>";
  text = text + "<label for=\"mqttserver\">* Counter Value</label><input id=\"counter\" name=\"counter\" type=\"number\" min=\"1\" max=\"2147483647\" step=\"1\" value=\""+zaehlerstand+"\" required ></input>";
    text = text + "<ul><li class=\"primary\"><button type=\"submit\" class=\"cust-button cust-button-primary\">Save</button></li></ul>";
 
  text = text + "</fieldset></form>";
  text = text + "</div>";
  text = text + "</div>";
  text = text + "<div class=\"row\">";
  text = text + "The counter value is not memory persistent. In case of a reset or power loss, counter value will be reset to 0<br/>";
  text = text + "API-REST-Call: <a href=\"/api/QueryCounter\" target=\"_blank\">/api/QueryCounter</a>";
  text = text + "</div>";
  server.sendContent(text);

  text = "<hr><div class=\"menu menu-horizontal\"><ul class=\"menu-list\">";
   text = text + "<li class=\"menu-item-vertical\"><a href=\"/update\" type=\"button\">Firmware Update</a></li>";
  text = text + "<li class=\"menu-item-vertical\" style=\"background-color: red;\"><a href=\"/factoryreset\"  type=\"button\" onclick=\"if (confirm('Do you really want to perform a factory reset?')){return true;}else{event.stopPropagation(); event.preventDefault();};\">FACTORY RESET</a></li>";
  text = text + "</ul></div>";
  
  text = text + "</div>";




  
  text = text + "</div>";


  
  text = text + generateHTMLFooter();
  server.sendContent(text);
server.sendContent("");
  
 // server.send(200, "text/html", text);
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


void handleDeviceSettingsStore() {
      String timezone = server.arg("timezone");
      Serial.println("from form: timezone"+timezone);
      int tz = timezone.toInt();
      writeDeviceSettings(timezone);
      timeClient.setTimeOffset(tz*3600);
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

void handleFactoryReset() {
     clearEEPROM();
     server.send(200, "text/html", "Executed factory Reset. Please reconnect to your device.");
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
