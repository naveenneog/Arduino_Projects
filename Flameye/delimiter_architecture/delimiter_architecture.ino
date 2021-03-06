#include <Event.h>
#include <Timer.h>

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
unsigned int oneKnob = 0;
unsigned int twoKnob = 0;
unsigned int threeKnob = 0; //Var to handle 3 Sliders


unsigned int prev_oneKnob = 0; // Var to hold the Previous Values
unsigned int prev_twoKnob = 0;
unsigned int prev_threeKnob = 0;

int diff_oneKnob = 0; //Var to hold Differences
int diff_twoKnob = 0;
int diff_threeKnob = 0;

int prev_Flame_Sensor_1_Data = 0;//Variable to transfer the last read sensor data
int prev_Flame_Sensor_2_Data = 0;
int prev_Flame_Sensor_3_Data = 0;

int new_Flame_Sensor_1_Data = 0;//Variable to read the current sensor val
int new_Flame_Sensor_2_Data = 0;
int new_Flame_Sensor_3_Data = 0;

int turnOffCount_1 = 0; //Loop count to stop
int turnOffCount_2 = 0;
int turnOffCount_3 = 0;

int flame_Sensor_Value_1 = 0;//Flame Sensor Values
int flame_Sensor_Value_2 = 0;
int flame_Sensor_Value_3 = 0;

int pot_Read_Value_1 = 0;        // value read from the pot
int pot_Read_Value_2 = 0;
int pot_Read_Value_3 = 0;

const int FLAME_START_VALUE = 410; //DAC Value if flame is just on {could vary}
const int FLAME_SATURATION_VALUE = 450;//DAC value of constant flame could change


byte int_pos[3]; //Position Variable array

int intChunk = 0;
//Stepper Initialisation
Stepper stepper(STEPS, MOTOR_DRIVER_IN_0, MOTOR_DRIVER_IN_1, MOTOR_DRIVER_IN_2, MOTOR_DRIVER_IN_3);
// the previous reading from the analog input
int previous = 0;
int commandNumber = 0 ;
int commandVal = 0;
Timer t ;

void setup() {
  // set the speed of the motor to 30 RPMs
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
  //Read all Sensor Values
  ReadAllSensorValues();

  //Detect the Slope and initiate cut down of Flame
  RTDSlopeDetector();

  //Send the Sensor Values to App
  
  //Read Bluetooth Data
  if (Serial.available()) {
    String cmd = Serial.readStringUntil(':');
    String val = Serial.readStringUntil(',');

    if (cmd != "" && val != "") {
      Serial.print("Command :" + cmd);
      Serial.println("cmdVal ," + val);
      commandNumber = cmd.toInt();
      commandVal = val.toInt();
      switch (commandNumber) {
        case 1: Serial.println("Knob One :" + val);
          prev_oneKnob = oneKnob;
          oneKnob = commandVal;
          diff_oneKnob = oneKnob - prev_oneKnob;
          rotateStepper(En_MD1 , diff_oneKnob);
          //                Serial.print("oneKnob_Pos : " + String(oneKnob));
          //                Serial.print( " Prev_Position :" + String(prev_oneKnob));
          //                Serial.println("Difference oneKnob : " + String(diff_oneKnob));
          break;

        case 2: Serial.println("Knob Two :" + val);
          prev_twoKnob = twoKnob;
          twoKnob = commandVal;
          diff_twoKnob = twoKnob - prev_twoKnob;
          //                Serial.print("twoKnob_Pos : " + String(twoKnob));
          //                Serial.print(" Prev_Position :" + String(prev_twoKnob));
          //                Serial.println("Difference twoKnob : " + String(diff_twoKnob));
          rotateStepper(En_MD2 , diff_twoKnob);
          break;

        case 3: Serial.println("Knob Three :" + val);
          prev_threeKnob = threeKnob ;
          threeKnob = commandVal;
          diff_threeKnob = threeKnob - prev_threeKnob;
          //                Serial.print("threeKnob_Pos : " + String(threeKnob));
          //                Serial.print("Prev_Position :" + String(prev_threeKnob));
          //                Serial.println("Difference threeKnob: " + String(diff_threeKnob));
          rotateStepper(En_MD3 , diff_threeKnob );

          break;

        case 6: Serial.println("Timer One :" + val);
          t.after(1000, turnOff);
          break;

        case 7: Serial.println("Timer Two :" + val);
          break;

        case 8: Serial.println("Timer Three :" + val);
          break;

        case 5: Serial.println("Turn Off Knob :" + val);
          switch (commandVal) {
            case 1: rotateStepper(En_MD1 , 180);
              break;
            case 2: rotateStepper(En_MD2 , 180);
              break;
            case 3: rotateStepper(En_MD3 , 180);
              break;
            default:
              break;
          }
          break;

        case 9: Serial.println("Proximity detector");

          //Set the Timer to turn off all the on knobs
          break;

        default:
          break;


      }
    }
  }
}

void rotateStepper(int Enable_Pin , int diff) {
  digitalWrite(SWITCHING_REGULATOR_EN , HIGH);
  digitalWrite(Enable_Pin, 1);
  stepper.step(diff);
  digitalWrite(Enable_Pin, 0);
  digitalWrite(SWITCHING_REGULATOR_EN , LOW);

}


void ReadAllSensorValues() {
  flame_Sensor_Value_1 = analogRead(RTD_1_ANALOG_A0);
  flame_Sensor_Value_2 = analogRead(RTD_2_ANALOG_A1);
  flame_Sensor_Value_3 = analogRead(RTD_3_ANALOG_A2);

  pot_Read_Value_1 = analogRead(ENC_0_ANALOG_A3);
  pot_Read_Value_2 = analogRead(ENC_1_ANALOG_A4);
  pot_Read_Value_3 = analogRead(ENC_2_ANALOG_A5);

}


void RTDSlopeDetector() {
  //assign the old data to new data
  prev_Flame_Sensor_1_Data = new_Flame_Sensor_1_Data;
  prev_Flame_Sensor_2_Data = new_Flame_Sensor_2_Data;
  prev_Flame_Sensor_2_Data = new_Flame_Sensor_3_Data;

  new_Flame_Sensor_1_Data = flame_Sensor_Value_1;
  new_Flame_Sensor_2_Data = flame_Sensor_Value_2;
  new_Flame_Sensor_3_Data = flame_Sensor_Value_3;

  if (new_Flame_Sensor_1_Data < prev_Flame_Sensor_1_Data && flame_Sensor_Value_1 > FLAME_SATURATION_VALUE) {
    turnOffCount_1 += 1;
    if (turnOffCount_1 >= 10) {
      //Turn off Knob 1
      rotateStepper(En_MD1 , 180);
      turnOffCount_1 = 0;
    }
  }
  if (new_Flame_Sensor_2_Data < prev_Flame_Sensor_2_Data && flame_Sensor_Value_2 > FLAME_SATURATION_VALUE) {
    turnOffCount_2 += 1;
    if (turnOffCount_2 >= 10) {
      //Turn off Knob 2
      rotateStepper(En_MD2 , 180);
      turnOffCount_2 = 0;
    }
  }
  if (new_Flame_Sensor_3_Data < prev_Flame_Sensor_3_Data && flame_Sensor_Value_3 > FLAME_SATURATION_VALUE) {
    turnOffCount_3 += 1;
    if (turnOffCount_3 >= 10) {
      //Turn off Knob 3
      rotateStepper(En_MD3 , 180);
      turnOffCount_3 = 0;
    }
  }
}

void turnOff(){
  Serial.println("Turn off the knob");
}



