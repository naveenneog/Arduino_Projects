/**
 * 
 * JUST Temperature Controller
 * 
 * By Flameye Technologies
 * @Author: Naveen G
 */

int latchPin = 4; //PD4 //Pin connected to (12)(R_CLK) ST_CP of 74HC595
int clockPin = 9; //PB1 Pin Connected to (11) SH_CP (SR_CLK) of 74HC595
int dataPin = 8; //14 DS (SER) (PB0) - DIgital Pin 8

const int TEMPERATURE = 65; //HEATER TEMPERATURE FIXED 

int WifiInput = 6; //PD5
int WifiOutput = 5; //PD6
int RELAY = 3; //RELAY PD3

int prevTemp ;
int currTemp ;
int tempDiff ;

 byte data = 0x00;
 byte dataArrayLSB[10];
 byte dataArrayMSB[10];

void setup() {
 Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(RELAY,OUTPUT);
  pinMode(WifiInput, INPUT);
  pinMode(WifiOutput,OUTPUT);

//By default turn on the heater
 digitalWrite(RELAY,HIGH);
 digitalWrite(WifiOutput,HIGH);
 dataArrayLSB[9] = 0x09;
 dataArrayLSB[8] = 0x08;
 dataArrayLSB[7] = 0x07;
 dataArrayLSB[6] = 0x06;
 dataArrayLSB[5] = 0x05;
 dataArrayLSB[4] = 0x04;
 dataArrayLSB[3] = 0x03;
 dataArrayLSB[2] = 0x02;
 dataArrayLSB[1] = 0x01;
 dataArrayLSB[0] = 0x00;
 
 dataArrayMSB[9] = 0x90; //0x10010000;
 dataArrayMSB[8] = 0x80;
 dataArrayMSB[7] = 0x70;
 dataArrayMSB[6] = 0x60;
 dataArrayMSB[5] = 0x50;
 dataArrayMSB[4] = 0x40;
 dataArrayMSB[3] = 0x30;
 dataArrayMSB[2] = 0x20;
 dataArrayMSB[1] = 0x10;
 dataArrayMSB[0] = 0x00;
}

void loop() {
 prevTemp = currTemp;
 currTemp = takeReading();
 tempDiff = currTemp - prevTemp;
 tempDiff = abs(tempDiff);
 if(tempDiff >= 1){
    display7Segment(currTemp);
    
 }
 if(currTemp > TEMPERATURE){
   turnHeater(LOW);
 }else{
   turnHeater(HIGH);
 }
 
}

//Turns the heater on / off
void turnHeater(boolean turnOn){
  
    digitalWrite(RELAY,turnOn);
    digitalWrite(WifiOutput,turnOn);
}
void display7Segment(int currTemp){
    data = decToBcd(currTemp);
    digitalWrite(latchPin, LOW);
    Serial.println(data);
    shiftOut(dataPin, clockPin, MSBFIRST,data);  
    digitalWrite(latchPin, HIGH);
    delay(100);// pause before next value:
}
////////////DISPLAY///////////////
byte decToBcd(int val)
{
  int MSB ;
  int LSB;
  MSB = val/10;
  LSB = val%10;
//  Serial.print("MSB-");
//  Serial.print(MSB);
//  Serial.print("LSB-");
//  Serial.println(LSB);
  
//  Serial.print("MSB BYTE-");
//  Serial.print(dataArrayMSB[MSB]);
//   Serial.print("LSB BYTE-");
//  Serial.println(dataArrayLSB[LSB]);
  //As the Display board is reversed this is causing the problem
  return (dataArrayLSB[MSB] | dataArrayMSB[LSB]);
}

//////////Reading from RTD Sensor ///////////
float V;
float temp;
float Rx;
int RTD_Sensor =A0;
// Variables to convert voltage to resistance
float C = 98.676;
float slope = 15.414;

//Voltage and Bitmax specifier
float RTD_Voltage = 5.0;
float Bit_Size = 1024.0;
// Variables to convert resistance to temp
float R0 = 100.0;
float alpha = 0.00385;
int takeReading(){
//   Bits to Voltage
  V = (analogRead(RTD_Sensor)/Bit_Size)*RTD_Voltage; // (bits/2^n-1)*Vmax 
//  Serial.print("Sensor Value->");
//  Serial.println(analogRead(RTD_Sensor));
//   Serial.print("Voltage Value->");
//    Serial.println(V);
    
  // Voltage to resistance
  Rx = V*slope+C; //y=mx+c
//   Serial.print("Rx Value->");
//    Serial.println(Rx);
   
  // Resistance to Temperature
  temp= (Rx/R0-1.0)/alpha; // from Rx = R0(1+alpha*X)
  // Uncommect to convet celsius to fehrenheit
  // temp = temp*1.8+32; 
  Serial.print("Temperatre :::");
  Serial.println(temp);
  

  return (int)temp;
  
 }
