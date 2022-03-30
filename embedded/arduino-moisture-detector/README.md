# Arduino Moisture detector

This is a standalone project that tries to run a moisture sensor linked to and Arduino Pro Mini while minimizing its power consumption.
Arduino device will interogate the moisture sensor at specific intervals in time and in case this falls below a specific treshold the arduino device will emit a sound via a buzzer. The moisture percentage will be notified as well every 1 hour as well via internal blinking LED.

## Components used

- Arduino Pro Mini 3.3 V
- FC-28 Soil Moisture sensor
- CP2102 USB to TTL for code upload on arduino
- YwRobot Breadboard Power Supply
- Buzzer

## Measurements Power Consumption 


Arduino pro mini 3.3 V

| Normal  |    Idle | Comments | 
| ------- | ------- | ----------- |
| 15.5 mA |         | with CP2102 USB to TTL |
| 14.7 mA |         | after removing sensor power LED |
| 13.8 mA |         | after removing arduino power LED |
| 13.8 mA | 13.0 mA | after removing sensor digital LED |
|  4.8 mA |  3.9 mA | no CP2102 USB to TTL |
|  4.8 mA, 11 mA on buzzer |  0.07 mA | Deep sleep for 8 sec |

Note: In the first 4 measurements I have forgot CP2102 USB to TTL linked to arduino, even if I have removed the USB connection. The CP2102 LED was powered bi arduino power source so measurements had an additional ~ 9.0 mA.


## Pinout used 

</br>

### Arduino Pro Mini

| Arduino Pro mini  | Other device |Description |
|:--      | :--        | :--        |
|VCC      | Battery +                       | Power connection        |
|GND      | Battery -, FC-28 GND, Buzzer -  | Ground                  |
|A0       | FC-28                           | Soil Sensor analog pin  |
|10       | FC-28                           | Soil Sensor VCC Pin     |
|9        | Buzzer                          | Buzzer + value          |


## Credits
- Nick Gammon for its power analyze on different Arduino sleep modes http://www.gammon.com.au/power