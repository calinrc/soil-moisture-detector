#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/power.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_PCD8544.h>

const byte LED = LED_BUILTIN;

const byte sensorPin = 10;
const byte buzzer = 9;
int analog_value;
int moisture;
volatile int count = 0; // counter
const unsigned long DAY_SEC = 86400;
const unsigned long HOUR_SEC = 3600;
const unsigned int CYCLE_SEC = 8;
const unsigned long HOUR_CYCLE = HOUR_SEC / CYCLE_SEC - 1; // one cycle is used to measure the moisture
const unsigned long DAY_CYCLE  = (DAY_SEC / CYCLE_SEC) - 24; // onece a day tone the the piezo speaker

//const unsigned long HOUR_CYCLE = 3;
//const unsigned long DAY_CYCLE  = 6;


// LCD 5110 Software SPI (slower updates, more flexible pin options):
// D5 - Serial clock out (SCLK)
// D7 - Serial data out (DIN)
// D2 - Data/Command select (D/C)
// D8 - LCD chip select (CS)
// D0 - LCD reset (RST)
//int8_t DC, int8_t CS, int8_t RST
//Adafruit_PCD8544 display = Adafruit_PCD8544(D2, D8, D0);

//int8_t SCLK, int8_t DIN, int8_t DC, int8_t CS, int8_t RST
//Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

//int8_t DC, int8_t CS, int8_t RST
// Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);


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

void readMoistureSensor()
{
  // Serial.println("Reading soil moisture");

  digitalWrite(sensorPin, HIGH);
  delay(8 * 1000);
  // {
  //   digitalWrite(buzzPositive, HIGH);
  //   digitalWrite(buzzNegative, LOW);
  //   delay(5 * 1000);
  //   digitalWrite(buzzPositive, LOW);
  //   digitalWrite(buzzNegative, LOW);
  // }
  // else
  // {
  //   digitalWrite(buzzPositive, LOW);
  //   digitalWrite(buzzNegative, LOW);
  // }
  analog_value = analogRead(A0);
  digitalWrite(sensorPin, LOW);
  moisture = map(analog_value, 0, 1023, 0, 100);
  moisture = 100 - moisture;
  // display.clearDisplay();
	// // Dump debug info about the card; PICC_HaltA() is automatically called
	// display.setCursor(0,0);
  // display.println("Moisture in ");
  // display.print("soil=");
  // display.println(moisture);
  // display.print("\nCycle=");
  // display.println(count);
  // display.display();

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

// watchdog interrupt
ISR(WDT_vect)
{
  wdt_disable(); // disable watchdog
} // end of WDT_vect

void sleepNow()
{
  // this method is based on http://www.gammon.com.au/power
  // also on https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf (page 35)
  // disable ADC
  //byte adcsra_save = ADCSRA;
  ADCSRA = 0;
  power_all_disable();

  // clear various "reset" flags
  MCUSR = 0;
  // allow changes, disable reset
  WDTCSR = bit(WDCE) | bit(WDE);
  // set interrupt mode and an interval
  WDTCSR = bit(WDIE) | bit(WDP3) | bit(WDP0); // set WDIE, and 8 seconds delay
  wdt_reset();                                // pat the dog

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  noInterrupts(); // timed sequence follows
  sleep_enable();

  // turn off brown-out enable in software
  MCUCR = bit(BODS) | bit(BODSE);
  MCUCR = bit(BODS);
  interrupts(); // guarantees next instruction executed
  sleep_cpu();

  // cancel sleep as a precaution
  sleep_disable();
  // ADCSRA = adcsra_save;  // stop power reduction
  power_all_enable(); // turn on all modules
}

void setup()
{
  count = 0;
  Serial.begin(9600);
  Serial.println("Rebooted ... ");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(sensorPin, OUTPUT);
  digitalWrite(sensorPin, LOW);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  // display.begin();
	// display.setContrast(50);

	// display.clearDisplay();
	// // text display tests
	// display.setRotation(2);
	// display.setTextSize(1);
	// display.setTextColor(BLACK);
	// display.setCursor(0,0);
	// display.println("\n\nRebooted ... \n\n\n");
	// display.display();
  
}

void loop()
{

  if (count % HOUR_CYCLE == 0)
  {
    readMoistureSensor();
  }
  count++;

  sleepNow();

} // end of loop