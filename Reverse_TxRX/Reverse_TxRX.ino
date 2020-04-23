#include <SoftwareSerial.h>
#include <Stepper.h>
//doesnt work 

// change this to the number of steps on your motor
#define STEPS 100
int En_MD1 = 3; // PD5(9)
int En_MD2 = 4; // PD4(2)
int En_MD3 = 5; // PD3(1)

int MOTOR_DRIVER_IN_0 = 6; //PD6(10)
int MOTOR_DRIVER_IN_1 = 7; //PD7(11)
int MOTOR_DRIVER_IN_2 = 8; //PB0(12)
int MOTOR_DRIVER_IN_3 = 9; //PB1(13)

int red = 0;
int green = 0;
int blue = 0;1
int prev_red = 0;
int diff = 0;
int int_pos[3];
int enteredFlag = 0;
SoftwareSerial reverseSerial(1,0);
Stepper stepper(STEPS, MOTOR_DRIVER_IN_0, MOTOR_DRIVER_IN_1, MOTOR_DRIVER_IN_2, MOTOR_DRIVER_IN_3);
// the previous reading from the analog input
int previous = 0;


void setup() {
  // put your setup code here, to run once:
reverseSerial.begin(9600);
reverseSerial.println("Hello Reverse World");
}

void loop() {
if(reverseSerial.available()){
      for(int i=0;i<3;i++){
       String chunk = reverseSerial.readStringUntil(',');
        if(chunk != "") {
          int_pos[i] = chunk.toInt();
        if(i==0){
             prev_red = red;
             red = int_pos[i];
             diff = red - prev_red;
             rotateStepper(En_MD1 , red);
        }
         reverseSerial.println("Pos"+String(int_pos[i]));
         
      }
     }

  }

delay(50);
}
void rotateStepper(int Enable_Pin , int diff){
  digitalWrite(Enable_Pin,1);
  stepper.step(diff);
  delay(20);
  digitalWrite(Enable_Pin,0);
}
