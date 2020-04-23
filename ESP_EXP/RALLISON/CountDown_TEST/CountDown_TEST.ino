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
int second = 10;
int minute = 00;
unsigned char hour = 12;
boolean buzzer = false;
unsigned int TIME = 1500;
#define CLK 2//pins definitions for TM1637 and can be changed to other ports
#define DIO 3

TM1637 tm1637(CLK,DIO);
 void setup() {
 tm1637.set();
 tm1637.init();
 Timer1.initialize(500000);//timing for 500ms
 Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR

 Serial.begin(9600);
    
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
