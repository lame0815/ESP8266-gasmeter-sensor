
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <DoubleResetDetect.h>
#include <ElegantOTA.h>
#include <PubSubClient.h>
#define DRD_TIMEOUT 2.0
#define DRD_ADDRESS 0x00

DoubleResetDetect drd(DRD_TIMEOUT, DRD_ADDRESS);

int zaehlerstand = 0;
int prevConsumption = 0;
int lastsubmit = -1;
int lastvalidinterrupt = 0;
int debounceinterval;
int lastReedState = -1;

int consumptionSubmitInterval = 0;
int consumptioncounter = 0;

static int SENSOR_PIN = D2;
static int DEBOUNCE_INTERVAL = 500;

static String WIFIMODE_AP = "A";
static String WIFIMODE_CLIENT = "C";
static String VERSION = "0.0.1";
static String AUTHOR = "Lars Mense (<a href=\"https://www.lars-mense.de\" target=\"_blank\">www.lars-mense.de</a>)";
String currentWifiMode;
String macToID(const uint8_t*);


void connectWifi();
void handleRoot();
void handleSetup();
void handleJSONQuery();
void handleStore();
void handleCounterStore();
void handleDebounceIntervalStore();
void handleSetCounter();
void handleCustCSS();
void handleSetDebounceInterval();
void handleWifiSetup();
void handleMqttSetup();
void handleWifiStore();
void handleMqttStore();
void callback(char* , byte* , unsigned int);

String readWiFiSSID();
String readWiFiMode();
String readWiFiPass();
String readMqttServer();
String readMqttPort();
String readMqttUsername();
String readMqttPass();
void connectMqtt();
void WriteConfiguration(String, String, String, String, String, String, String);
void writeDebounceInterval(String);

String generateHTMLHead(String);
String generateHTMLFooter(void);
String generateHTMLMenu(void);
String returnFavicon(void);
String readDebounceInterval(void);
String loadJSON(void);
void handlePureCSS();
String deviceId = "gassensor_";

void mqtttSubmitCounter(int);
void reconnectMqtt(void);

ESP8266WebServer server(80);
WiFiClient client;
PubSubClient mqttclient(client);



/* setup method */
void setup() {
  Serial.begin(115200);
  pinMode (LED_BUILTIN, OUTPUT) ; // define LED as output interface
  pinMode (SENSOR_PIN, INPUT) ; // output interface as defined Reed sensor

  /* setup node id */
  uint8_t mac[6];
  WiFi.macAddress(mac);
  deviceId = deviceId + macToID(mac);

  EEPROM.begin(512); //Initializing EEPROM
  currentWifiMode = readWiFiMode();
  /* if double reset was set: go to AP-Moe */
  if (drd.detect()) {
    currentWifiMode = WIFIMODE_AP;
  }

  /* go to Access Point Mode */
  if (WIFIMODE_AP.equals(currentWifiMode)) {

    Serial.println("Entering Access Point Mode...");
    // AP mode
    boolean result = WiFi.softAP(deviceId);
    if (result == true) {
      Serial.println("Ready");
    }
    else {
      Serial.println("Failed!");
    }
  } else { /* go to Access Client Mode */
    // client mode
    Serial.println("Entering Client Mode...");
    connectWifi();
    connectMqtt();
    server.on("/", handleRoot);

  }

  server.on("/", handleRoot);
  server.on("/api/QueryCounter", handleJSONQuery);
  server.on("/setupwifi", handleWifiSetup);
  server.on("/setupmqtt", handleMqttSetup);
  server.on("/setupdebounceinterval", handleSetDebounceInterval);
  server.on("/setcounter", handleSetCounter);
  server.on("/style.css", handleCustCSS);
  server.on("/wifistore", handleWifiStore);
  server.on("/mqttstore", handleMqttStore);
  server.on("/debounceintervalstore", handleDebounceIntervalStore);
  server.on("/counterstore", handleCounterStore);
  ElegantOTA.begin(&server);
  server.begin();
  Serial.println("HTTP server started");

  String debounceString = readDebounceInterval();
  debounceinterval = debounceString.toInt();
  if (debounceinterval == 0) {
    debounceinterval = DEBOUNCE_INTERVAL;
  }

  //mqtt.subscribe(&reed);

}

/*
 * Perform Wifi COnnection as CLient 
 */
void connectWifi() {
  // Wait for connection
  WiFi.mode(WIFI_STA);
  WiFi.hostname(deviceId);
  WiFi.begin(readWiFiSSID(), readWiFiPass());
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(readWiFiSSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
}

/*
 * The Loop
 */
void loop() {

  /* connect Mqtt if in client mode and previously not connected */
  if (WIFIMODE_CLIENT.equals(currentWifiMode)) {
    if (!mqttclient.connected()) {
      connectMqtt();
    }
  }

  boolean countPlus = false;

  int val = digitalRead(SENSOR_PIN);

  /* Turn LED off after 1 second */
  if (millis() - lastvalidinterrupt > 1000) {
    digitalWrite (LED_BUILTIN, HIGH);
  }

  /* check Reed status */
  if (val == LOW) { 
    if (lastReedState == HIGH) { // falling level - Debouncing
      if (millis() - lastvalidinterrupt > debounceinterval) {
        Serial.println("count++");
        zaehlerstand = zaehlerstand + 1;
        countPlus = true;
        Serial.println("****************** ");
        Serial.println(zaehlerstand);
        lastvalidinterrupt = millis();
        digitalWrite (LED_BUILTIN, LOW); // turn LED ON
      } else {
        Serial.println("do nothing");
        lastvalidinterrupt = millis();
      }
    }
  }
  lastReedState = val;


  bool submitInterval = false;

  /* recalculate consumption counter. Count rotations per minute */
  if (millis() - consumptionSubmitInterval > 60000) {
    consumptionSubmitInterval = millis();
    prevConsumption = consumptioncounter;
    consumptioncounter = 0;
    submitInterval = true;
  }
  if (countPlus) {
    consumptioncounter = consumptioncounter + 1;
    Serial.println("Consumption counter: ");
    Serial.print(consumptioncounter);
    Serial.print("\n");
  }


  /* in client mode only: public reed info if theres are changes  */
  if (WIFIMODE_CLIENT.equals(currentWifiMode)) {
    MDNS.update();
    if (submitInterval) {
      mqtttSubmitUsage(prevConsumption);
    }
    if (lastsubmit != zaehlerstand) {
      mqtttSubmitCounter(zaehlerstand);
      lastsubmit = zaehlerstand;
      Serial.println("Last mqtt submit on " + String(lastsubmit, 3));
    }
  }

  server.handleClient();




}

/*
 * small tool concert mac id to String 
 */
String macToID(const uint8_t* mac)
{
  String result;
  for (int i = 3; i < 6; ++i) {
    result += String(mac[i], 16);
  }
  result.toUpperCase();
  return result;
}
