/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

// the setup routine runs once when you press reset:
int sensorValue;
int angle;
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
//  sensorValue = analogRead(A0);
  // print out the value you read:
//  Serial.print("Sensor A0:");
//  Serial.println(sensorValue);
//  sensorValue = analogRead(A1);
  // print out the value you read:
//  Serial.print("Sensor A1:");
//  Serial.println(sensorValue);
//  sensorValue = analogRead(A2);
  // print out the value you read:
//  Serial.print("Sensor A2:");
//  Serial.println(sensorValue);
  sensorValue = analogRead(A3);
  // print out the value you read:
  Serial.print("Sensor A3:");
  Serial.println(sensorValue);
  angle = map(sensorValue, 0, 540, 0, 180);
  Serial.print("Angle of Pot-->");
  Serial.println(String(angle));
//  sensorValue = analogRead(A4);
  // print out the value you read:
//  Serial.print("Sensor A4:");
//  Serial.println(sensorValue);
//  sensorValue = analogRead(A5);
//   print out the value you read:
//  Serial.print("Sensor A5:");
//  Serial.println(sensorValue);
//  sensorValue = analogRead(A6);
  // print out the value you read:
//  Serial.print("Sensor A6:");
//  Serial.println(sensorValue);
//  sensorValue = analogRead(A7);
  // print out the value you read:
//  Serial.print("Sensor A7:");
//  Serial.println(sensorValue);
  delay(1000);        // delay in between reads for stability
}
