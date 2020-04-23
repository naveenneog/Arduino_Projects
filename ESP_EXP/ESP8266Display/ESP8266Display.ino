/**
* LCD circuit:
* LCD RS pin to digital pin 12
* LCD Enable pin to digital pin 11
* LCD D4 pin to digital pin 5
* LCD D5 pin to digital pin 4
* LCD D6 pin to digital pin 3
* LCD D7 pin to digital pin 2
* LCD R / W pin to ground
* 10K resistor:
* ends at + 5V and ground
* wiper to LCD VO pin (pin 3)
*/



//// Pin connected to latch pin (ST_CP)(12) of 74HC595
//const int latchPin = 0;
//// Pin connected to clock pin (SH_CP)(11) of 74HC595
//const int clockPin = 1;
////// Pin connected to Data in (DS)(14) of 74HC595
//const int dataPin = 3;

int latchPin = 5;
int clockPin = 4;
int dataPin = 3;
 
byte leds = 0x00000011;
 
void setup() 
{
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
}
 
void loop() 
{
//  leds = 0;
//  updateShiftRegister();
//  delay(500);
//  for (int i = 0; i < 8; i++)
//  {
//    bitSet(leds, i);
//    Serial.println(leds);
//    updateShiftRegister();
//    Serial.println(String(leds));
//    Serial.println(leds);
//    delay(500);
//  }
//  bitSet(leds,0);
//  bitSet(leds,2);
  Serial.println(leds);
  updateShiftRegister();
  delay(100);
}
 
void updateShiftRegister()
{
   digitalWrite(latchPin, LOW);
   shiftOut(dataPin, clockPin, MSBFIRST, leds);
   digitalWrite(latchPin, HIGH);
}
