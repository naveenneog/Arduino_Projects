/************************************************************************/
/*																		
 *	BraincardNeurons.h	--	Driver for the Braincard			        	
 *  revision 1.0, April 22, 2016 
 *	Copyright (c) 2016, General Vision Inc, All rights reserved	
 */
/******************************************************************************/
#ifndef _BraincardSPI_h_
#define _BraincardSPI_h_

#include "SPI.h"

extern "C" {
  #include <stdint.h>
}

class BraincardSPI
{
	public:
				
		BraincardSPI();
		int braincardSelectPin = 10;
		int connect();		
		void setClockDivider(int divider);
		int FPGArev();	
		
		int read(unsigned char reg);
		void write(unsigned char reg, int data);
		void writeAddr(long addr, int length, int data[]);
		void readAddr(long addr, int length, int data[]);						
		
		static const int CM1K=0x01; // byte 2 in Addr to access the neurons
		static const int BC_VERSION=0x0E; // byte 5 to read FPGA version
};
#endif
