/**
 * Test Case 1 
 * Use case run all 3 motors starting from M1,M2,M3 for 5 Secs and loop through 10 Times AND Stop
 * With out Bluetooth
 * Tester : Neo-G
 */
#include <Stepper.h>
#define STEPS 100


int En_MD1 = 3; // PD5(9)
int En_MD2 = 4; // PD4(2)
int En_MD3 = 5; // PD3(1)

int SWITCHING_REGULATOR_EN = 10; //(PB2)

int MOTOR_DRIVER_IN_0 = 6; //PD6(10)
int MOTOR_DRIVER_IN_1 = 7; //PD7(11)
int MOTOR_DRIVER_IN_2 = 8; //PB0(12)
int MOTOR_DRIVER_IN_3 = 9; //PB1(13)

const int LOOP_COUNT = 10;

Stepper stepper(STEPS, MOTOR_DRIVER_IN_0, MOTOR_DRIVER_IN_1, MOTOR_DRIVER_IN_2, MOTOR_DRIVER_IN_3);

class Knob
{
    //Class Member Variables
    int enable_Pin;
    long int value;
    long int prev_Value;
    long int diff;

  public:
    Knob(int en_Pin)
    {
      enable_Pin = en_Pin;
      value = 0;
      prev_Value = 0;
      diff = 0;

    }

    void Rotate(int val)
    {
      Serial.println("Object Val" + String(val));
      prev_Value = value;
      value = val;
      diff = value - prev_Value;
      //      delay(100);
      //      Serial.println("Knob object :"+val);
      Serial.println("Difference :" + String(diff));
      Serial.println("Motor Number:"+String(enable_Pin));
      rotateStepper(enable_Pin , diff);
    }

    void rotateStepper(int Enable_Pin , int diff) {
      digitalWrite(SWITCHING_REGULATOR_EN , HIGH);
      digitalWrite(Enable_Pin, 1);
      stepper.step(diff);
      digitalWrite(Enable_Pin, 0);
      digitalWrite(SWITCHING_REGULATOR_EN , LOW);

    }

};


Knob knob1(En_MD1);
Knob knob2(En_MD2);
Knob knob3(En_MD3);


void setup() {
stepper.setSpeed(30);
  //Declaring Enable Pins
  pinMode(En_MD1, OUTPUT);
  pinMode(En_MD2, OUTPUT);
  pinMode(En_MD3, OUTPUT);

  //Declaring the Power Saving Pin (Switching Regulator Pin)
  pinMode(SWITCHING_REGULATOR_EN, OUTPUT);
  digitalWrite(SWITCHING_REGULATOR_EN , LOW);
  //Serial Interfaces setup
  Serial.begin(9600);
  Serial.println("Hello BlueSpider");

}

void loop() {
  // put your main code here, to run repeatedly:
   
        Serial.println("Turning Positive Turns");
        TestTurns(100);
         Serial.println("Turning Negative Turns");
        TestTurns(-100);
    

}
void TestTurns(int turns){
    knob1.Rotate(turns);
    delay(100);
    knob2.Rotate(turns);
    delay(100);
    knob3.Rotate(turns);
    delay(100);
}


