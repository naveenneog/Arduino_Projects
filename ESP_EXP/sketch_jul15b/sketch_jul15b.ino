
int latchPin = 5;
int clockPin = 4;
int dataPin = 3;
 
int seven_seg_digits[10][4] = { { 0,0,0,0 }, // = 0
                                { 0,0,0,1 }, // = 1
                                { 0,0,1,0 }, // = 2
                                { 0,0,1,1 }, // = 3
                                { 0,1,0,0 }, // = 4
                                { 0,1,0,1 }, // = 5
                                { 0,1,1,0 }, // = 6
                                { 0,1,1,1 }, // = 7
                                { 1,0,0,0 }, // = 8
                                { 1,0,0,1 } // = 9
                                };

void disp7Segm(int digit)
{
    digitalWrite(5, seven_seg_digits[digit][0]);
    digitalWrite(9, seven_seg_digits[digit][1]);
    digitalWrite(10, seven_seg_digits[digit][2]);
    digitalWrite(11, seven_seg_digits[digit][3]);
}

void setup() {
    // set up the LCD's number of columns and rows:
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
}

void loop() {
    // Print a message to the LCD.
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    // print the number of seconds since reset:
    
    for(int j=0; j<100; j++){
    disp7SegmWrite(j);
    delay(500);
    }
}

// This method sends bits to the shift register:
void registerWrite(int whichPin, int whichState) {
    // the bits you want to send
    byte bitsToSend = 0;
    // turn off the output so the pins don't light up
    // while you're shifting bits:
    digitalWrite(latchPin, LOW);
    // turn on the next highest bit in bitsToSend:
    bitWrite(bitsToSend, whichPin, whichState);
    // shift the bits out:
    shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);
    // turn on the output so the LEDs can light up:
    digitalWrite(latchPin, HIGH);
}

void disp7SegmWrite(int number) {
    int num1=0, num2=0;
    if((number < 0) || (number > 99)){
        number = 0;
    }
    num1=number/10;
    num2=number%10;
    // the bits you want to send
    byte bitsToSend = 0;
    // turn off the output so the pins don't light up
    // while you're shifting bits:
    digitalWrite(latchPin, LOW);
    
    //Escreve num1 (digito mais significativo)
    bitWrite(bitsToSend, 0, seven_seg_digits[num1][3]);
    bitWrite(bitsToSend, 1, seven_seg_digits[num1][2]);
    bitWrite(bitsToSend, 2, seven_seg_digits[num1][1]);
    bitWrite(bitsToSend, 3, seven_seg_digits[num1][0]);
    
    //Escreve num2 (digito menos significativo)
    bitWrite(bitsToSend, 4, seven_seg_digits[num2][3]);
    bitWrite(bitsToSend, 5, seven_seg_digits[num2][2]);
    bitWrite(bitsToSend, 6, seven_seg_digits[num2][1]);
    bitWrite(bitsToSend, 7, seven_seg_digits[num2][0]);
    // shift the bits out:
    shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);
    // turn on the output so the LEDs can light up:
    digitalWrite(latchPin, HIGH);
}
