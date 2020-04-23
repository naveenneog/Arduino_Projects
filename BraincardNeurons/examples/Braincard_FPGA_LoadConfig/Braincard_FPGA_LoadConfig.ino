//------------------------------------------------------------------------
// Braincard FPGA LoadConfig
//
// This utility script loads a new configuration file of the Braincard FPGA
// from an SD card to the user sectors 8-15 of the Micron 25P80 Flash memory.
// The file must be saved on the SD card as "bc_top.bin"
//
// At initialization of the BrainCard, the FPGA looks for a valid user
// configuration file in sector 8 of its configuration Flash memory.
// If none is found, it loads the default configuration residing in sector 0.
//
// Copyright 2016 General Vision Inc.
// -----------------------------------------------------------------------

#include <SPI.h>
#include <SD.h>
#include <SPIFlash.h> 

#define FLASH_CSn 8   // FPGA FLASH CSn on D8
#define SD_CSn 9      //SD CSn on D9
#define BUFF_SIZE 256
File myFile;
File root;
SPIFlash flash(FLASH_CSn, 0x2020); // instantiate Flash class
byte myBuffer[BUFF_SIZE];
long myAddress;
int Sector;
int nbr;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {; } // wait for serial port to connect. Needed for native USB port only
  Serial.println();

  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CSn)) {
    Serial.println("SD init FAIL!");
    return;
  } else // SD found try to program the BrainCard config Flash
    {
    Serial.println("SD init OK");
    Serial.println("SD directory is:");
    root = SD.open("/");
    printDirectory(root, 0);
    myFile = SD.open("bc_top.bin");
      if (myFile) {
        Serial.println("Config file: bc_top.bin is ready");
        Serial.println("____________________________");
        if(detectFlash(flash))
          {
          for(Sector=8;Sector<16;Sector++)
            {
            eraseSector(flash,Sector);
            }
            
        myAddress=0x80000;
        // Flash programming takes place here
        // close the file:
          while(myFile.available())
          {
            nbr=myFile.read(myBuffer,BUFF_SIZE);
            flash.writeBytes(myAddress,myBuffer,BUFF_SIZE);
            Serial.print("Writing ");Serial.print(nbr);  Serial.print(" bytes at address ");Serial.println(myAddress,HEX);
            myAddress+=BUFF_SIZE;
          }
        }
        myFile.close();
       Serial.println("___  FPGA update done! _____");
       Serial.println("____________________________");
      } else { 
      Serial.println("Error opening bc_top.bin");// if the file didn't open, print an error:
      }
  }
}

void loop() {
  // nothing happens after setup
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
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


