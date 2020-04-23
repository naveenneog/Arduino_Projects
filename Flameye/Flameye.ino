/**
   Author : Neo-G
   Connect the Programmed Pins
*/

#include <Stepper.h>

//change this to the number of steps on your motor
#define STEPS 100

const int RTD_1_ANALOG_A0 = A0;
const int RTD_2_ANALOG_A1 = A1;
const int RTD_3_ANALOG_A2 = A2;
const int ENC_0_ANALOG_A3 = A3;
const int ENC_1_ANALOG_A4 = A4;
const int ENC_2_ANALOG_A5 = A5;
const int ANALOG_A6 = A6;
const int ANALOG_A7 = A7;



int En_MD1 = 3; // PD5(9)
int En_MD2 = 4; // PD4(2)
int En_MD3 = 5; // PD3(1)

int SWITCHING_REGULATOR_EN = 10; //(PB2)

int MOTOR_DRIVER_IN_0 = 6; //PD6(10)
int MOTOR_DRIVER_IN_1 = 7; //PD7(11)
int MOTOR_DRIVER_IN_2 = 8; //PB0(12)
int MOTOR_DRIVER_IN_3 = 9; //PB1(13)
// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to


//Variables Declarations
byte oneKnob = 0;
byte twoKnob = 0;
byte threeKnob = 0; //Var to handle 3 Sliders


byte prev_oneKnob = 0; // Var to hold the Previous Values
byte prev_twoKnob = 0;
byte prev_threeKnob = 0;

int diff_oneKnob = 0; //Var to hold Differences
int diff_twoKnob = 0;
int diff_threeKnob = 0;

byte int_pos[3]; //Position Variable array

int intChunk = 0;

Stepper stepper(STEPS, MOTOR_DRIVER_IN_0, MOTOR_DRIVER_IN_1, MOTOR_DRIVER_IN_2, MOTOR_DRIVER_IN_3);
// the previous reading from the analog input
int previous = 0;

void setup() {
  // put your setup code here, to run once:
  // set the speed of the motor to 30 RPMs
  stepper.setSpeed(30);
  pinMode(En_MD1, OUTPUT);
  pinMode(En_MD2, OUTPUT);
  pinMode(En_MD3, OUTPUT);
  //Serial Interfaces setup
  Serial.begin(9600);
  Serial.println("Hello Spoorthi");

}

void loop() {

  if (Serial.available()) {
    for (int i = 0; i < 3; i++) {

      String chunk = Serial.readStringUntil(',');

      if (chunk != "" ) {
        intChunk = chunk.toInt();
        if (intChunk < 2000) {
          int_pos[i] = chunk.toInt();
          //Serial.println("Read Chunk : " + chunk);

          switch (i) {
            case 0 :  // When the Flame 1 slider is moved
                prev_oneKnob = oneKnob;
                oneKnob = int_pos[i];
                diff_oneKnob = oneKnob - prev_oneKnob;
                rotateStepper(En_MD1 , diff_oneKnob/3);
                            //Serial.print("oneKnob_Pos : " + String(oneKnob));
                            //Serial.println( " Prev_Position :" + String(prev_oneKnob));
                            //Serial.println("Difference oneKnob : " + String(diff_oneKnob));
              
              break;
            case 1: 
                prev_twoKnob = twoKnob;
                twoKnob = int_pos[i];
                diff_twoKnob = twoKnob - prev_twoKnob;
                            //Serial.print("twoKnob_Pos : " + String(twoKnob));
                            //Serial.println(" Prev_Position :" + String(prev_twoKnob));
                            //Serial.println("Difference twoKnob : " + String(diff_twoKnob));
                rotateStepper(En_MD2 , diff_twoKnob/3);

              
              break;
            case 2:
                prev_threeKnob = threeKnob ;
                threeKnob = int_pos[i];
                diff_threeKnob = threeKnob - prev_threeKnob;
                            //Serial.println("threeKnob_Pos : " + String(threeKnob));
                            //Serial.print("Prev_Position :" + String(prev_threeKnob));
                            //Serial.println("Difference threeKnob: " + String(diff_threeKnob));
                rotateStepper(En_MD3 , diff_threeKnob/3);
              
              break;
          }
        }
        else {
          switch (intChunk) {
            case 2000: //turn off knob 1
              //Serial.println("Turn off Knob 1");
              rotateStepper(En_MD1 , 180);
              break;
            case 3000: //Serial.println("Turn off knob 2");
              rotateStepper(En_MD2 , 180);
              break;
            case 4000: //Serial.println("Turn off knob 3");
              rotateStepper(En_MD3 , 180);
              break;
          }
        }
      }
    }
  }
  delay(20);
}
//  Serial.println("Testing circuit");


void rotateStepper(int Enable_Pin , int diff) {
  digitalWrite(Enable_Pin, 1);
  stepper.step(diff);
  digitalWrite(Enable_Pin, 0);
//  delay(20);
}

