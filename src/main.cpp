#include <Arduino.h>

void setup()
{
  Serial.begin(9600);
#define sensorPin 10
#define buzzPositive 8
#define buzzNegative 9
}

void loop()
{
  digitalWrite(sensorPin, HIGH);
  delay(100);
  int analog_value = analogRead(A0);
  int moisture = map(analog_value, 0, 1023, 0, 100);
  if (moisture <= 40)
  {
    digitalWrite(buzzPositive, HIGH);
    digitalWrite(buzzNegative, LOW);
    delay(30 * 1000);
    digitalWrite(buzzPositive, LOW);
    digitalWrite(buzzNegative, LOW);
  }
  else
  {
    digitalWrite(buzzPositive, LOW);
    digitalWrite(buzzNegative, LOW);
  }
  Serial.print("Moisture in soil = ");
  Serial.println(moisture);
  digitalWrite(sensorPin, LOW);
  delay(10*1000);
}
