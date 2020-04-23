int sensorValue;
int angle;
#include <SoftwareSerial.h>
SoftwareSerial bluetoothSerial(10, 11); // RX, TX

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello BlueSpider");
  bluetoothSerial.begin(9600);
  bluetoothSerial.println("Chill");
}

void loop() {
//put your main code here, to run repeatedly:
//Android sends angle
  sensorValue = analogRead(A3);
  printAngleValues(sensorValue);
  delay(1000);
//Checks the feedback from the Potentiometer 
//And Reiterate the Control signal
//Check If the Temperature is not there but Pot is sensing values then rotate to turn off
//Unable to rotate warn the User to go and control manually



}
int prevData;
int newData;
String angleS;
void printAngleValues(int sensor){
  Serial.print("Sensor :");
  Serial.println(String(sensor));
  
  angle = map(sensor, 0, 1024, 0, 300);
  prevData = newData;
  newData = angle;
  if(prevData-newData != 0){
  Serial.print("Angle of Pot-->");
  Serial.println(String(angle));
  angleS=angle+";";
  bluetoothSerial.write(angle);
  bluetoothSerial.println("huh");
  }
}

