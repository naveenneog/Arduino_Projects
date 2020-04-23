#include <Event.h>
#include <Timer.h>


/**
 * Reading the Data stream to find two delimiters
 * 
 * Author : Neo G
 * Last Modified:29 Dec 16
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
String KnobOne = "KONE"; 1:12
String KnobTwo = "KTWO";
String KnobThree = "KTRI";
String TimerOne = "TION";
String TimerTwo = "TITW";
String TimerThree = "TITR";
String TurnOffKnob = "OFF";


*/
Timer t ;
int commandNumber = 0;
int commandValue = 0;
int countComm = 0;
int timeid =0;
int rotator_angle = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello , Spider Edge");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
//    for (int i = 0; i < 3; i++) {

      String cmd = Serial.readStringUntil(':');
      String val = Serial.readStringUntil(',');
//      countComm +=1;
//      if(countComm >=10){
//        Serial.println("1:150,");
//        countComm = 0;
//      }
      if (cmd != "" && val != "") {
        Serial.print("Command :" + cmd);
        Serial.println("cmdVal ," + val);
        commandNumber = cmd.toInt();
        commandValue = val.toInt();
        switch (commandNumber) {
          case 1: Serial.println("Knob One :" + val);
              rotator_angle = map(commandValue, 0, 150, 0, 90);
              rotateStepper(rotator_angle);
            break;

          case 2: Serial.println("Knob Two :" + val);
            break;

          case 3: Serial.println("Knob Three :" + val);
            break;

          case 6: Serial.println("Timer One :" + val);
              t.after(1000,turnOff);
            break;

          case 7: Serial.println("Timer Two :" + val);
            break;

          case 8: Serial.println("Timer Three :" + val);
            break;

          case 5: Serial.println("Turn Off Knob :" + val);
            break;

          default:
            break;


        }
      }
    }
  }

 void turnOff() {
    Serial.println("::::::::: Knob is off ::::::::::");
  }

