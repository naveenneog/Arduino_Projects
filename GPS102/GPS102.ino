#include<stdlib.h>
#include <Adafruit_GPS.h>      // Using s modified version of the Adafruit libraries
Adafruit_GPS GPS(&Serial2);    // Can't use SoftwareSerial

// Global Variables
int value = 0;
unsigned int ReportingInterval = 20000;  // How often do you want to send to Ubidots (in millis)
unsigned long LastReport = 0;            // Keep track of when we last sent data
char c;                                  // Used to relay input from the GPS serial feed
String Location = "";                    // Will build the Location string here


void setup() {
  // put your setup code here, to run once:
 Serial.begin(19200);       // This is the Serial port that communicates with the Arduino IDR Serial monitor
  GPS.begin(9600);           // Serial1 where we communicate with the Adafruit GPS on Arduino pins 0 and 1 - default baud rate
  Serial.println("Connected Location Logger");
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); // RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);    // 1 Hz update rate - Adafruit does not suggest using anything higher than 1 Hz
  //GPS.sendCommand(PGCMD_ANTENNA);            // Request updates on antenna status, comment out to keep quiet
 
}

void loop() {
  char c = GPS.read();        // Receive output from the GPS one character at a time
  if (GPS.newNMEAreceived()) {  // if a sentence is received, we can check the checksum, parse it...
    Serial.println(GPS.lastNMEA());   // Use this line for debugging to see raw feed
    if (!GPS.parse(GPS.lastNMEA()))   // check to ensure we succeeded with parsing
      return;                        // If we fail to parse a sentence we just wait for another
  }

  ParseLocation();
}

boolean ParseLocation() 
// Refer to http://www.gpsinformation.org/dale/nmea.htm#GGA
// Sample data: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
{
  char Latitude[10];
  char Longitude[10];
  float Lat = GPS.latitude;
  float Lon = GPS.longitude;
  float Latcombo = (int)Lat/100 + (((Lat/100 - (int)Lat/100)*100)/60);
  float Loncombo = (int)Lon/100 + (((Lon/100 - (int)Lon/100)*100)/60);
  sprintf(Latitude, "%'.3f", Latcombo);
  sprintf(Longitude, "%'.3f", Loncombo);
  Location = "{\"lat\":" + String(Latitude) + ",\"lng\":-" + String(Longitude) + "}";
  Serial.println(Location);
}
