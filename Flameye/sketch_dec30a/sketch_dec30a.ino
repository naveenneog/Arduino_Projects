#include <Event.h>
#include <Timer.h>
Timer t ;
void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
  Serial.println("Hello , Spider Edge");
}

void loop() {
  // put your main code here, to run repeatedly:
//Serial.println("Timer One :");
String incoming = Serial.readString();
Serial.println(incoming);
int intVal = incoming.toInt();
if(intVal == 143){
  Serial.println("Timer start");
             int value= t.after(10,turnOff);
}
else if(intVal == 224){
  int value = t.every(2000,doSomething);
}
}

void turnOff() {
    Serial.println("::::::::: Knob is off ::::::::::");
}

void doSomething(){
  Serial.println("Something");
}

