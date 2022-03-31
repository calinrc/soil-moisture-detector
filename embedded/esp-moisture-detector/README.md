# ESP Moisture detector

This is a ESP8266 arduino project used to monitor a soil moisture sensor and send its data via mqtt protocol to a monitor application deployed on a RaspberryPI device [see](../../service/events_tracer/README.md)

## Components used

- ESP-12S 3.3 V
- FC-28 Soil Moisture sensor
- CP2102 USB to TTL for code upload on arduino
- YwRobot Breadboard Power Supply (for development purpose only)

## Measurements Power Consumption 


ESP-12S 3.3 V

| Normal  |    Idle | Comments | 
| ------- | ------- | ----------- |
| TBD |         | with CP2102 USB to TTL |
| TBD |  TBD | no CP2102 USB to TTL |


## Pinout used 

</br>

### ESP-12S

| ESP-12S  | Other device |Description |
|:--      | :--        | :--        |
|VCC      | Battery +                       | Power connection        |
|GND      | Battery -, FC-28 GND            | Ground                  |
|A0       | FC-28                           | Soil Sensor analog pin  |
|TBD      | FC-28                           | Soil Sensor VCC Pin     |


## References
- https://randomnerdtutorials.com/esp8266-adc-reading-analog-values-with-nodemcu/