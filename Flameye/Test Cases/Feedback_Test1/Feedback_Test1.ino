#include <SoftwareSerial.h>

SoftwareSerial bluetoothSerial(10, 11); // RX, TX
int prevData;
int newData;
int sensorValue;
int angle;

void setup() {
  Serial.println("Hello BlueSpider");
  bluetoothSerial.begin(9600);
  bluetoothSerial.println("Its a beginning");
}
int commandNumber = 0 ;
int commandVal = 0;
int comNumberMan = 0;
int comValMan = 0;
  
void loop() {
  //Reading the Bluetooth Stream if the data is available in Serial in each iteration
  if (bluetoothSerial.available()) {
    String cmd = bluetoothSerial.readStringUntil(':'); //Reading the command
    String val = bluetoothSerial.readStringUntil(','); //Reading the Value

    if (cmd != "" && val != "") {
      Serial.print("Command :" + cmd);
      Serial.println("cmdVal ," + val);
      commandNumber = cmd.toInt(); //Parsing the String values
      commandVal = val.toInt();
      switch (commandNumber) {
        case 1: Serial.println("Knob One :" + val);
          break;

        case 2: Serial.println("Knob Two :" + val);
          break;

        case 3: Serial.println("Knob Three :" + val);
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
       
          break;

        case 9: Serial.println("Proximity detector");
          //Implementation of Geo Fencing in the App using RSSI read values of the Bluetooth
          //Taking the Last known command to
          //Set the Timer to turn off all the on knobs
        

          break;

        default:
          break;


      }
//Read the sensor values and write it back to the app
  sensorValue = analogRead(A3);
  printAngleValues(sensorValue);
  delay(500);
}
  }
}
  
void printAngleValues(int sensor){
  angle = map(sensor, 0, 1024, 0, 300);
  prevData = newData;
  newData = angle;
  if(prevData-newData != 0){
  Serial.print("Angle of Pot-->");
  Serial.println(String(angle));
  bluetoothSerial.write(angle);
  }
}

