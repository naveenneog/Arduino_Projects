/************************************************************************/
/*																		
 *	BraincardNeurons.h	--	Driver for the Braincard			        	
 *  revision 1.1, September 16, 2016 
 *	Copyright (c) 2016, General Vision Inc, All rights reserved	
 *
 * http://general-vision.com/documentation/TM_NeuroMem_Technology_Reference_Guide.pdf
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/******************************************************************************/
#ifndef _BraincardNeurons_h_
#define _BraincardNeurons_h_

#include "BraincardSPI.h"

extern "C" {
  #include <stdint.h>
}

class BraincardNeurons
{
	public:
		// Modules of the Braincard (Byte #0 in map address)
		static const int CM1K=0x01;
		
		// Registers of CM1K (Byte #3 in map address)
		static const int NM_NCR=0x00;
		static const int NM_COMP=0x01;
		static const int NM_LCOMP=0x02;
		static const int NM_DIST=0x03;
		static const int NM_IDX=0x03;
		static const int NM_CAT=0x04;
		static const int NM_AIF=0x05;
		static const int NM_MINIF=0x06;
		static const int NM_MAXIF=0x07;
		static const int NM_TESTCOMP=0x08;
		static const int NM_TESTCAT=0x09;
		static const int NM_NID=0x0A;
		static const int NM_GCR=0x0B;
		static const int NM_RSTCHAIN=0x0C;
		static const int NM_NSR=0x0D;
		static const int NM_NCOUNT=0x0F	;
		static const int NM_FORGET=0x0F;
				
		static const int NEURONSIZE=256; //memory capacity of each neuron in byte
		static const int MAXNEURONS=1024; //number of neurons per chip
		
		BraincardNeurons();
		int begin();
		void forget();
		void forget(int Maxif);
		void clearNeurons();
		
		void setContext(int context, int minif, int maxif);
		void getContext(int* context, int* minif, int* maxif);
		void setRBF();
		void setKNN();
		
		int broadcast(unsigned char vector[], int length);
		int learn(unsigned char vector[], int length, int category);
		int classify(unsigned char vector[], int length);
		int classify(unsigned char vector[], int length, int* distance, int* category, int* nid);
		int classify(unsigned char vector[], int length, int K, int distance[], int category[], int nid[]);

		void readNeuron(int nid, int* context, unsigned char model[], int* aif, int* category);
		void readNeuron(int nid, unsigned char neuron[]);
		int readNeurons(unsigned char neurons[], int ncount);
		int writeNeurons(unsigned char neurons[], int ncount);

 		struct neuronContent
		{
			int context;
			int model[NEURONSIZE];
			int aif;
			int minif;
			int category;
		}; 		
		//void readNeuron2(int nid, neuronContent data);
		
		//-----------------------------------
		// Access to global neuron registers
		//-----------------------------------
		int NCOUNT();
		void GCR(int value);
		int GCR();
		void MINIF(int value);
		int MINIF();
		void MAXIF(int value);
		int MAXIF();
		void NSR(int value);
		int NSR();
		void AIF(int value);
		int AIF();
		void CAT(int value);
		int CAT();
		void IDX(int value);
		int DIST();
		void RSTCHAIN();		
		void COMP(int value);
		int COMP();
		void LCOMP(int value);
		int LCOMP();

};
#endif
