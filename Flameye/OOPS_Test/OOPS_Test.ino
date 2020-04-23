class Flasher
{
    int ledPin;
    long OnTime;
    long OffTime;

    int ledState;
    long previousMillis;

  public:
    Flasher(int pin)
    {
      ledPin = pin;
//      OnTime = on;
//      OffTime = off;

      pinMode(ledPin, OUTPUT);

      ledState = LOW;
      previousMillis = 0;
    }

    void bla(long on , long off)
    {
      OnTime = on;
      OffTime = off;
 
      unsigned long currentMillis = millis();

      if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
      {
        ledState = LOW;  // Turn it off
        previousMillis = currentMillis;  // Remember the time
        digitalWrite(ledPin, ledState);  // Update the actual LED
      }
      else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
      {
        ledState = HIGH;  // turn it on
        previousMillis = currentMillis;   // Remember the time
        digitalWrite(ledPin, ledState);   // Update the actual LED
      }
    }
};

Flasher led1(12);
Flasher led2(13);
Flasher led3(11);

void setup()
{
  
}

void loop()
{
led1.bla(100,200);
led2.bla(200,300);
led3.bla(300,400);
  
}

