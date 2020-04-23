
int latchPin = 5;
int clockPin = 4;
int dataPin = 3;

 int latchPinLevel = 9;
int clockPinLevel = 8;
int dataPinLevel = 7;

 byte data = 0x00;
 byte dataArrayLSB[10];
 byte dataArrayMSB[10];
 

          byte dataGREEN;
 byte dataArrayGREEN[10];
 
void setup() {
  //set pins to output so you can control the shift register
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
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

 dataArrayGREEN[9] = 0xFF; //11111111
      dataArrayGREEN[8] = 0x7F; //01111111
      dataArrayGREEN[7] = 0x3F; //00111111
      dataArrayGREEN[6] = 0x1F; //00011111
      dataArrayGREEN[5] = 0x0F; //00001111
      dataArrayGREEN[4] = 0x07; //00000111
      dataArrayGREEN[3] = 0x03; //00000011
      dataArrayGREEN[2] = 0x01; //00000001
      dataArrayGREEN[1] = 0x00; //00000000
      dataArrayGREEN[0] = 0x00; //00000111
}

void loop() {
  // count from 0 to 255 and display the number 
  // on the LEDs
  for (byte numberToDisplay = 0; numberToDisplay < 99; numberToDisplay++) {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
//    data = decToBcd(numberToDisplay);
    Serial.println(byte(data));
//    data= dataArrayMSB[2] | dataArrayLSB[3];
 data = decToBcd(numberToDisplay);
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    Serial.print(numberToDisplay);
    shiftOut(dataPin, clockPin, MSBFIRST,data);  
     
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    //move 'em out
    delay(100);
    digitalWrite(latchPinLevel,LOW);
    shiftOut(dataPinLevel, clockPinLevel,MSBFIRST, dataArrayGREEN[2]); 
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(latchPinLevel,HIGH);
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
  Serial.print("MSB-");
  Serial.print(MSB);
  Serial.print("LSB-");
  Serial.println(LSB);
  
  Serial.print("MSB BYTE-");
  Serial.print(dataArrayMSB[MSB]);
   Serial.print("LSB BYTE-");
  Serial.println(dataArrayLSB[LSB]);
  return (dataArrayMSB[MSB] | dataArrayLSB[LSB]);
}
