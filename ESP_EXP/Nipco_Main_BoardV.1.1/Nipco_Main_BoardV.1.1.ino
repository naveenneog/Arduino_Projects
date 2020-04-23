int latchPin = 4; //PD4 //Pin connected to (12)(R_CLK) ST_CP of 74HC595
int clockPin = 9; //PB1 Pin Connected to (11) SH_CP (SR_CLK) of 74HC595
int dataPin = 8; //14 DS (SER) (PB0) - DIgital Pin 8


int WifiInput = 6; //PD5
int WifiOutput = 5; //PD6
int RELAY = 3; //RELAY PD3

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
int WifiStateChanged = 0;
int prevValue = 1;
int currentValue = 1;;
void loop() {
  prevValue = currentValue;
  currentValue = digitalRead(WifiInput);
if(currentValue != prevValue){
      WIFI(currentValue);
      WifiStateChanged = 1;
}
 delay(100);
 data = decToBcd(takeReading());
    digitalWrite(latchPin, LOW);
    Serial.println(data);
    shiftOut(dataPin, clockPin, MSBFIRST,data);  
    digitalWrite(latchPin, HIGH);
    delay(1000);// pause before next value:
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

void WIFI(int currVal){
//////////////FOR WIFI/////////////////
Serial.println(digitalRead(WifiInput));
if(currVal == 0){
  turnRelay(LOW);
   delay(50);
}else if(currVal == 1){
  turnRelay(HIGH);
  delay(50);
}
////////////////////////////////////////
}
////////////Reading from RTD Sensor ///////////
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
  if(temp > 65){
        Serial.println("Turn off the Relay");
       turnRelay(LOW);
    }// Comment out the digital Read if it is not a Wifi device
    else 
    if(temp <= 65){
       turnRelay(HIGH);
       if(WifiStateChanged >0 ){
         if(digitalRead(WifiInput) == HIGH) {
             turnRelay(HIGH);
            Serial.println("Turn on the Relay");
         }else{
            turnRelay(LOW);
         }
       }
    }
   

  return (int)temp;
  
 }

void turnRelay(boolean on){
   digitalWrite(RELAY,on);
   digitalWrite(WifiOutput,on);
 }

