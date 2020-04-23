#include "TM1637.h"
#define CLK 2//pins definitions for TM1637 and can be changed to other ports
#define DIO 3
#define RESET 4
#define START 7
#define INRMNT 6
#define DCRMNT 5
TM1637 tm1637(CLK,DIO);


void setup()
{
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  pinMode(RESET, INPUT);
  pinMode(START, INPUT);
  pinMode(INRMNT, INPUT);
  pinMode(DCRMNT, INPUT);
  Serial.begin(9600);
}
void loop()
{
  int8_t NumTab[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};//0~9,A,b,C,d,E,F 


  for(int j=0; j<= sizeof(NumTab); j++){
    tm1637.display(0,NumTab[j]);
    tm1637.display(1,NumTab[j]);
    tm1637.display(2,NumTab[j]);
    tm1637.display(3,NumTab[j]);
    delay(300);
  }
}
