int WifiInput = 6; //PD5
int WifiOutput = 5; //PD6
//int RELAY = 13; //RELAY PD3
int RELAY = 3;

int Temperature_Constant = 65;
void setup() {
  // put your setup code here, to run once:
pinMode(WifiInput,INPUT);
pinMode(WifiOutput,OUTPUT);
pinMode(RELAY,OUTPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
//communicateWithWifi();

Serial.println(digitalRead(WifiInput));
if(Temperature_Constant
if(digitalRead(WifiInput) == 1){
   digitalWrite(RELAY,HIGH);
   digitalWrite(WifiOutput,HIGH);
}else {
  digitalWrite(RELAY,LOW);
  digitalWrite(WifiOutput,LOW);
}
delay(500);
}

void communicateWithWifi(){
  if(digitalRead(WifiInput) == HIGH){
  Serial.println("Input from Wifi On");
    digitalWrite(RELAY,HIGH);
    digitalWrite(WifiOutput,HIGH);
  }else if(digitalRead(WifiInput) == LOW){
    Serial.println("Input from Wifi On");
     digitalWrite(RELAY,LOW);
     digitalWrite(WifiOutput,LOW);
  }
 }

