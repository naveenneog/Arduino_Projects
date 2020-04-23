/**
 * 
 */
//Define Segments for LED
int LED_BUILTIN = 2;
int SEG_A = 15;
int SEG_B = 4;
int SEG_C = 16;
int SEG_D = 17;
int SEG_E = 5;
int SEG_F = 18;
int SEG_G = 19;
int DOT = 21;
int Disp1En = 22;
int Disp2En = 23;

int TempTemp = 0;
// millis and Sensor Read time

// Variables to convert voltage to resistance
float C = 98.676;
float slope = 15.414;

// Variables to convert resistance to temp
float R0 = 100.0;
float alpha = 0.00385;

//Analog Input
int Vin = 36;//ANALOG_PIN_0 // GPIO36
const int CUTOFF_TEMP = 65;

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
 int takeReading(){
  // Bits to Voltage
  unsigned long currentMillis = millis();
 if (currentMillis - previousMillis >= OnTime)
 {
  previousMillis = currentMillis;
  V = (analogRead(RTD_Sensor)/4095.0)*3.3; // (bits/2^n-1)*Vmax 
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
 }
};

class Display
{
    long prevMillis ;
    long OnTime = 10;
    int EN1 ;
    int EN2;
    int LSB;
    int MSB;
  /**
   * Used Switching Circuit for the Display
   */
  public: 
  Display(int initialValues, int EN1, int EN2)
  {
//    fancyLoading();
    EN1 = EN1;
    EN2 = EN2;
   
  }
  
  void WriteTwoDigitNumber(int number){
    Serial.print("Writing Temperature->");
    Serial.println(number);
    MSB = number/10;
    LSB = number%10;
    digitalWrite(EN1,HIGH);
    DisplayNumber(MSB);
    delay(10); 
    digitalWrite(EN1,LOW);
    
    digitalWrite(EN2,HIGH);
    DisplayNumber(LSB);
    delay(10); 
    digitalWrite(EN2,LOW);
        
  }

  void DisplayNumber(int number){
  
  switch(number){
    case 0: Zero();
    break;
    case 1: One();
    break;
    case 2: Two();
    break;
    case 3: Three();
    break;
    case 4: Four();
    break;
    case 5: Five();
    break;
    case 6: Six();
    break;
    case 7: Seven();
    break;
    case 8: Eight();
    break;
    case 9: Nine();
    break;
    default: blinkDot();
    break;
  }
  }
void Zero() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, HIGH);
  
}

void One() {
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
  
}

void Two() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, LOW);
  
}

void Three() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, LOW);
  
}

void Four() {
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
  
}
void Five() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
  
}

void Six() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
  
}

void Seven() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, HIGH);
  
}

void Eight() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
  
}

void Nine() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, HIGH);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
  
}

void alphaA() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
  
}
void alphaB() {
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
  
}
void alphaC() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, HIGH);
  
}
void alphaD() {
  digitalWrite(SEG_A, HIGH);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, HIGH);
  digitalWrite(SEG_G, LOW);
  
}
void alphaE() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
  
}

void alphaF() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, HIGH);
  digitalWrite(SEG_C, HIGH);
  digitalWrite(SEG_D, HIGH);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
  
}

void blinkDot(){
  delay(500);
  digitalWrite(DOT,LOW);
  delay(500);
  digitalWrite(DOT,HIGH);
}
  

};

Temperature temperature1(Vin,true);
Display disp7Seg(0,Disp1En,Disp2En);
void setup() {
  // put your setup code here, to run once:
 // initialize digital pin LED_BUILTIN as an output.
 Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Analog IN Test");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  pinMode(DOT,OUTPUT);
  pinMode(Disp1En,OUTPUT);
  pinMode(Disp2En,OUTPUT);
  pinMode(Vin,INPUT);
}
int MSB;
int LSB;
void loop() {
  // put your main code here, to run repeatedly:
  
       TempTemp =  (int)temperature1.takeReading();
//      disp7Seg.WriteTwoDigitNumber(TempTemp);
  if(TempTemp < 100) {
   MSB = (int)TempTemp/10;
    LSB = TempTemp%10;
    Serial.print("Bits Value->");
    Serial.println(TempTemp);
    Serial.println("Bits Value->");
    Serial.println(MSB);
    Serial.println(LSB);
    digitalWrite(Disp1En,HIGH);
    disp7Seg.DisplayNumber(MSB);
    delay(10); 
    digitalWrite(Disp1En,LOW);
    
    digitalWrite(Disp2En,HIGH);
    disp7Seg.DisplayNumber(LSB);
    delay(10); 
    digitalWrite(Disp2En,LOW);
  }
}
