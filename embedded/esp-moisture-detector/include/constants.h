#ifndef __CONSTANTS_H_
#define __CONSTANTS_H_

// html documents
const char* const DOC_HEADER = "<!DOCTYPE html><html><head><title>ESP8266 conf</title></head><body>";
const char* const DOC_FOOTER ="</body></html>";
const char* const DOC_REBOOTING ="<div>Saved! Rebooting...</div>";
const char* const DOC_FORM = "<h1>Wi-Fi conf</h1><form action='/' method='POST'> \
<div>SSID:</div> \
<div><input type='text' name='ssid' value='$SSID'/></div> \
<div>Password:</div> \
<div><input type='password' name='password' value='$PASSWORD'/></div> \
<div>MQTT user:</div> \
<div><input type='text' name='mqtt_user' value='$MQTTUSER'/></div> \
<div>MQTT password:</div> \
<div><input type='password' name='mqtt_password' value='$MQTTPASSORD'/></div> \
<div>OTA password:</div> \
<div><input type='password' name='ota_password' value='$OTAPASSORD'/></div> \
<div><input type='submit' value='Save'/></div></form>";



// Configuration for fallback access point
// if Wi-Fi connection fails.
const char* const AP_ssid = "ESP8266_fallback_AP";
const char* const AP_password = "SuperSecretPassword";

#endif //__CONSTANTS_H_