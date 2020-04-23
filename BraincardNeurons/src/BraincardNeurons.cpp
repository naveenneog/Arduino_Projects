/************************************************************************/
/*																		
 *	BraincardNeurons.cpp	--	Driver for the Braincard			        	
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

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <BraincardNeurons.h>
#include <BraincardSPI.h>

using namespace std;
extern "C" {
  #include <stdint.h>
}

BraincardSPI spi;

// ------------------------------------------------------------ //
//    Constructor to the class BraincardNeurons
// ------------------------------------------------------------ 
BraincardNeurons::BraincardNeurons(){	
}
// ------------------------------------------------------------ 
// Initialize the neural network
// ------------------------------------------------------------ 
int BraincardNeurons::begin()
{
	int error=spi.connect();
	if (error==0) clearNeurons();
	return(error);
}

// ------------------------------------------------------------ 
// Un-commit all the neurons, so they become ready to learn
// Reset the Maximum Influence Field to default value=0x4000
// ------------------------------------------------------------ 
void BraincardNeurons::forget()
{
	spi.write(NM_FORGET, 0);
}
// ------------------------------------------------------------ 
// Un-commit all the neurons, so they become ready to learn,
// Set the Maximum Influence Field (default value=0x4000)
// ------------------------------------------------------------ 
void BraincardNeurons::forget(int Maxif)
{
	spi.write(NM_FORGET, 0);
	spi.write(NM_MAXIF, Maxif);
}
// --------------------------------------------------------------
// Un-commit all the neurons, so they become ready to learn,
// Set the Maximum Influence Field (default value=0x4000)
// Clear the memory of the neurons
// --------------------------------------------------------------
void BraincardNeurons::clearNeurons()
{
	int TempNSR=spi.read(NM_NSR);
	spi.write(NM_FORGET,0);
	spi.write(NM_NSR, 16);
	for (int i=0; i< NEURONSIZE; i++) spi.write(NM_TESTCOMP,0);
	spi.write(NM_NSR, TempNSR);
}
// --------------------------------------------------------
// Broadcast a vector to the neurons and return the recognition status
// 0= unknown, 4=uncertain, 8=Identified
//---------------------------------------------------------
int BraincardNeurons::broadcast(unsigned char vector[], int length)
{
	//spi.writeAddr(0x01000001, length-1, vector);
	for (int i=0; i<length-1;i++) spi.write(NM_COMP, vector[i]);
	spi.write(NM_LCOMP, vector[length-1]);
	return(spi.read(NM_NSR));
}
//-----------------------------------------------
// Learn a vector using the current context value
//----------------------------------------------
int BraincardNeurons::learn(unsigned char vector[], int length, int category)
{
	broadcast(vector, length);
	spi.write(NM_CAT,category);
	return(spi.read(NM_NCOUNT));
}
// ---------------------------------------------------------
// Classify a vector and return its classification status
// NSR=0, unknown
// NSR=8, identified
// NSR=4, uncertain
// ---------------------------------------------------------
int BraincardNeurons::classify(unsigned char vector[], int length)
{
	broadcast(vector, length);
	return(spi.read(NM_NSR));
}
//----------------------------------------------
// Recognize a vector and return the best match, or the 
// category, distance and identifier of the top firing neuron
//----------------------------------------------
int BraincardNeurons::classify(unsigned char vector[], int length, int* distance, int* category, int* nid)
{
	broadcast(vector, length);
	*distance = spi.read(NM_DIST);
	*category= spi.read(NM_CAT) & 0x7FFF; //Degenerated bit15 is masked
	*nid =spi.read(NM_NID);
	return(spi.read(NM_NSR));
}
//----------------------------------------------
// Recognize a vector and return the response  of up to K top firing neurons
// The response includes the distance, category and identifier of the neuron
// The Degenerated flag of the category is masked rmask the degenerated response, use the current context value
// Return the number of firing neurons or K whichever is smaller
//----------------------------------------------
int BraincardNeurons::classify(unsigned char vector[], int length, int K, int distance[], int category[], int nid[])
{
	int recoNbr=0;
	broadcast(vector, length);
	for (int i=0; i<K; i++)
	{
		distance[i] = spi.read(NM_DIST);
		if (distance[i]==0xFFFF)
		{ 
			category[i]=0xFFFF;
			nid[i]=0xFFFF;
		}
		else
		{
			recoNbr++;
			category[i]= spi.read(NM_CAT) & 0x7FFF; //Degenerated bit15 is masked
			nid[i] =spi.read(NM_NID);
		}
	}
return(recoNbr);
}
// ------------------------------------------------------------ 
// Set a context and associated minimum and maximum influence fields
// ------------------------------------------------------------ 
void BraincardNeurons::setContext(int context, int minif, int maxif)
{
	spi.write(NM_GCR, context);
	spi.write(NM_MINIF, minif);
	spi.write(NM_MAXIF, maxif);
}
// ------------------------------------------------------------ 
// Get a context and associated minimum and maximum influence fields
// ------------------------------------------------------------ 
void BraincardNeurons::getContext(int* context, int* minif, int* maxif)
{
	*context = spi.read(NM_GCR);
	*minif= spi.read(NM_MINIF); 
	*maxif =spi.read(NM_MAXIF);
}
// --------------------------------------------------------
// Set the neurons in Radial Basis Function mode (default)
//---------------------------------------------------------
void BraincardNeurons::setRBF()
{
	int tempNSR=spi.read(NM_NSR);
	spi.write(NM_NSR, tempNSR & 0xDF);
}
// --------------------------------------------------------
// Set the neurons in K-Nearest Neighbor mode
//---------------------------------------------------------
void BraincardNeurons::setKNN()
{
	int tempNSR=spi.read(NM_NSR);
	spi.write(NM_NSR, tempNSR | 0x20);
}
//-------------------------------------------------------------
// Read the contents of the neuron pointed by index in the chain of neurons
// starting at index 1
//-------------------------------------------------------------
void BraincardNeurons::readNeuron(int nid, int* context, unsigned char model[], int* aif, int* category)
{
	if (nid==0) return;
	int TempNSR=spi.read(NM_NSR);
	spi.write(NM_NSR, 0x10);
	spi.write(NM_RSTCHAIN, 0);
	for (int i=0; i<nid-1; i++) spi.read(NM_CAT); // move to index in the chain of neurons
	*context=spi.read(NM_NCR);
	for (int j=0; j<NEURONSIZE; j++) model[j]=spi.read(NM_COMP);
	*aif=spi.read(NM_AIF);
	*category=spi.read(NM_CAT);
	spi.write(NM_NSR, TempNSR); // set the NN back to its calling status
}
//-------------------------------------------------------------
// Read the contents of the neuron pointed by index in the chain of neurons
// Returns an array of 264 bytes with the following format
// 2-bytes NCR, 256-bytes COMP, 2-bytes AIF, 2-bytes MINIF, 2-bytes CAT
//-------------------------------------------------------------
void BraincardNeurons::readNeuron(int nid, unsigned char neuron[])
{
	if (nid==0) return;
	int TempNSR=spi.read(NM_NSR); // save value to restore NN status upon exit of the function
	spi.write(NM_NSR, 0x0010);
	spi.write(NM_RSTCHAIN, 0);
	for (int i=0; i<nid-1; i++) spi.read(NM_CAT); // move to index in the chain of neurons
	int Temp=spi.read(NM_NCR);
	neuron[0]=(unsigned char)((Temp & 0xFF00)>>8);
	neuron[1]=(unsigned char)(Temp & 0x00FF);
	for (int j=0; j<256; j++) { Temp=spi.read(NM_COMP); neuron[j+2]= (unsigned char)Temp;}
	Temp=spi.read(NM_AIF);
	neuron[258]=(unsigned char)((Temp & 0xFF00)>>8);
	neuron[259]=(unsigned char)(Temp & 0x00FF);
	Temp=spi.read(NM_MINIF);
	neuron[260]=(unsigned char)((Temp & 0xFF00)>>8);
	neuron[261]=(unsigned char)(Temp & 0x00FF);
	Temp=spi.read(NM_CAT);
	neuron[262]=(unsigned char)((Temp & 0xFF00)>>8);
	neuron[263]=(unsigned char)(Temp & 0x00FF);
	spi.write(NM_NSR, TempNSR); // set the NN back to its calling status
}
//----------------------------------------------------------------------------
// Read the contents of "ncount" neurons, with ncount being less than or equal
// to the number of committed neurons. 
// Returns an array of ncount records of 264 bytes with the following format
// 2-bytes NCR, 256-bytes COMP, 2-bytes AIF, 2-bytes MINIF, 2-bytes CAT
//----------------------------------------------------------------------------
int BraincardNeurons::readNeurons(unsigned char neurons[], int ncount)
{
	int TempNSR=spi.read(NM_NSR); // save value to restore NN status upon exit of the function
	spi.write(NM_NSR, 0x0010);
	spi.write(NM_RSTCHAIN, 0);
	for (int i=0; i<ncount; i++)
	{
		neurons[(i*264)+0]=0;
		int Temp=spi.read(NM_NCR);
		neurons[(i*264)+1]=(unsigned char)(Temp & 0x00FF);
		for (int j=0; j<256; j++) { Temp=spi.read(NM_COMP); neurons[(i*264)+j+2]= (unsigned char)Temp;}
		Temp=spi.read(NM_AIF);
		neurons[(i*264)+258]=(unsigned char)((Temp & 0xFF00)>>8);
		neurons[(i*264)+259]=(unsigned char)(Temp & 0x00FF);
		Temp=spi.read(NM_MINIF);
		neurons[(i*264)+260]=(unsigned char)((Temp & 0xFF00)>>8);
		neurons[(i*264)+261]=(unsigned char)(Temp & 0x00FF);
		Temp=spi.read(NM_CAT);
		neurons[(i*264)+262]=(unsigned char)((Temp & 0xFF00)>>8);
		neurons[(i*264)+263]=(unsigned char)(Temp & 0x00FF);
		if ((Temp==0xFFFF) | (Temp==0)) break;
	}
	spi.write(NM_NSR, TempNSR); // set the NN back to its calling status
	return(spi.read(NM_NCOUNT));
}
//---------------------------------------------------------------------
// Clear and restore the content of ncount neurons from an array of
// ncount records of 264 bytes with the following format:
// 2-bytes NCR, 256-bytes COMP, 2-bytes AIF, 2-bytes MINIF, 2-bytes CAT
//---------------------------------------------------------------------
int BraincardNeurons::writeNeurons(unsigned char neurons[], int ncount)
{
	int TempNSR=spi.read(NM_NSR); // save value to restore NN status upon exit of the function
	int TempGCR=spi.read(NM_GCR);
	spi.write(NM_FORGET, 0);
	spi.write(NM_NSR, 0x0010);
	spi.write(NM_RSTCHAIN,0 );
	for (int i=0; i<ncount; i++)
	{
		spi.write(NM_NCR, neurons[(i*264)+1]);
		for (int j=0; j<256; j++) spi.write(NM_COMP, neurons[(i*264)+j+2]);
		spi.write(NM_AIF, (neurons[(i*264)+258]<<8)+neurons[(i*264)+259]);
		spi.write(NM_MINIF, (neurons[(i*264)+260]<<8)+neurons[(i*264)+261]);
		int cat=(neurons[(i*264)+262]<<8)+neurons[(i*264)+263];
		spi.write(NM_CAT, cat );
		if ((cat==0xFFFF) | (cat==0)) break;
	}
	spi.write(NM_NSR, TempNSR); // set the NN back to its calling status
	spi.write(NM_GCR, TempGCR);
	return(spi.read(NM_NCOUNT));
}
// --------------------------------------------------------
// Read the number of committed neurons
//---------------------------------------------------------
int BraincardNeurons::NCOUNT()
{
	return(spi.read(NM_NCOUNT));
}
// --------------------------------------------------------
// Get/Set the Minimum Influence Field register
//---------------------------------------------------------
void BraincardNeurons::MINIF(int value)
{
	spi.write(NM_MINIF, value);
}
int BraincardNeurons::MINIF()
{
	return(spi.read(NM_MINIF));
}
// --------------------------------------------------------
// Get/Set the Maximum Influence Field register
//---------------------------------------------------------
void BraincardNeurons::MAXIF(int value)
{
	spi.write(NM_MAXIF, value);
}
int BraincardNeurons::MAXIF()
{
	return(spi.read(NM_MAXIF));
}
// --------------------------------------------------------
// Get/Set the Global Context register
//---------------------------------------------------------
void BraincardNeurons::GCR(int value)
{
	spi.write(NM_GCR, value);
}
int BraincardNeurons::GCR()
{
	return(spi.read(NM_GCR));
}
// --------------------------------------------------------
// Get/Set the Category register
//---------------------------------------------------------
void BraincardNeurons::CAT(int value)
{
	spi.write(NM_CAT, value);
}
int BraincardNeurons::CAT()
{
	return(spi.read(NM_CAT));
}
// --------------------------------------------------------
// Get the Distance register
//---------------------------------------------------------
int BraincardNeurons::DIST()
{	
	return(spi.read(NM_DIST));
}
// --------------------------------------------------------
// Set the Component Index register
//---------------------------------------------------------
void BraincardNeurons::IDX(int value)
{
	spi.write(NM_IDX, value);
}
// --------------------------------------------------------
// Get/Set the Network Status register
// bit 2 = UNC (read only)
// bit 3 = ID (read only)
// bit 4 = SR mode
// bit 5= KNN mode
//---------------------------------------------------------
void BraincardNeurons::NSR(int value)
{
	spi.write(NM_NSR, value);
}
int BraincardNeurons::NSR()
{
	return(spi.read(NM_NSR));
}
// --------------------------------------------------------
// Get/Set the AIF register
//---------------------------------------------------------
void BraincardNeurons::AIF(int value)
{
	spi.write(NM_AIF, value);
}
int BraincardNeurons::AIF()
{
	return(spi.read(NM_AIF));
}
// --------------------------------------------------------
// Reset the chain to first neuron in SR Mode
//---------------------------------------------------------
void BraincardNeurons::RSTCHAIN()
{
	spi.write(NM_RSTCHAIN, 0);
}
// --------------------------------------------------------
// Get/Set the COMP register (component)
//---------------------------------------------------------
void BraincardNeurons::COMP(int value)
{
	spi.write(NM_COMP, value);
}
int BraincardNeurons::COMP()
{
	return(spi.read(NM_COMP));
}
// --------------------------------------------------------
// Get/Set the LCOMP register (last component)
//---------------------------------------------------------
void BraincardNeurons::LCOMP(int value)
{
	spi.write(NM_LCOMP, value);
}
int BraincardNeurons::LCOMP()
{
	return(spi.read(NM_LCOMP));
}


/*
void BraincardNeurons::readNeuron2(int nid, neuronContent data)
{
	if (nid==0) return;
	int TempNSR=NM_NSR;
	NM_NSR=0x0010;
	NM_RSTCHAIN= 0;
	for (int i=0; i< nid-1; i++) NM_CAT; // move to index in the chain of neurons
	data.context=NM_NCR;
	for (int j=0; j<NEURONSIZE; j++) data.model[j]=NM_COMP;
	data.aif=NM_AIF;
	data.minif=NM_MINIF;
	data.category=NM_CAT;
	NM_NSR=TempNSR; // set the NN back to its calling status
} */