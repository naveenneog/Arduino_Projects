
int latchPin = 4; // -> 2 //PD4 //Pin connected to (12)(R_CLK) ST_CP of 74HC595
int clockPin = 9; // -> 3 // PB1 Pin Connected to (11) SH_CP (SR_CLK) of 74HC595
int dataPin = 8; //  -> 1 //14 DS (SER) (PB0) - DIgital Pin 8

 byte data = 0x00;
 byte dataArrayLSB[10];
 byte dataArrayMSB[10];
 
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

}

void loop() {
  // count from 0 to 255 and display the number 
  // on the LEDs
  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
//    numberToDisplay = 32;
        data= dataArrayMSB[3] | dataArrayLSB[2];
    Serial.println(numberToDisplay);
    shiftOut(dataPin, clockPin, MSBFIRST, data);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(500);
  }
}

