
/**
   Reading the Data stream to find two delimiters

   Author : Neo G
   Last Modified:31 Dec 16
  CMD Format --> Command is Key<1,9> <:> Value <,>

  Key:Value,
  KNOB_ONE: 234 —> 1:12,
  KNOB_TWO:23 --> 2:23,
  KNOB_THREE:32-->3:32,

  OFF:Knob1--> 5:1,
  OFF:Knob2 —>  5:2,
  OFF:Knob1--> 5:3,

  Timerone: Secs:  6:123,
  TimerTwo:7:233,
  TimerThree:8:233,

  Query                 Value to App
  Feedback:Knob1-->9:1, Output --> Knob1:Pos:0
  Feedback:Knob2-->9:2, Output --> Knob2:Pos:180
  Feedback:Knob3-->9:3, Output --> Knob3:Pos:90
  
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

const int ENCODER_MIN = 512; //Calibrate these values to reflect the Actual Values
const int ENCODER_MAX = 1024;

int En_MD1 = 3; // PD5(9)
int En_MD2 = 4; // PD4(2)
int En_MD3 = 5; // PD3(1)

int SWITCHING_REGULATOR_EN = 10; //(PB2)

int MOTOR_DRIVER_IN_0 = 6; //PD6(10)
int MOTOR_DRIVER_IN_1 = 7; //PD7(11)
int MOTOR_DRIVER_IN_2 = 8; //PB0(12)
int MOTOR_DRIVER_IN_3 = 9; //PB1(13)


const int FLAME_START_VALUE = 410; //DAC Value if flame is just on {could vary}
const int FLAME_SATURATION_VALUE = 450;//DAC value of constant flame could change
const int TIME_OFF = 5;

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

class FlameSafe
{
    byte newData;
    byte prevData;
    int turnOffCount;
    int sensorValue;
    int id; // use motor enable pin to ID the class
    int flame_sensor_pin;
    long previousMillis ;
    long OnTime = 1000;
    int riseCount = 0;
  public:
    FlameSafe(int flame_sensor)
    {
      flame_sensor_pin = flame_sensor;

      if (flame_sensor == A0)
        id = En_MD1;
      else if (flame_sensor == A1)
        id = En_MD2;
      else if (flame_sensor == A2)
        id = En_MD3;

      newData = 0;
      prevData = 0;
      turnOffCount = 0;
      sensorValue = 0;

    }

    void sensorSloper()
    {
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= OnTime)
      {
        previousMillis = currentMillis;
        sensorValue = analogRead(flame_sensor_pin);

        // print the results to the serial monitor:

        prevData = newData;
        newData = sensorValue;
//        Serial.print("sensor " );
//        Serial.print(String(id) +" =");
//        Serial.println(sensorValue);
//        Serial.println("Flame on value" +String(FLAME_START_VALUE));
        //Loop to stop the spark or ignitor
        if (newData > prevData && sensorValue > FLAME_START_VALUE && sensorValue < (FLAME_START_VALUE + 100) ) {
//          Serial.print("sensor = " );
//          Serial.print(sensorValue);
//          Serial.println("!!!Flame On!!!! Temperature raising");
          //Turn off the Flame Ignitor
          turnOffCount = 0; //Initialise the turn off Counter



          //Stop the Spark
        } else if (newData < prevData &&  sensorValue > FLAME_SATURATION_VALUE && turnOffCount < 11) { //Loop to detect if the flame is falling continuosly and turn off the knob
          Serial.print("Inside sensor = " );
          Serial.print(sensorValue);
          if (turnOffCount == 10) {
            //Turn off Knob 1
            Serial.println("***Flame is Off Turn off the Knob***");
            turnOffKnob();
          }
          Serial.print("Count = " );
          Serial.print(turnOffCount);
          turnOffCount += 1; // Count to count for 10 loop cycle to turn off the knob
        }
      }
    }
    void turnOffKnob() {
      digitalWrite(SWITCHING_REGULATOR_EN , HIGH);
      digitalWrite(id, 1);
      stepper.step(180);
      digitalWrite(id, 0);
      digitalWrite(SWITCHING_REGULATOR_EN , LOW);

    }

};
/**
   Encoder Class to instantiate a Potentiometer encoder
   Which will read the Encoder Value and Send it back to the App
   TODO: Still has to map the Potentiometer value to the Position Map and send the data back in a Command Format
*/
class Encoder
{
    long int potentiometer_value;
    int pot_pin;

