
////Pin connected to ST_CP(12) of 74HC595
//int latchPin = 5;
////Pin connected to SH_CP(11) of 74HC595
//int clockPin = 4;
//////Pin connected to DS(14) of 74HC595
//int dataPin = 3;

//Pin connected to ST_CP(12) of 74HC595
int latchPinLevel = 9;
//Pin connected to SH_CP(11) of 74HC595
int clockPinLevel = 8;
////Pin connected to DS(14) of 74HC595
int dataPinLevel = 7;
// Pin connected to MS of 74HC595
int masterResetPin = 2;
byte data = 0x11111111;
byte dataGREEN;
byte dataArrayGREEN[10];
void setup() {
  // put your setup code here, to run once:
//set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(masterResetPin, OUTPUT);
  digitalWrite(masterResetPin, HIGH);
   //Arduino doesn't seem to have a way to write binary straight into the code 
  //so these values are in HEX.  Decimal would have been fine, too. 
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

  int valIn = 23;
  int MSB = valIn/10;
   for (int j = 0; j <= MSB; j++) {
    //load the light sequence you want from array
  
    dataGREEN = dataArrayGREEN[9];
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(latchPinLevel, 0);
    //move 'em out
    shiftOut(dataPinLevel, clockPinLevel,MSBFIRST, dataGREEN);   
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(latchPinLevel, 1);
    delay(300);
  }
  delay(2000);
  for(int i=0; i<10 ;i++){
    dataGREEN = dataArrayGREEN[0];
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(latchPinLevel, 0);
    //move 'em out
    shiftOut(dataPinLevel, clockPinLevel,MSBFIRST, dataGREEN);   
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(latchPinLevel, 1);
    delay(10);
  }
  
}
