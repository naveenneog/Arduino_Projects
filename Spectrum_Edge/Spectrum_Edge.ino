/**
 * Spectrum edge 
 * 
 * Driving 3 Stepper Motor from 4-16 Demux Controlled from 
 * Demux A-B : Pin 5,6 (Select Pins)
 * Stepper : Pin 8,9,10,11
 * Bletooth : Pin 3 (Rx),4 (Tx) 
 * 
 * Author : Neo-G
 */

#include<Stepper.h>
#include <SoftwareSerial.h>
// change this to the number of steps on your motor
#define STEPS 100

int EN_PIN = 12;
int A = 5;
int B = 6;
Stepper stepper(STEPS, 8, 9, 10, 11);
SoftwareSerial bleSerial(3, 4); // RX, TX
byte red = 0;
byte green = 0;
byte blue = 0;
byte prev_red = 0;
byte prev_green = 0;
byte prev_blue = 0;
int diff_red = 0;
int diff_green = 0;
int diff_blue = 0;
byte int_pos[3];

void setup() {
  //Setup Stepper speed 10rpm
   stepper.setSpeed(10);
   pinMode(EN_PIN,OUTPUT);
   pinMode(A,OUTPUT);
   pinMode(B,OUTPUT);
   Serial.begin(9600);
   bleSerial.begin(9600);//Setup BLE
   bleSerial.println("Hello, Spider Edge");
   
}

void loop() {
  //Start the Ble loop
if(bleSerial.available()){
      for(int i=0;i<3;i++){
       String chunk = bleSerial.readStringUntil(',');
        if(chunk != "") {
          int_pos[i] = chunk.toInt();
            if(i==0){
                 prev_red = red;
                 red = int_pos[i];
                 diff_red = red - prev_red;
                 digitalWrite(A,0);
                 digitalWrite(B,0);
                 stepMotor(diff_red);
                 Serial.println("Difference Value:red: "+String(diff_red));
            }
            else if(i==1)
            {
               prev_green = green;
               green = int_pos[i];
               diff_green = green - prev_green;
              // digitalWrite(A,1);
              // digitalWrite(B,0);
              // stepMotor(diff_green);
            Serial.println("Difference Value:green: "+String(diff_green));
            }
            else if(i==2)
            {
               prev_blue = blue;
               blue = int_pos[i];
               diff_blue = blue - prev_blue;
             //  digitalWrite(A,0);
             //  digitalWrite(B,0);
              // stepMotor(diff_blue);
               Serial.println("Difference Value:blue: "+String(diff_blue));
            }
             Serial.println("Pos"+String(int_pos[i]));
         }
     }
            
}

}

void stepMotor(int diff)
{
if(diff>0){
        bleSerial.println("flame++");
      }else if(diff <0){
        bleSerial.println("flame--Edge");
      }
       digitalWrite(EN_PIN,1);
       Serial.println("Stepping motor :" +diff);
       stepper.step(diff);
       digitalWrite(EN_PIN,0);
       delay(10);
}

