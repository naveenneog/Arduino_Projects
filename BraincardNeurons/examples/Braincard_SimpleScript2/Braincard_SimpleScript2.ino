//------------------------------------------------------------------------
// Braincard SimpleScript
// Equivalent to SimpleScript +
// use of the neurons in KNN and RBF mode,
// use of multiple contexts
//
// http://general-vision.com/documentation/TM_NeuroMem_Technology_Reference_Guide.pdf */
//
// Copyright 2016 General Vision Inc.
// -----------------------------------------------------------------------
//
// The patterns are arrays of length LEN composed of identical values VAL
// They basically represent horizontal lines with different heights. This representation
// is easy to comprehend the distance between the learned lines (stored in the memory
// of the committed neurons) and a new input line

#include <BraincardNeurons.h>
BraincardNeurons hNN;

int ncr, dist, cat, aif, nid, nsr, ncount;

#define LEN 4
byte pattern[LEN]; // values must range between 0-255. Upper byte is discarded by CM1K

#define K 3
int dists[K], cats[K], nids[K];

void setup() {
  Serial.begin(9600);
  while (!Serial);    // wait for the serial port to open

  Serial.println("Welcome to the Braincard Script 2");
  
  // Initialize the neurons
  hNN.begin();
    
  //Build knowledge by learning 3 patterns with each constant values (respectively 11, 15 and 20)
  Serial.print("\nLearning three patterns...");
  for (int i=0; i<LEN; i++) pattern[i]=11;
  hNN.learn(pattern,LEN, 55);
  for (int i=0; i<LEN; i++) pattern[i]=15;
  hNN.learn(pattern,LEN, 33);
  for (int i=0; i<LEN; i++) pattern[i]=20;
  int neuronsCommitted=hNN.learn(pattern,LEN, 100);
   
  displayNeurons(neuronsCommitted, LEN);

  hNN.setKNN(); 
  Serial.print("\n\nRecognizing pattern ");
  for (int i=0; i<LEN; i++) pattern[i]=12;     
  for (int i=0; i<LEN; i++) { Serial.print(pattern[i]); Serial.print(", ");}
  Serial.print("\nin mode KNN (K=3) ");
  int responseNbr=hNN.classify(pattern, LEN, K, dists, cats, nids);
  for (int i=0; i<responseNbr; i++)
  {
      Serial.print("\nFiring neuron "); Serial.print(nids[i]);
      Serial.print(", Category="); Serial.print(cats[i]);
      Serial.print(", at Distance="); Serial.print(dists[i]);
  }

  hNN.setRBF();
  Serial.print("\n\nRecognizing pattern ");
  for (int i=0; i<LEN; i++) pattern[i]=12;     
  for (int i=0; i<LEN; i++) { Serial.print(pattern[i]); Serial.print(", ");}
  Serial.print("\nin mode RBF (K up to 3) ");
  responseNbr=hNN.classify(pattern, LEN, K, dists, cats, nids);
  for (int i=0; i<responseNbr; i++)
  {
      Serial.print("\nFiring neuron "); Serial.print(nids[i]);
      Serial.print(", Category="); Serial.print(cats[i]);
      Serial.print(", at Distance="); Serial.print(dists[i]);
  }
  
  // change the context to witness that a 2nd sub-network can be trained independantly
  // of the content of the 1st sub-network
  hNN.GCR(2);
  Serial.print("\n\nLearning a new pattern (13) under a different context=2");
  for (int i=0; i<LEN; i++) pattern[i]=13;
  neuronsCommitted=hNN.learn(pattern,LEN, 100);

  displayNeurons(neuronsCommitted, LEN);
  Serial.print("\nNotice the commitment of new neuron with context 2 and consequently");
  Serial.print("\nno impact on the influence fields of neuron1 and 2 belonging to context 1");
  
  hNN.GCR(1);
  Serial.print("\n\nLearning the same example (13) under context=1");
  for (int i=0; i<LEN; i++) pattern[i]=13;
  neuronsCommitted=hNN.learn(pattern,LEN, 100);

  displayNeurons(neuronsCommitted, LEN);
  Serial.print("\nNotice the commitment of new neuron with context 1");
  Serial.print("\nand the shrinking of the influence fields of neuron1 and 2");
  
  Serial.print("\n\nEdit a value between [10 and 30] as the amplitude of a new pattern + Enter");
}

void loop()
{
  if (Serial.available() == 3) 
  {
    byte VAL = Serial.read() - 48;
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
  void displayNeurons(int count, int length)
  {
    // display the content of the first "ncount" committed neurons
    // limiting the display of the model to the length "lengh"
    int ncr=0, cat=0, aif=0;
    int ncount= hNN.NCOUNT();
    if (count>ncount) count=ncount;
    byte model[hNN.NEURONSIZE];
    Serial.print("\n\nTotal committed neurons = "); Serial.print(ncount);
    Serial.print("\nDisplay the first "); Serial.print(count);
    for (int i=1; i<=count; i++)
    {  
        hNN.readNeuron(i, &ncr, model, &aif, &cat);
        Serial.print("\nneuron "); Serial.print(i); Serial.print("\tmodel=");
        for (int k=0; k<length; k++) { Serial.print(model[k]); Serial.print(", ");} 
        Serial.print("\tncr="); Serial.print(ncr);  
        Serial.print("\taif="); Serial.print(aif);     
        Serial.print("\tcat="); Serial.print(cat);
    }
    Serial.print("\n----------------------------------");
  }
