#include <TimerOne.h>
#include "TM1637.h"
#define ON 1
#define OFF 0

int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
int8_t dataOff[] = {0x0, 0x0, 0x0, 0x0}; 
int8_t dataOn[] =  {0xff, 0xff, 0xff, 0xff};
unsigned char ClockPoint = 1;
unsigned char Update;
unsigned char halfsecond = 0;
int second = 00;
int minute = 10;
unsigned char hour = 12;
boolean buzzer = false;
unsigned int TIME = 1500;
#define CLK 2//pins definitions for TM1637 and can be changed to other ports
#define DIO 3


int RESET = 4;
int DECREMENT = 5;
int INCREMENT = 6;
int START = 7;


TM1637 tm1637(CLK,DIO);

void setup() {
 tm1637.set();
  tm1637.init();
 Timer1.initialize(500000);//timing for 500ms
 Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR
 stopwatchPause();
 second = 0;
  Serial.begin(9600);
pinMode(RESET, INPUT_PULLUP);
pinMode(DECREMENT, INPUT_PULLUP);
pinMode(INCREMENT, INPUT_PULLUP);
pinMode(START, INPUT_PULLUP);
}

void loop() {
 if(Update == ON)
  {
    TimeUpdate();
    tm1637.display(TimeDisp);
  }
  
  if(buzzer){
    buzz();
  }

int resetVal = digitalRead(RESET);
int decVal = digitalRead(DECREMENT);
int incVal = digitalRead(INCREMENT);
int startVal = digitalRead(START);

  if(!resetVal){
    Serial.println("RESET");
    stopwatchPause();
    minute =10;
    second =0;
    buzzer = false;
    Update = ON;
//    TimeUpdate();
    
  }
  if(!incVal){
    Serial.print("+");
//    Allow only 90 mins
    if(minute < 90){
      minute++;
    }
    Update = ON;
//     TimeUpdate();
  }
  if(!decVal){
    Serial.print("-");
    if(minute>0){
      minute= minute -1;
      Update = ON;
//       TimeUpdate();
    }
  }
  if(!startVal){
    Serial.print("START");
    Update = ON;
     stopwatchStart();
//    buzzer = false;
//    TimeUpdate();
  }
  delay(120);
}
void TimingISR()
{
  halfsecond++;
  if(minute >= 0 ){
    Update = ON;
  }
  if(halfsecond == 2){
    second --;
    if(second == 0 )
    {
      minute --;
      if(minute <= 0){
        buzz();
        buzzer = true;
        Update = OFF;
      }
      second = 60;
    }
    halfsecond = 0;
  }
  ClockPoint = (~ClockPoint) & 0x01;
}
void TimingSTOP(){
  buzz();
}
void TimeUpdate(void)
{
  if(ClockPoint)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF);
  TimeDisp[0] = minute / 10;
  TimeDisp[1] = minute % 10;
  TimeDisp[2] = second / 10;
  TimeDisp[3] = second % 10;
  Update = OFF;
}

void buzz(){
  
  Serial.println("Times up");
  tm1637.display(dataOff);
  tm1637.point(false);
  delay(500);
  tm1637.point(true);
  tm1637.clearDisplay();
  delay(500);
  
}

void stopwatchStart()//timer1 on
{
//  TCCR1B |= Timer1.clockSelectBits;
TCCR1B |= (1 << CS11);

}
void stopwatchPause()//timer1 off if [CS12 CS11 CS10] is [0 0 0].
{
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
}
void stopwatchReset()
{
  stopwatchPause();
  second = 0;
  minute = 0;
  Update = ON;
}

