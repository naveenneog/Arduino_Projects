/*
 * MotorKnob
 *
 * A stepper motor follows the turns of a potentiometer
 * (or other sensor) on analog input 0.
 *
 * http://www.arduino.cc/en/Reference/Stepper
 * This example code is in the public domain.
 */

#include <Stepper.h>

// change this to the number of steps on your motor
#define STEPS 100
//int EN_PIN_A = 5;
int EN_PIN = 7;
int EN_PIN_1 = 6;
//int EN_PIN_B = 5;
//int EN_PIN_C = 5;
// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 8, 9, 10, 11);
//int A = 5;
//int B = 6;
// the previous reading from the analog input
//int previous = 0;

void setup()  
{
  // set the speed of the motor to 30 RPMs
  stepper.setSpeed(50);
  pinMode(EN_PIN,OUTPUT);
  pinMode(EN_PIN_1,OUTPUT);
//  pinMode(B,OUTPUT);
//  pinMode(8,OUTPUT);
//  pinMode(EN_PIN_A,OUTPUT);
//  pinMode(EN_PIN_B,OUTPUT);
//  pinMode(EN_PIN_C,OUTPUT);
//  digitalWrite(12,1);
//  digitalWrite(B,0);
}

void loop()
{
  // get the sensor value
  //int val = analogRead(0);

  // move a number of steps equal to the change in the
  // sensor reading
  
//rotateStepper(EN_PIN_A , 20);
rotateStepperEver(200);
delay(100);
//rotateStepper(EN_PIN_B , 30);
rotateStepperEver(-300);
delay(100);
//rotateStepper(EN_PIN_C , 40);
rotateStepperEver(300);
delay(100);
  // remember the previous value of the sensor
  //previous = val;
}

void rotateStepper(int Enable_Pin , int diff){
  digitalWrite(Enable_Pin,1);
  stepper.step(diff+diff);
  delay(300);
  stepper.step(-diff);
  digitalWrite(Enable_Pin,0);
}
void rotateStepperEver( int diff) {
        digitalWrite(EN_PIN, 1);
        digitalWrite(EN_PIN_1, 1);
        stepper.step(diff);
        delay(10);
        digitalWrite(EN_PIN, 0);
         digitalWrite(EN_PIN_1, 0);
 }
 
