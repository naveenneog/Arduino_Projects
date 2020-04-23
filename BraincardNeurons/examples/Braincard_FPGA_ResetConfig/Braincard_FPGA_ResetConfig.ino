//------------------------------------------------------------------------
// Braincard FPGA ResetConfig
//
// This utility script erases the configuration file of the Braincard FPGA
// stored in the user sectors 8-15 of the Micron 25P80 Flash memory.
//
// At its next initialization, this will force the BrainCard to restore its default
// FPGA configuration residing in sector 0 of the Micron 25P80 Flash.
//
// Copyright 2016 General Vision Inc.
// -----------------------------------------------------------------------

#include <SPI.h>
#include <SPIFlash.h> 

#define FLASH_CSn 8   // FPGA FLASH CSn on D8

SPIFlash flash(FLASH_CSn, 0x2020); // instantiate Flash class
int Sector;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  SPI.begin();
  while (!Serial) {; } // wait for serial port to connect. Needed for native USB port only
  }

void loop() {
     Serial.println("Do you want to erase BC_TOP ? (Y/N)");
     Serial.println("___________________________________");
     while(Serial.available()==0);
     int answer=(Serial.read());
    if (answer=='Y' || answer=='y')
      {
        if(detectFlash(flash))
          {
          for(Sector=8;Sector<16;Sector++)
            {
            eraseSector(flash,Sector);
            }
       Serial.println("___  FPGA BCTOP Erase done! _____");
       Serial.println("_________________________________");
          }
      }
      Serial.println("Exiting...");
for(;;)delay(100);
}


boolean detectFlash(SPIFlash myFlash)
{
Serial.println("Start Flash detect...");
if (myFlash.initialize())
  {
    Serial.println("Flash init OK!");
    Serial.print("DeviceID: ");
    Serial.println(myFlash.readDeviceId(), HEX);
  }
else {Serial.println("Flash init FAIL!");return(false);}
return(true);
}

boolean eraseSector(SPIFlash myFlash, int sector)
{
myFlash.blockErase64K(sector * 0x10000);
while(myFlash.busy());
Serial.print("Sector "); Serial.print(sector); Serial.println(" Erase DONE");
}


