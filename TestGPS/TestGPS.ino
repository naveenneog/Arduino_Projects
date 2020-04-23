int ledPin = 13; // LED connected to digital pin 13
int inPin = 0;   // pushbutton connected to digital pin 7
String val = ' ';     // variable to store the read value

void setup()
{
  pinMode(ledPin, OUTPUT);      // sets the digital pin 13 as output
  pinMode(inPin, INPUT);  
  Serial.begin(9600);// sets the digital pin 7 as input
}

void loop()
{
  
  val =  &Serial1
  Serial.print(val);
 
  // read the input pin
  digitalWrite(ledPin, val);    // sets the LED to the button's value
}
