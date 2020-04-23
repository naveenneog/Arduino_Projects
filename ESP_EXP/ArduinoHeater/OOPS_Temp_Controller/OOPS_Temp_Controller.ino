/**
 * 
 */

int TempTemp = 0;
// millis and Sensor Read time

//Display Shift Register Pins
int latchPinDisplay = 5;
int clockPinDisplay = 4;
int dataPinDisplay = 3;

//Level Indicator Shift Register Pins

int latchPinLevel = 8;
int clockPinLevel = 7;
int dataPinLevel = 6;

// Variables to convert voltage to resistance
float C = 98.676;
float slope = 15.414;

//Voltage and Bitmax specifier
float RTD_Voltage = 5.0;
float Bit_Size = 1024.0;

// Variables to convert resistance to temp
float R0 = 100.0;
float alpha = 0.00385;

//Analog Input
int Vin = A0;//ANALOG_PIN_0 // GPIO36
const int CUTOFF_TEMP = 65;

//HEATER Constants
int HEATER_TEMP = 65;
int RELAY = 10;
class Temperature
{
  // Define Variables
float V;
float temp;
float Rx;
int RTD_Sensor;
boolean degree ;

long previousMillis ;
 long OnTime = 100;
public: 
 Temperature(int tSensor , boolean degree_Fareinhiet)
 {
  RTD_Sensor = tSensor;
  degree = degree_Fareinhiet;
 }
 /**
  * This method takes the reading and returns the Temperature
  */
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
  Serial.println(temp);
  if(temp > CUTOFF_TEMP){
        Serial.println("Turn off the Relay");
    }

  return (int)temp;
  
 }
};

class Display
{
    
 byte data = 0x00;
 byte dataArrayLSB[10];
 byte dataArrayMSB[10];
 
 byte dataGREEN;
 byte dataArrayGREEN[10];
 
 int latchPin;
 int clockPin;
 int dataPin;
 
 
  /**
   * Used Switching Circuit for the Display
   */
  public: 
  Display( int latchPin, int clockPin, int dataPin)
  {
//    fancyLoading();
    latchPin = latchPin;
    clockPin = clockPin;
    dataPin = dataPin;
    
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
  
  void WriteTwoDigitNumber(int number){
    //Writing zero for negative values
    if(number< 0){
      number = 0;
    }
    Serial.print("Writing Temperature->");
    Serial.println(number);
    data = decToBcd(number);
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    Serial.print(number);
    Serial.println(data);
    shiftOut(dataPin, clockPin, MSBFIRST,data);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    delay(100);
  }

  byte decToBcd(int val)
{
  int MSB ;
  int LSB;
  MSB = val/10;
  LSB = val%10;
  Serial.print("MSB->");
  Serial.print(MSB);
   Serial.print("LSB->");
  Serial.println(LSB);
  Serial.print("MSB BYTE-");
  Serial.print(dataArrayMSB[MSB]);
   Serial.print("LSB BYTE-");
  Serial.println(dataArrayLSB[LSB]);
  /**
   * Writing to LEvel indicator MSB value of the Temperature
   */
   dataGREEN = dataArrayGREEN[MSB-1]; //Write one value less
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(latchPinLevel, 0);
    //move 'em out
    shiftOut(dataPinLevel, clockPinLevel,MSBFIRST, dataGREEN);   
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(latchPinLevel, 1);
    //////////////////////////////////////////////
  return (dataArrayMSB[MSB] | dataArrayLSB[LSB]);
}
};


Temperature temperature1(A0,true);
Display disp7Seg(latchPinDisplay,clockPinDisplay,dataPinDisplay);
void setup() {
  // put your setup code here, to run once:
 // initialize digital pin LED_BUILTIN as an output.
 Serial.begin(9600);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Analog IN Test");
  pinMode(latchPinDisplay,OUTPUT);
  pinMode(latchPinLevel,OUTPUT);
  pinMode(clockPinDisplay,OUTPUT);
  pinMode(clockPinLevel,OUTPUT);
  pinMode(dataPinDisplay,OUTPUT);
  pinMode(dataPinLevel,OUTPUT);
  pinMode(A0,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
       TempTemp =  (int)temperature1.takeReading();
//      disp7Seg.WriteTwoDigitNumber(TempTemp);
  if(TempTemp < 100) {
    Serial.print("Read Temperature");
    Serial.println(TempTemp);
    disp7Seg.WriteTwoDigitNumber(TempTemp);
    delay(10); 
    if(TempTemp < HEATER_TEMP){
      digitalWrite(RELAY,HIGH);
    }else{
      digitalWrite(RELAY,LOW);
    }
     
  }
  delay(1000);
}
