
int latchPin = 5;
int clockPin = 4;
int dataPin = 3;

int RELAY = 12;

 byte data = 0x00;
 byte dataArrayLSB[10];
 byte dataArrayMSB[10];
 
         
 
void setup() {
  //set pins to output so you can control the shift register
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(RELAY,OUTPUT);
//  digitalWrite(RELAY,HIGH);
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
  // count from 0 to 255 and display the number 
  // on the LEDs
  for (byte numberToDisplay = 0; numberToDisplay < 99; numberToDisplay++) {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
//    data = decToBcd(numberToDisplay);
    Serial.println(byte(data));
    data= dataArrayMSB[2] | dataArrayLSB[3];

// data = decToBcd(takeReading());
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    Serial.print(numberToDisplay);
    shiftOut(dataPin, clockPin, MSBFIRST,data);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(500);
  }
}

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
  return (dataArrayMSB[MSB] | dataArrayLSB[LSB]);
}

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
//  unsigned long currentMillis = millis();
// if (currentMillis - previousMillis >= OnTime)
// {
//  previousMillis = currentMillis;
  V = (analogRead(RTD_Sensor)/Bit_Size)*RTD_Voltage; // (bits/2^n-1)*Vmax 
  Serial.print("Sensor Value->");
  Serial.println(analogRead(RTD_Sensor));
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
  Serial.print("Temperatre :::");
  Serial.println(temp);
  if(temp > 65){
        Serial.println("Turn off the Relay");
        digitalWrite(RELAY,LOW);
    }else {
      digitalWrite(RELAY,HIGH);
    }

  return (int)temp;
  
 }
