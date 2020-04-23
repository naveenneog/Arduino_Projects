/*********
  Neo-G
  Complete project details at http://randomnerdtutorials.com  
*********/

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager


//static const uint8_t D0   = 16;
//static const uint8_t D1   = 5;
//static const uint8_t D2   = 4;
//static const uint8_t D3   = 0;
//static const uint8_t D4   = 2;
//static const uint8_t D5   = 14;
//static const uint8_t D6   = 12;
//static const uint8_t D7   = 13;
//static const uint8_t D8   = 15;
//static const uint8_t D9   = 3;
//static const uint8_t D10 = 1;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
//String output5State = "off";
//String output4State = "off";
String output1State = "on";

// Assign output variables to GPIO pins
const int output5 = 5;
const int output4 = 4;
const int input0 = 16;
const int output1 = 5;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(output5, OUTPUT);
  pinMode(output4, OUTPUT);
  pinMode(input0,INPUT);
  pinMode(output1,OUTPUT);
  // Set outputs to LOW
  digitalWrite(output5, LOW);
  digitalWrite(output4, LOW);
  digitalWrite(output1,HIGH);

  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  
  // Uncomment and run it once, if you want to erase all the stored information
//  wifiManager.resetSettings();
  
  // set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("Flameye_AP");
  // or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();
  
  // if you get here you have connected to the WiFi
  Serial.println("Connected.");
  Serial.println("IP Address");
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if(header.indexOf("GET /1/on") >=0){
              Serial.println("GPIO 1 on");
              output1State = "on";
              digitalWrite(output1,HIGH);
            }else if(header.indexOf("GET /1/off") >= 0) {
              Serial.println("GPIO 1 off");
              output1State = "off";
              digitalWrite(output1, LOW);
            }
            
//            else if (header.indexOf("GET /5/on") >= 0) {
//              Serial.println("GPIO 5 on");
//              output5State = "on";
//              digitalWrite(output5, HIGH);
//            } else if (header.indexOf("GET /5/off") >= 0) {
//              Serial.println("GPIO 5 off");
//              output5State = "off";
//              digitalWrite(output5, LOW);
//            } else if (header.indexOf("GET /4/on") >= 0) {
//              Serial.println("GPIO 4 on");
//              output4State = "on";
//              digitalWrite(output4, HIGH);
//            } else if (header.indexOf("GET /4/off") >= 0) {
//              Serial.println("GPIO 4 off");
//              output4State = "off";
//              digitalWrite(output4, LOW);
//            }

            delay(100);
            if(digitalRead(input0) == LOW){
              output1State = "off";
            }else if(digitalRead(input0) == HIGH){
              output1State = "on";
            }
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Nipco Smart Water Heater</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>HEATER - State " + output1State + "</p>");
            // If the output5State is off, it displays the ON button       
            if (output1State=="off") {
              client.println("<p><a href=\"/1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/1/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            
//            // Display current state, and ON/OFF buttons for GPIO 5  
//            client.println("<p>GPIO 5 - State " + output5State + "</p>");
//            // If the output5State is off, it displays the ON button       
//            if (output5State=="off") {
//              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
//            } 
//               
//            // Display current state, and ON/OFF buttons for GPIO 4  
//            client.println("<p>GPIO 4 - State " + output4State + "</p>");
//            // If the output4State is off, it displays the ON button       
//            if (output4State=="off") {
//              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
//            } else {
//              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
//            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

