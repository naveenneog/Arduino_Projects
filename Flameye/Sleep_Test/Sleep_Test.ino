#include <avr/sleep.h>

int En_MD1 = 3; // PD5(9)
int En_MD2 = 4; // PD4(2)
int En_MD3 = 5; // PD3(1)
int SWITCHING_REGULATOR_EN = 10; //(PB2)
void setup () 
{
  
//  set_sleep_mode (SLEEP_MODE_EXT_STANDBY);  
//  sleep_enable();
//  sleep_cpu ();  
//  pinMode(En_MD1, OUTPUT);
//  pinMode(En_MD2, OUTPUT);
//  pinMode(En_MD3, OUTPUT);
//  //Declaring the Power Saving Pin (Switching Regulator Pin)
//  pinMode(SWITCHING_REGULATOR_EN, OUTPUT);
//  digitalWrite(SWITCHING_REGULATOR_EN , LOW);
}  // end of setup

void loop () {
  delay(1000);
  }
