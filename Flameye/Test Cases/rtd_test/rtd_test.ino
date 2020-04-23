class FlameSafe
{
    int newData;
    int prevData;
    long turnOffCount;
    int sensorValue;
    int id; // use motor enable pin to ID the class
    int flame_sensor_pin;
    long previousMillis ;
    long OnTime = 1000;
    boolean isFlameRaising = false;
//    int riseCount = 0;
  public:
    FlameSafe(int flame_sensor)
    {
      flame_sensor_pin = flame_sensor;

    //Associates the Respective enable pins based on the sensor
      if (flame_sensor == A0)
        id = En_MD1;
      else if (flame_sensor == A1)
        id = En_MD2;
      else if (flame_sensor == A2)
        id = En_MD3;

      newData = 0;
      prevData = 0;
      turnOffCount = 0;
      sensorValue = 0;

    }

    void sensorSloper()
    {
      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= OnTime)
      {
        previousMillis = currentMillis;
        sensorValue = analogRead(flame_sensor_pin);

        // print the results to the serial monitor:

        prevData = newData;
        newData = sensorValue;
     /*   //        Serial.print("sensor " );
        //        Serial.print(String(id) +" =");
        //        Serial.println(sensorValue);
        //        Serial.println("Flame on value" +String(FLAME_START_VALUE));
        //Loop to stop the spark or ignitor
        if (newData > prevData && sensorValue > FLAME_START_VALUE && sensorValue < (FLAME_START_VALUE + 100) ) {
        //          Serial.print("sensor = " );
        //          Serial.print(sensorValue);
        //          Serial.println("!!!Flame On!!!! Temperature raising");
          //Turn off the Flame Ignitor
          turnOffCount = 0; //Initialise the turn off Counter



          //Stop the Spark
        } else if (newData < prevData &&  sensorValue > FLAME_SATURATION_VALUE && turnOffCount < 10) { //Loop to detect if the flame is falling continuosly and turn off the knob
//          Serial.print("Inside sensor = " );
//          Serial.print(sensorValue);
          if (turnOffCount == 9) {
            //Turn off Knob 1
            Serial.println("***Flame is Off Turn off the Knob***");
            turnOffKnob();
          }
//          Serial.print("Count = " );
//          Serial.print(turnOffCount);
          turnOffCount += 1; // Count to count for 10 loop cycle to turn off the knob
        }*/

      //Assigns the variable with the raise or fall
        if(newData > prevData){
             isFlameRaising = true;
        }else if(newData < prevData){
             isFlameRaising = false;
        }

        //Initiates the count in case of raising
        if(isFlameRaising && sensorValue > FLAME_START_VALUE && sensorValue < (FLAME_START_VALUE + 100)){
        
          turnOffCount = 0; // Initialise the count
          
        }else if(!isFlameRaising &&  sensorValue > FLAME_SATURATION_VALUE) { //Loop to detect if the flame is falling continuosly and turn off the knob
            
            switch(turnOffCount){
              case 10: //Turn off Knob
                        Serial.println("***Flame is Off Turn off the Knob***");
                        turnOffKnob();
                        turnOffCount = 0;
                        break;
              default: //Raise the count
                        Serial.print("turnOffCount ="+String(turnOffCount));
                        turnOffCount++;
                        break;
            }
      }
    }
    }
    void turnOffKnob() {
      digitalWrite(SWITCHING_REGULATOR_EN , HIGH);
      digitalWrite(id, 1);
      stepper.step(180);
      digitalWrite(id, 0);
      digitalWrite(SWITCHING_REGULATOR_EN , LOW);

    }

};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
