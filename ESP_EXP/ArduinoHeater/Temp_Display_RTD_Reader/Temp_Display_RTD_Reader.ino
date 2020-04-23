#include "Timer.h"


// You'll need to download this timer library from here
// http://www.doctormonk.com/search?q=timer 

// Define Variables
float V;
float temp;
float Rx;

// Variables to convert voltage to resistance
float C = 98.676;
float slope = 15.414;

// Variables to convert resistance to temp
float R0 = 100.0;
float alpha = 0.00385;

//Analog Input
#define  Vin  A0

Timer t; // Define Timer object


void setup() {
  Serial.begin(9600);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Analog IN Test");
  pinMode(Vin,INPUT); // Make Vin Input
  t.every(100,takeReading); // Take Reading Every 100ms

}

void loop() {
  // put your main code here, to run repeatedly:
 t.update(); // Update Timer
}

void takeReading(){
  // Bits to Voltage
  V = (analogRead(Vin)/4095.0)*3.3; // (bits/2^n-1)*Vmax 
  Serial.print("Sensor Value->");
  Serial.println(analogRead(Vin));
   Serial.print("Voltage Value->");
    Serial.println(V);
    
  // Voltage to resistance
  Rx = V*slope+C; //y=mx+c
   Serial.print("Rx Value->");
    Serial.println(Rx);
   
  // Resistance to Temperature
  temp= (Rx/R0-1.0)/alpha; // from Rx = R0(1+alpha*X)
  // Uncommect to convet celsius to fehrenheit
  // temp = temp*1.8+32; 
  Serial.println(temp);
}
