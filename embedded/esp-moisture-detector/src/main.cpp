#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include "definitions.h"
#include "constants.h"
#include "config_utils.h"
#include "utils_misc.h"

WifiConf wifiConf;
bool cold_start = false;

ESP8266WebServer server(1880);
const byte sensorPin = A0;
const byte buzzer = 9;
volatile int count = 0; // counter

void handleColdStateWebServerRequest();

void setup()
{
  Serial.begin(9600);
  Serial.println("Booting...");
  pinMode(LED_BUILTIN, OUTPUT);
  // init wifiConfig
  // to read/write wifi configuration.
  initConf(&wifiConf);
  readWifiConf(&wifiConf);

  Serial.printf("Try connect to configured accesspoint \"%s\" \n", wifiConf.wifi_ssid);
  Serial.printf("MQTT username \"%s\" \n", wifiConf.mqtt_username);
  Serial.printf("MQTT broker \"%s\" \n", wifiConf.mqtt_broker);

  if (!connectToWiFi(&wifiConf))
  {
    cold_start = true;
    setUpAccessPoint();
  }
  HandlerFunction handler = handleColdStateWebServerRequest;
  setUpWebServer(cold_start, &server, &wifiConf, handler);
  setUpOverTheAirProgramming(cold_start, &wifiConf);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.printf("Cold start %d \n", cold_start);
  Serial.println("Setup completed ...");
}

void handleColdStateWebServerRequest()
{
  bool save = serverHandleRequest(&server, &wifiConf);

  if (save)
  {
    Serial.println("Wi-Fi conf saved. Rebooting...");
    delay(1000);
    ESP.restart();
  }
}

void loop()
{
  // Give processing time for ArduinoOTA.
  // This must be called regularly
  // for the Over-The-Air upload to work.
  ArduinoOTA.handle();

  if (count % HOUR_CYCLE == 0)
  {
    readMoistureSensor(sensorPin, buzzer, count);
  }
  count++;

  // Give processing time for the webserver.
  // This must be called regularly
  // for the webserver to work.
  if (cold_start)
  {
    server.handleClient();
  }
}