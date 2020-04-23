
const int buzzer = 13; //buzzer to arduino pin 2
const int LED = 16;

void setup(){
 
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 2 as an output

}

void loop(){
// Make an awesome sound
//Try to play words and music
  digitalWrite(LED_BUILTIN,1);
  tone(buzzer, 1000); // Send 1KHz sound signal...
  digitalWrite(LED_BUILTIN,0);
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  delay(900);        // ...for 1sec
  tone(buzzer, 2000);
   delay(800); 
   tone(buzzer, 5000);
   delay(600); 
   tone(buzzer, 8000);
   delay(500); 
   tone(buzzer, 10000);
    delay(1000); 
   tone(buzzer, 20000);
  
  
}
