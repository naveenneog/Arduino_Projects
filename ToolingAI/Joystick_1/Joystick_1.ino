

#include <Stepper.h>
const int stepsPerRevolution = 100;  // change this to fit the number of steps per revolution
// for your motor

int ledPin = 13;
 int joyPin1 = 0;                 // slider variable connecetd to analog pin 0
 int joyPin2 = 1;                 // slider variable connecetd to analog pin 1
 int value1 = 0;                  // variable to read the value from the analog pin 0
 int value2 = 0;                  // variable to read the value from the analog pin 1

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

 void setup() {
  // set the speed at 60 rpm:
  pinMode(ledPin, OUTPUT);              // initializes digital pins 0 to 7 as outputs
  myStepper.setSpeed(60);
  Serial.begin(9600);
 }

 int treatValue(int data) {
  if(data < 300 && data >= 0){
    return -1;
  }else if(data >600 && data <1500){
    return 1;
  }else {
    return 0;
  }
  
 }

  void rotateMotorX(int direction){
    if( direction > 0 ){
       myStepper.step(stepsPerRevolution) ;
    }else if (direction < 0 ) {
       myStepper.step(-stepsPerRevolution);
    }
    delay(10);
    
  }
 void loop() {
  // reads the value of the variable resistor
  value1 = analogRead(joyPin1);  
  // this small pause is needed between reading
  // analog pins, otherwise we get the same value twice
  delay(100);            
  // reads the value of the variable resistor
  value2 = analogRead(joyPin2);  


  Serial.print('X');
  Serial.print(treatValue(value1));
  rotateMotorX(treatValue(value1));
  Serial.print('Y');
  Serial.println(treatValue(value2));
//  rotateMotorY(treatValue(value1));
  delay(100);
 }
