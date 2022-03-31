#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include "definitions.h"
#include "constants.h"
#include "config_utils.h"
#include "utils_misc.h"
#include <PubSubClient.h>

WifiConf wifiConf;
bool cold_start = false;

ESP8266WebServer server(1880);
static const uint8_t POWER_PIN = D1; // GPIO 5
const byte buzzer = 9;
volatile int count = 0; // counter
WiFiClient espClient;
PubSubClient client(espClient);

#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int moisture_value;
const char *broker_topic = "home/sensors/moisture";
bool published;
String clientId = "ESP8266Client-";


void handleColdStateWebServerRequest();

void reconnect(WifiConf *wifiConf, const char *client_id)
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    // Attempt to connect
    if (client.connect(client_id, wifiConf->mqtt_username, wifiConf->mqtt_password))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("outTopic", "hello world");
      // // ... and resubscribe
      // client.subscribe("inTopic");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Booting...");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW);
  // init wifiConfig
  // to read/write wifi configuration.
  initConf(&wifiConf);
  readWifiConf(&wifiConf);

  Serial.printf("Try connect to configured accesspoint \"%s\" \n", wifiConf.wifi_ssid);
  Serial.printf("MQTT username \"%s\" \n", wifiConf.mqtt_username);
  Serial.printf("MQTT broker \"%s\" \n", wifiConf.mqtt_broker);
  clientId += String(random(0xffff), HEX);
  Serial.printf("MQTT client_id \"%s\" \n", clientId.c_str());



  if (!connectToWiFi(&wifiConf))
  {
    cold_start = true;
    setUpAccessPoint();
    HandlerFunction handler = handleColdStateWebServerRequest;
    setUpWebServer(cold_start, &server, &wifiConf, handler);
  }
  else
  {
    client.setServer(wifiConf.mqtt_broker, 1883);
  }
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

  if (count % CYCLE_SEC == 0)
  {
    moisture_value = readMoistureSensor(POWER_PIN, buzzer, count);
    if (!client.connected())
    {
      Serial.println("Reconnect PubSub Client");

      reconnect(&wifiConf, clientId.c_str());
    }
    snprintf(msg, MSG_BUFFER_SIZE, "%d", moisture_value);
    Serial.printf("Publish message: \"%s\"\n",msg );
    published = client.publish(broker_topic, msg);
    Serial.printf("Complete publishing with status \"%d\"\n", published);
  }
  count++;
  delay(8 * 1000);

  // Give processing time for the webserver.
  // This must be called regularly
  // for the webserver to work.
  if (cold_start)
  {
    server.handleClient();
  }
}