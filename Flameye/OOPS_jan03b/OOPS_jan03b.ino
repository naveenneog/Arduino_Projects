
#include <Stepper.h>

int MOTOR_DRIVER_IN_0 = 6; //PD6(10)
int MOTOR_DRIVER_IN_1 = 7; //PD7(11)
int MOTOR_DRIVER_IN_2 = 8; //PB0(12)
int MOTOR_DRIVER_IN_3 = 9; //PB1(13)

#define STEPS 100

//Motor Handler Class
class Motron
{
    int enable_Pin;
    long prev_Value;
    long val;
    long diff;

  public:
    Motron(int en_Pin)
    {
      enable_Pin = en_Pin;
      val = 0;
      prev_Value = 0;
      diff = 0;
    }

    void rotate(long value)
    {
      Serial.println("Knob :" + enable_Pin );
      Serial.println("Value ::" + value);
      prev_Value = value;
      value = val;
      diff = value - prev_Value;

    }
};

int SWITCHING_REGULATOR_EN = 10; //(PB2)
//Stepper Motor Rotator Class
class rotateStepper
{
    int motorPin;
    Stepper stepper(STEPS, MOTOR_DRIVER_IN_0, MOTOR_DRIVER_IN_1, MOTOR_DRIVER_IN_2, MOTOR_DRIVER_IN_3);

  public:
    rotateStepper(int motor_enable_pin)
    {
      motorPin = motor_enable_pin;
      
    }

    void rotateStepper(long diff)
    {
      digitalWrite(SWITCHING_REGULATOR_EN , HIGH);
      digitalWrite(motorPin, 1);
      stepper.step(diff);
      digitalWrite(motorPin, 0);
      digitalWrite(SWITCHING_REGULATOR_EN , LOW);
    }
};
//Flame Sensor Reading every 1 Second Class


void setup() {

}

void loop() {

}
