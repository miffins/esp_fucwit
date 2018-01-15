
// INCLUDES ==================================================================
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <PubSubClient.h>
#include <Cmd.h>
#include <String.h>
#include <Bounce2.h>

void setupSerial() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("\n\n********************************************************************"));
  Serial.println(F("Fancy Updatable Configureable Wifi IoT Thingy (mk2) (2017)"));
  Serial.println(F("Craig \"magicalmiffin\" Whiffin - magicalmiffin@gmail.com"));
  Serial.println(F("********************************************************************\n"));
}
/*
 * Acronyms on a postcard, please.
 * This is Miffin's horrifying mishmash of the ArduinoOTA SecureWebUpdater example sketch, and a bunch of others.
 * It is designed to turn an ESP8266 into a useful piece of equipment.
 * 
 * Let me show you its features:
 *    Can be pre-configured with a selection of Wifi SSIDs and passwords, so you can move it between a few known networks.
 *    If it can't find one, it should create its own AP so you can join that and fix it.
 *    
 * To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/


// DECLARATIONS ==============================================================
void SetRGB(uint8_t R = 0, uint8_t G = 0, uint8_t B = 0);

#define mqttChannelIn "TESTING/in"
#define mqttChannelOut "TESTING/out"

const uint8_t CH1pin = 0;
const uint8_t CH2pin = 5;

const int LED = 2; //the little blue LED on the Witty Cloud board. Should be on all ESP-12E types.
const int LDR = A0; //light sensor.
const int BUTTON = 4; //the button on the ESP half of the Witty board.
const int RED = 15; //the red portion of the 5050RGBLED. Should be PWMed.
const int GREEN = 12; //as above, green.
const int BLUE = 13; //duh.

WiFiClient client; // or... use WiFiFlientSecure for SSL //WiFiClientSecure client;
PubSubClient mqtt(client);
Bounce btn = Bounce();
uint8_t state;

char hostString[16] = {0};
unsigned long lastConnectAttempt = 0;

const char* host = "esp8266-webupdate";
const char* update_path = "/";
const char* update_username = "miffin";
const char* update_password = "pwdsaredumb";
const char* ssid = "OvO the owlery";
const char* password = "p4ssw0rd";

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

//============================================================================

void setup() {
  setupGPIOs();
  setupSerial();
  registerCommands();
  wifi_AutoSetup();
  mDNSSetup();

  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();
  MDNS.addService("http", "tcp", 80);
  Serial.printf("HTTPUpdateServer ready! Open http://%s.local%s in your browser and login with username '%s' and password '%s'\n", host, update_path, update_username, update_password);
  
  String ip;// = AIO_SERVER; 
  uint16_t port; 
  byte ipBytes[4];
  MQTT_connect_via_mDNS(ip, &port, ipBytes);
  Serial.print(F("Connecting to MQTT server on: "));
  Serial.print(ip);
  Serial.print(":");
  Serial.println(port);

  mqtt.setServer(ipBytes, port);
  mqtt.subscribe(mqttChannelIn);
  mqtt.setCallback(mqtt_handler);
}

//============================================================================

void loop() {
  //should be an 'if wifi borked' section here
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print(F("Wifi broke. Restarting it."));
    wifi_AutoSetup();
  }
  
  if (!mqtt.connected()) {
    if (lastConnectAttempt - millis() > 600000) {
      MQTT_reconnect();
    }
  } else {
    mqtt.loop(); //check subscriptions and do callbacks if needed
  }
  
  btn.update();
  if (btn.fell()) {
    Serial.println(F("Button pressed."));
    SendMsg(F("BUTTON PRESSED"));
    state++;
    stateUpdater();
  }
  
  httpServer.handleClient();
}

void setupGPIOs() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  pinMode(LED, OUTPUT);
  pinMode(CH1pin, OUTPUT);
  pinMode(CH2pin, OUTPUT);
  pinMode(LDR, INPUT);
  pinMode(BUTTON, INPUT);

  btn.attach(BUTTON);
  btn.interval(50);
}

