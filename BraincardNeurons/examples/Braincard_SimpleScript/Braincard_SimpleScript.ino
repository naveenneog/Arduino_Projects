#include <BraincardNeurons.h>
#include <BraincardSPI.h>
#include <SPIFlash.h>

//------------------------------------------------------------------------
// Braincard SimpleScript
//
// Simple Test Script to understand how the neurons learn and recognize patterns of numbers
//
// Refer to http://www.general-vision.com/documentation/TM_TestNeurons_SimpleScript.pdf
//
// Copyright 2016 General Vision Inc.
// -----------------------------------------------------------------------
//
// The patterns are arrays of length LEN composed of identical values VAL
// They basically represent horizontal lines with different heights. This representation
// is easy to comprehend the distance between the learned lines (stored in the memory
// of the committed neurons) and a new input line
//
// In this script, the patterns to recognize are generated programatically to
// surround the learned patterns

#include <BraincardNeurons.h>
BraincardNeurons hNN;

byte VAL=11;
int ncr, dist, cat, aif, nid, nsr, ncount;

#define LEN 4
byte pattern[LEN]; // values must range between 0-255. Upper byte is discarded by CM1K

#define K 3
int dists[K], cats[K], nids[K];

void setup() {
  Serial.begin(9600);
  while (!Serial);    // wait for the serial port to open
  
  if (hNN.begin()!=0) Serial.print("Warning!!! BrainCard not properly responding. Verify connection and power supply");
  else {Serial.print("Welcome to the BrainCard");}
  
  //Build knowledge by learning 3 patterns with each constant values (respectively 11, 15 and 20)
  Serial.print("\nLearning three patterns...");
  for (int i=0; i<LEN; i++) pattern[i]=11;
  hNN.learn(pattern,LEN, 55);
  for (int i=0; i<LEN; i++) pattern[i]=15;
  hNN.learn(pattern,LEN, 33);
  for (int i=0; i<LEN; i++) pattern[i]=20;
  ncount=hNN.learn(pattern,LEN, 100);
   
  // Display the content of the committed neurons
  Serial.print("\nDisplay the neurons, count="); Serial.print(ncount);
  byte model[hNN.NEURONSIZE];
  for (int i=1; i<=ncount; i++)
  {  
      hNN.readNeuron(i, &ncr, model, &aif, &cat);
      Serial.print("\nneuron "); Serial.print(i); Serial.print("\tmodel=");
      for (int k=0; k<LEN; k++) { Serial.print(model[k]); Serial.print(", ");} 
      Serial.print("\tncr="); Serial.print(ncr);  
      Serial.print("\taif="); Serial.print(aif);     
      Serial.print("\tcat="); Serial.print(cat);
  }
  
  for (VAL=12; VAL<16; VAL++)
  {
      Serial.print("\n\nRecognizing a new pattern: ");
      for (int i=0; i<LEN; i++) pattern[i]=VAL;     
      for (int i=0; i<LEN; i++) { Serial.print(pattern[i]); Serial.print(", ");}
      int responseNbr=hNN.classify(pattern, LEN, K, dists, cats, nids);
      for (int i=0; i<responseNbr; i++)
      {
          Serial.print("\nFiring neuron "); Serial.print(nids[i]);
          Serial.print(", Category="); Serial.print(cats[i]);
          Serial.print(", at Distance="); Serial.print(dists[i]);
      }
  }

  Serial.print("\n\nLearning a new example (13) falling between neuron1 and neuron2...");
  for (int i=0; i<LEN; i++) pattern[i]=13;
  ncount=hNN.learn(pattern,LEN, 100);

  // Display the content of the committed neurons
  Serial.print("\nDisplay the neurons, count="); Serial.print(ncount);
  for (int i=1; i<=ncount; i++)
  {  
      hNN.readNeuron(i, &ncr, model, &aif, &cat);
      Serial.print("\nneuron "); Serial.print(i); Serial.print("\tmodel=");
      for (int k=0; k<LEN; k++) { Serial.print(model[k]); Serial.print(", ");} 
      Serial.print("\tncr="); Serial.print(ncr);  
      Serial.print("\taif="); Serial.print(aif);     
      Serial.print("\tcat="); Serial.print(cat);
  }
  Serial.println("\n\nNotice the addition of neuron 4 and");
  Serial.println("the shrinking of the influence fields of neuron1 and 2");
  
  //Prompt User for input
  Serial.print("\n\nEdit a value between [10 and 30] as the amplitude of a new pattern + Enter");
}

void loop()
{
  if (Serial.available() == 3) 
  {
      VAL = Serial.read() - 48;
      VAL = VAL *10 + Serial.read() - 48;
      Serial.read(); // to read the newline
  
      for (int i=0; i<LEN; i++) pattern[i]=VAL;     
      Serial.print("\npattern=");
      for (int i=0; i<LEN; i++) { Serial.print(pattern[i]); Serial.print(", ");}
    
      int responseNbr=hNN.classify(pattern, LEN, K, dists, cats, nids);
      for (int i=0; i<responseNbr; i++)
      {
          Serial.print("\nFiring neuron "); Serial.print(nids[i]);
          Serial.print(", Category="); Serial.print(cats[i]);
          Serial.print(", at Distance="); Serial.print(dists[i]);
      }
      Serial.print("\n\nEdit a value between [10 and 30] as the amplitude of a new pattern + Enter");
   }
 }

