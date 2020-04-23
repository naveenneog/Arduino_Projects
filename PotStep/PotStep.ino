/**
 * Mapping the Potentiometer Values to the Stepper motor
 */

 #include <Stepper.h>

 #define STEPS 100
 int EN_PIN_A = 6;
 int potpin = 0;  // analog pin used to connect the potentiometer
  int val = 0;
  int preVal = 0;
  int diff = 0;
 
 Stepper stepper(STEPS, 8, 9, 10, 11);
 
void setup() {
  stepper.setSpeed(30);
  pinMode(EN_PIN_A,OUTPUT);
  
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
 val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
 val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
 diff = val - preVal;
 preVal = val;
 rotateStepper(EN_PIN_A,diff);

}


void rotateStepper(int Enable_Pin , int diff){
  digitalWrite(Enable_Pin,1);
  stepper.step(diff+diff);
  delay(15);
  stepper.step(-diff);
  digitalWrite(Enable_Pin,0);
}
 