  public:
    Encoder(int encoder_pin)
    {
      potentiometer_value = 0;
      pot_pin = encoder_pin;
    }

    void encode()
    {
      potentiometer_value = analogRead(pot_pin);
      Serial.println("Encoder Value " + potentiometer_value);
    }

    boolean isOn() {
      if (potentiometer_value > ENCODER_MIN && potentiometer_value < ENCODER_MAX)
      {
        return true;
      }
      else {
        return false;
      }
    }
};
Knob knob1(En_MD1);
Knob knob2(En_MD2);
Knob knob3(En_MD3);

FlameSafe flameSafe1(RTD_1_ANALOG_A0);
FlameSafe flameSafe2(RTD_2_ANALOG_A1);
FlameSafe flameSafe3(RTD_3_ANALOG_A2);

Encoder encode1(ENC_0_ANALOG_A3);
Encoder encode2(ENC_1_ANALOG_A4);
Encoder encode3(ENC_2_ANALOG_A5);

int commandNumber = 0 ;
int commandVal = 0;

void setup() {
  // put your setup code here, to run once:
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
  //Reading and Taking care of the Flame Safety Sensors in every loop
  flameSafe1.sensorSloper();
  flameSafe2.sensorSloper();
  flameSafe3.sensorSloper();

  //Reading the Bluetooth Stream if the data is available in Serial in each iteration
  if (Serial.available()) {
    String cmd = Serial.readStringUntil(':'); //Reading the command
    String val = Serial.readStringUntil(','); //Reading the Value

    if (cmd != "" && val != "") {
      Serial.print("Command :" + cmd);
      Serial.println("cmdVal ," + val);
      commandNumber = cmd.toInt(); //Parsing the String values
      commandVal = val.toInt();
      switch (commandNumber) {
        case 1: Serial.println("Knob One :" + val);
          knob1.Rotate(commandVal); //Rotate the Knob1 accordingly
          break;

        case 2: Serial.println("Knob Two :" + val);
          knob2.Rotate(commandVal); //Rotate Knob2
          break;

        case 3: Serial.println("Knob Three :" + val);
          knob3.Rotate(commandVal); //Rotate Knob3
          break;

        case 6: Serial.println("Timer One :" + val);
          //Implement Hardware running timer instead of app generated timer
          //          t.after(1000, turnOff);
          break;

        case 7: Serial.println("Timer Two :" + val);
          break;

        case 8: Serial.println("Timer Three :" + val);
          break;

        case 5: Serial.println("Turn Off Knob :" + val);
          switch (commandVal)
          {
            case 1: turnOffKnob(En_MD1);
              break;
            case 2: turnOffKnob(En_MD2);
              break;
            case 3: turnOffKnob(En_MD3);
              break;
            default:
              break;
          }
          break;

        case 9: Serial.println("Proximity detector");
          //Implementation of Geo Fencing in the App using RSSI read values of the Bluetooth
          //Taking the Last known command to
          //Set the Timer to turn off all the on knobs
          delay(120000);
          if (encode1.isOn()) {
            turnOffKnob(En_MD1);
          }
          if (encode2.isOn()) {
            turnOffKnob(En_MD2);
          }
          if (encode3.isOn()) {
            turnOffKnob(En_MD3);
          }

          break;

        default:
          break;


      }
    }
  }
}


void turnOffKnob(int Enable_Pin) {
  digitalWrite(SWITCHING_REGULATOR_EN , HIGH);
  digitalWrite(Enable_Pin, 1);
  stepper.step(180);
  digitalWrite(Enable_Pin, 0);
  digitalWrite(SWITCHING_REGULATOR_EN , LOW);

}
