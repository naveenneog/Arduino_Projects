// -----------------------------------------------------------------------
// BrainCard Test_SPIcomm
// General Vision Inc.
// Copyright 2016 General Vision Inc.
// -----------------------------------------------------------------------

// This example uses the BraincardSPI class and ignores the BraincardNeurons class

#include <BraincardSPI.h>
BraincardSPI bcSPI;

//Definition of the CM1K registers used for the R/W test in save and restore mode
#define COMP 0x01
#define CAT 0x04
#define MINIF 0x06
#define MAXIF 0x07
#define RSTCHAIN 0x0C
#define NSR 0x0D
#define FORGET 0x0F

#define LEN 256
int pattern[LEN];
int pattern2[LEN];

int iteration=0, error=0, val=0, value1=2, value2=255;

void setup() 
{
  Serial.begin(115200);
  while (!Serial);  // wait for the serial port to open

  if (bcSPI.connect()!=0) Serial.print("Warning!!! BrainCard not properly responding. Verify connection and power supply");
  else {Serial.print("Welcome to the BrainCard rev ");Serial.println(bcSPI.FPGArev());}

  // Need to troubleshoot an error? Start by changing the clock divider
  // Uncomment if the default clock divider of 4 is unsufficient
  bcSPI.setClockDivider(8); 
}

void loop() {
  if (Serial)
  {   
    error=0;
    
    //---------------------------------------------
    // Test SPI Read/Write of single words
    //---------------------------------------------
    bcSPI.write(FORGET,0);
    bcSPI.write(MINIF,value1);
    bcSPI.write(MAXIF,value2);
    val=bcSPI.read(MINIF);
    if (val!=value1) {error=1; Serial.print("\nerror1, erroneous Minif=");Serial.print(val);}
    val=bcSPI.read(MAXIF);
    if (val!=value2) {error=2; Serial.print("\nerror2, erroneous Maxif=");Serial.print(val);}
    
    //---------------------------------------------
    // Test SPI Read/Write of multiple words
    // neurons are set to Save and Restore mode for this test
    //---------------------------------------------
    bcSPI.write(NSR,16); // set Save and Restore mode
    bcSPI.write(RSTCHAIN,0); // point to 1st neuron of the chain
    val=0; //initial value of the slope vectors
    for (int i=0; i<LEN; i++)
    {
      pattern[i]= val; 
      if (val>=255) val=0; else val++;
    }
    bcSPI.writeAddr(0x01000001,LEN,pattern);
    bcSPI.write(CAT,1);

    bcSPI.write(RSTCHAIN,0); //point to 1st neuron of the chain
    bcSPI.readAddr(0x01000001,LEN,&pattern2[0]);
    bcSPI.read(CAT);
    
    for (int i=0; i<LEN; i++) 
    {
      if (pattern2[i]!= pattern[i]) 
      {
        error=3; Serial.print("\nerror3, at iteration "); Serial.print(iteration);
        Serial.print(", expected COMP="); Serial.print(pattern[i]);
        Serial.print(", returned value="); Serial.print(pattern2[i]);
        break;
      }
    }
    bcSPI.write(NSR,0); // set Normal mode
    
    //---------------------------------------------
    // End of the test
    //---------------------------------------------
    
    iteration++;
    if (iteration % 100 ==0)
    { 
      if (error==0) 
      {
        Serial.print("\nIteration ");Serial.print(iteration); Serial.print(" Pass");
      }
    }
    value1++; if (value1>255) value1=0;
    value2--; if (value2<0) value2= 255;   
  }
}


