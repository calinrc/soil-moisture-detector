#ifndef __UTILS_MISC_H_
#define __UTILS_MISC_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
#include "config_utils.h"
#include "constants.h"

IPAddress AP_IP = IPAddress(10, 1, 1, 1);
IPAddress AP_subnet = IPAddress(255, 255, 255, 0);

typedef void (*HandlerFunction)();

bool connectToWiFi(WifiConf *wifiConf)
{
  Serial.printf("Connecting to '%s'\n", wifiConf->wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiConf->wifi_ssid, wifiConf->wifi_password);
  if (WiFi.waitForConnectResult() == WL_CONNECTED)
  {
    Serial.print("Connected. IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }
  else
  {
    Serial.println("Connection Failed!");
    return false;
  }
}

void setUpAccessPoint()
{
  Serial.println("Setting up access point.");
  Serial.printf("SSID: %s\n", AP_ssid);
  Serial.printf("Password: %s\n", AP_password);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(AP_IP, AP_IP, AP_subnet);
  if (WiFi.softAP(AP_ssid, AP_password))
  {
    Serial.print("Ready. Access point IP: ");
    Serial.println(WiFi.softAPIP());
  }
  else
  {
    Serial.println("Setting up access point failed!");
  }
}

void setUpOverTheAirProgramming(bool coldStart, WifiConf *wifiConf)
{

  // Change OTA port.
  // Default: 8266
  // ArduinoOTA.setPort(8266);

  // Change the name of how it is going to
  // show up in Arduino IDE.
  // Default: esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // Re-programming passowrd.
  // No password by default.
  if (!coldStart)
  {
    ArduinoOTA.setPassword(wifiConf->ota_password);
  }

  ArduinoOTA.begin();
}

void setUpWebServer(bool coldStart, ESP8266WebServer *server, WifiConf *wifiConf, HandlerFunction handleWebServerRequest)
{
  if (coldStart)
  {
    server->on("/", handleWebServerRequest);
    server->begin();
  }
}

bool serverHandleRequest(ESP8266WebServer *server, WifiConf *wifiConf)
{
  bool save = false;

  if (server->hasArg("ssid") &&
      server->hasArg("password") &&
      server->hasArg("mqtt_broker") &&
      server->hasArg("mqtt_user") &&
      server->hasArg("mqtt_password") &&
      server->hasArg("ota_password"))
  {
    server->arg("ssid").toCharArray(wifiConf->wifi_ssid, sizeof(wifiConf->wifi_ssid));
    server->arg("password").toCharArray(wifiConf->wifi_password, sizeof(wifiConf->wifi_password));
    server->arg("mqtt_broker").toCharArray(wifiConf->mqtt_broker, sizeof(wifiConf->mqtt_broker));
    server->arg("mqtt_user").toCharArray(wifiConf->mqtt_username, sizeof(wifiConf->mqtt_username));
    server->arg("mqtt_password").toCharArray(wifiConf->mqtt_password, sizeof(wifiConf->mqtt_password));
    server->arg("ota_password").toCharArray(wifiConf->ota_password, sizeof(wifiConf->ota_password));

    Serial.println(server->arg("ssid"));
    Serial.println(wifiConf->wifi_ssid);

    Serial.println(server->arg("mqtt_broker"));
    Serial.println(wifiConf->mqtt_broker);

    Serial.println(server->arg("mqtt_user"));
    Serial.println(wifiConf->mqtt_username);

    writeWifiConf(&wifiConf);
    save = true;
  }

  String message = "";
  message += DOC_HEADER;
  if (save)
    message += DOC_REBOOTING;
  else
  {
    String form = String(DOC_FORM);
    form.replace("$SSID", wifiConf->wifi_ssid);
    form.replace("$PASSWORD", wifiConf->wifi_password);
    form.replace("$MQTTBROKER", wifiConf->mqtt_broker);
    form.replace("$MQTTUSER", wifiConf->mqtt_username);
    form.replace("$MQTTPASSORD", wifiConf->mqtt_password);
    form.replace("$OTAPASSORD", wifiConf->ota_password);
    message += form;
  }

  message += DOC_FOOTER;
  server->send(200, "text/html", message);
  return save;
}

void blinkNumber(int a)
{
  int firstDigit = a % 10;
  int secondDigit = (a / 10);
  for (int i = 0; i <= secondDigit; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(150);
    digitalWrite(LED_BUILTIN, LOW);
    delay(150);
  }
  delay(1000);

  for (int i = 0; i <= firstDigit; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(150);
    digitalWrite(LED_BUILTIN, LOW);
    delay(150);
  }
}

const unsigned long DAY_SEC = 86400;
const unsigned long HOUR_SEC = 3600;
const unsigned int CYCLE_SEC = 8;
const unsigned long HOUR_CYCLE = HOUR_SEC / CYCLE_SEC - 1; // one cycle is used to measure the moisture
const unsigned long DAY_CYCLE  = (DAY_SEC / CYCLE_SEC) - 24; // onece a day tone the the piezo speaker


void readMoistureSensor(int sensorPin, int buzzer , int count)
{
  // Serial.println("Reading soil moisture");

  digitalWrite(sensorPin, HIGH);
  delay(8 * 1000);
  int analog_value = analogRead(A0);
  digitalWrite(sensorPin, LOW);
  int moisture = map(analog_value, 0, 1023, 0, 100);
  moisture = 100 - moisture;
  Serial.print("Moisture in soil = ");
  Serial.print(moisture);
  Serial.print(" batch count = ");
  Serial.println(count);
  Serial.flush();

  if (moisture < 50 && count % DAY_CYCLE == 0 )
  {
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
  }

  blinkNumber(moisture);
}

#endif //__UTILS_MISC_H_