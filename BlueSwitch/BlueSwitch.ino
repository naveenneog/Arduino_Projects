/*
  Switch statement

 Demonstrates the use of a switch statement.  The switch
 statement allows you to choose from among a set of discrete values
 of a variable.  It's like a series of if statements.

 To see this sketch in action, but the board and sensor in a well-lit
 room, open the serial monitor, and and move your hand gradually
 down over the sensor.

 The circuit:
 * photoresistor from analog in 0 to +5V
 * 10K resistor from analog in 0 to ground

 created 1 Jul 2009
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/SwitchCase
 */

// these constants won't change. They are the
// lowest and highest readings you get from your sensor:
const int sensorMin = 0;      // sensor minimum, discovered through experiment
const int sensorMax = 600;    // sensor maximum, discovered through experiment

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {

if (bleSerial.available()) {
    for (int i = 0; i < 3; i++) {
      String chunk = bleSerial.readStringUntil(',');
      if (chunk != "") {
        int_pos[i] = chunk.toInt();
      }
    }
    bleSerial.println("wait buddy !!!");
    
     for (int i = 0; i < 3; i++) {
      if (i == 0) {
          prev_red = red;
          red = int_pos[i];
          diff_red = red - prev_red;
          rotateStepper(EN_PIN_A, diff_red);
        }
        else if (i == 1) {
          prev_green = green;
          green = int_pos[i];
          diff_green = green - prev_green;
          rotateStepper(EN_PIN_B, diff_green);
        }
        else if(i == 2) {
           prev_blue = blue;
          blue = int_pos[i];
          diff_blue = blue - prev_blue;
          rotateStepper(EN_PIN_C, diff_blue);

        }
     }



  // read the sensor:
  int sensorReading = analogRead(A0);
  // map the sensor range to a range of four options:
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);

  // do something different depending on the
  // range value:
  switch (range) {
    case 0:    // your hand is on the sensor
      Serial.println("dark");
      break;
      
    case 1:    // your hand is close to the sensor
      Serial.println("dim");
      break;
    case 2:    // your hand is a few inches from the sensor
      Serial.println("medium");
      break;
    case 3:    // your hand is nowhere near the sensor
      Serial.println("bright");
      break;
  }
  delay(1);        // delay in between reads for stability
}





