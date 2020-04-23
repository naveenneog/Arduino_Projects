/************************************************************************/
/*																		
 *	BraincardSPI.cpp	--	Driver for the Braincard			        	
 *  revision 1.0, April 22, 2016 
 *	Copyright (c) 2016, General Vision Inc, All rights reserved	
 */
/******************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <BraincardSPI.h>

using namespace std;
extern "C" {
  #include <stdint.h>
}

// ------------------------------------------------------------ //
//    Constructor to the class BraincardNeurons
// ------------------------------------------------------------ 
BraincardSPI::BraincardSPI(){	
}
// ------------------------------------------------------------ 
// Initialize the SPI communication and verify proper interface
// to the CM1K by reading the default Minif value of 2-bytes
// return an error=1 otherwise
// ------------------------------------------------------------ 
int BraincardSPI::connect()
{
	SPI.begin();
	pinMode (braincardSelectPin, OUTPUT);
	pinMode (8, OUTPUT);	// Using FPGA Flash pin
	digitalWrite(10, HIGH);
	digitalWrite(8, HIGH);
	// Reset by pulling both CSn low and returning to high
	digitalWrite(10, LOW);
	digitalWrite(8, LOW);
	delay(200);
	digitalWrite(10, HIGH);
	digitalWrite(8, HIGH);
	delay(500);
	// Return 2 if CM1K present and SPI comm successful
	if(read(6)==2)return(0);else return(1); 
}
// --------------------------------------------------------
// Change the SPI clock divider 
//---------------------------------------------------------
void BraincardSPI::setClockDivider(int divider)
{
	SPI.end();
	switch(divider)
	{
	case 2: SPI.setClockDivider(SPI_CLOCK_DIV2);
	case 4: SPI.setClockDivider(SPI_CLOCK_DIV4);
	case 8: SPI.setClockDivider(SPI_CLOCK_DIV8);
	case 16: SPI.setClockDivider(SPI_CLOCK_DIV16);
	case 32: SPI.setClockDivider(SPI_CLOCK_DIV32);
	case 64: SPI.setClockDivider(SPI_CLOCK_DIV64);
	//case 128: SPI.setClockDivider(SPI_CLOCK_DIV128);
	default: SPI.setClockDivider(SPI_CLOCK_DIV4);
	}	
	SPI.begin();
}
// --------------------------------------------------------
// Read the FPGA_version
//---------------------------------------------------------
int BraincardSPI::FPGArev()
{
	return(read(BC_VERSION));
}
// --------------------------------------------------------
// SPI Read the register of a given module (module + reg = addr)
//---------------------------------------------------------
int BraincardSPI::read(unsigned char reg)
{
	digitalWrite(braincardSelectPin, LOW);
	SPI.transfer(1);  // Dummy for ID
	SPI.transfer(CM1K);
	SPI.transfer(0);
	SPI.transfer(0);
	SPI.transfer(reg);
	SPI.transfer(0);
	SPI.transfer(0);
	SPI.transfer(1); // expect 1 word back
	int data = SPI.transfer(0); // Send 0 to push upper data out
	data = (data << 8) + SPI.transfer(0); // Send 0 to push lower data out
	digitalWrite(braincardSelectPin, HIGH);
	return(data);
}
// ---------------------------------------------------------
// SPI Write the register of a given module (module + reg = addr)
// ---------------------------------------------------------
void BraincardSPI::write(unsigned char reg, int data)
{
	digitalWrite(braincardSelectPin, LOW);
	SPI.transfer(1);  // Dummy for ID
	SPI.transfer((unsigned char)(CM1K + 0x80)); // module and write flag
	SPI.transfer(0);
	SPI.transfer(0);
	SPI.transfer(reg);
	SPI.transfer(0);
	SPI.transfer(0);
	SPI.transfer(1); // expect 1 word back
	SPI.transfer((unsigned char)(data >> 8)); // upper data
	SPI.transfer((unsigned char)(data & 0x00FF)); // lower data
	digitalWrite(braincardSelectPin, HIGH);
}
// ---------------------------------------------------------
// SPI Write_Addr command
// multiple write of data in word format
// length is expressed in words
// ---------------------------------------------------------
void BraincardSPI::writeAddr(long addr, int length, int data[])
{
	digitalWrite(braincardSelectPin, LOW);
	SPI.transfer(1);  // Dummy for ID
	SPI.transfer((byte)(((addr & 0xFF000000) >> 24) + 0x80)); // Addr3 and write flag
	SPI.transfer((byte)((addr & 0x00FF0000) >> 16)); // Addr2
	SPI.transfer((byte)((addr & 0x0000FF00) >> 8)); // Addr1
	SPI.transfer((byte)(addr & 0x000000FF)); // Addr0
	SPI.transfer((byte)((length & 0x00FF0000) >> 16)); // Length2
	SPI.transfer((byte)(length & 0x0000FF00) >> 8); // Length1
	SPI.transfer((byte)(length & 0x000000FF)); // Length 0
	for (int i = 0; i < length; i++)
	{
		SPI.transfer((data[i] & 0xFF00)>> 8);
		SPI.transfer(data[i] & 0x00FF);
	}
	digitalWrite(braincardSelectPin, HIGH);
} 
//---------------------------------------------
// SPI Read_Addr command
//---------------------------------------------
void BraincardSPI::readAddr(long addr, int length, int data[])
{
	digitalWrite(braincardSelectPin, LOW);
	SPI.transfer(1);  // Dummy for ID
	SPI.transfer((byte)((addr & 0xFF000000) >> 24)); // Addr3 and write flag
	SPI.transfer((byte)((addr & 0x00FF0000) >> 16)); // Addr2
	SPI.transfer((byte)((addr & 0x0000FF00) >> 8)); // Addr1
	SPI.transfer((byte)(addr & 0x000000FF)); // Addr0
	SPI.transfer((byte)((length & 0x00FF0000) >> 16)); // Length2
	SPI.transfer((byte)(length & 0x0000FF00) >> 8); // Length1
	SPI.transfer((byte)(length & 0x000000FF)); // Lenght0
	for (int i = 0; i < length; i++)
	{
		data[i] = SPI.transfer(0); // Send 0 to push upper data out
		data[i] = (data[i] << 8) + SPI.transfer(0); // Send 0 to push lower data out
	}
	digitalWrite(braincardSelectPin, HIGH);
}

