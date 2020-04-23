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

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
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
}

// the loop function runs over and over again forever
void loop() {
digitalWrite(Disp1En,HIGH);
DisplayNumber(3);
delay(10); 
digitalWrite(Disp1En,LOW);

digitalWrite(Disp2En,HIGH);
DisplayNumber(4);
delay(10); 
digitalWrite(Disp2En,LOW);
  
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

//  blinkDot();
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











