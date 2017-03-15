//These are the pin numbers of the different servos.
unsigned int FLS = 32; //front-left shoulder pin number
unsigned int FLE = 21; //front-left elbow pin number
unsigned int FRS = 10; //front-right shoulder pin number
unsigned int FRE = 20; //front-right elbow pin number
unsigned int BLS = 25; //back-left shoulder pin number
unsigned int BLE = 22; //back-left elbow pin number
unsigned int BRS = 6; //back-right shoulder pin number
unsigned int BRE = 9; //back-right elbow pin number
unsigned int MLS = 4; //middle-left shoulder
unsigned int MLE = 23; //middle-left elbow
unsigned int MRS = 5; //middle-right shoulder
unsigned int MRE = 3; //middle-right elbow

unsigned int servoPinNumbers[] = {FLS, FLE, 
                                  FRS, FRE,
                                  BLS, BLE,
                                  BRS, BRE,
                                  MLS, MLE,
                                  MRS, MRE};

unsigned int PWMMin = 3277;
unsigned int PWMMax = 6553;

unsigned int PWMValues[7][12][3] = { //#actions = 7, 
                                     //#servos = 12,
                                     //#PWM values = 3 (this will eventually be much higher)
  { //stand up
    {1, 3, 7}, //FLS PWM values
    {1, 3, 7}, //FLE PWM values
    {1, 3, 7}, //FRS PWM values
    {1, 3, 7}, //FRE PWM values
    {1, 3, 7}, //BLS PWM values
    {1, 3, 7}, //BLE PWM values
    {1, 3, 7}, //BRS PWM values
    {1, 3, 7}, //BRE PWM values
    {1, 3, 7}, //MLS PWM values
    {1, 3, 7}, //MLE PWM values
    {1, 3, 7}, //MRS PWM values
    {1, 3, 7} //MRE PWM values
  },
  { //move forward
    {1, 3, 7}, //FLS PWM values
    {1, 3, 7}, //FLE PWM values
    {1, 3, 7}, //FRS PWM values
    {1, 3, 7}, //FRE PWM values
    {1, 3, 7}, //BLS PWM values
    {1, 3, 7}, //BLE PWM values
    {1, 3, 7}, //BRS PWM values
    {1, 3, 7}, //BRE PWM values
    {1, 3, 7}, //MLS PWM values
    {1, 3, 7}, //MLE PWM values
    {1, 3, 7}, //MRS PWM values
    {1, 3, 7} //MRE PWM values
  },
  { //move backward
    {1, 3, 7}, //FLS PWM values
    {1, 3, 7}, //FLE PWM values
    {1, 3, 7}, //FRS PWM values
    {1, 3, 7}, //FRE PWM values
    {1, 3, 7}, //BLS PWM values
    {1, 3, 7}, //BLE PWM values
    {1, 3, 7}, //BRS PWM values
    {1, 3, 7}, //BRE PWM values
    {1, 3, 7}, //MLS PWM values
    {1, 3, 7}, //MLE PWM values
    {1, 3, 7}, //MRS PWM values
    {1, 3, 7} //MRE PWM values
  },
  { //sidestep left
    {1, 3, 7}, //FLS PWM values
    {1, 3, 7}, //FLE PWM values
    {1, 3, 7}, //FRS PWM values
    {1, 3, 7}, //FRE PWM values
    {1, 3, 7}, //BLS PWM values
    {1, 3, 7}, //BLE PWM values
    {1, 3, 7}, //BRS PWM values
    {1, 3, 7}, //BRE PWM values
    {1, 3, 7}, //MLS PWM values
    {1, 3, 7}, //MLE PWM values
    {1, 3, 7}, //MRS PWM values
    {1, 3, 7} //MRE PWM values
  },
  { //sidestep right
    {1, 3, 7}, //FLS PWM values
    {1, 3, 7}, //FLE PWM values
    {1, 3, 7}, //FRS PWM values
    {1, 3, 7}, //FRE PWM values
    {1, 3, 7}, //BLS PWM values
    {1, 3, 7}, //BLE PWM values
    {1, 3, 7}, //BRS PWM values
    {1, 3, 7}, //BRE PWM values
    {1, 3, 7}, //MLS PWM values
    {1, 3, 7}, //MLE PWM values
    {1, 3, 7}, //MRS PWM values
    {1, 3, 7} //MRE PWM values
  },
  { //turn CW
    {1, 3, 7}, //FLS PWM values
    {1, 3, 7}, //FLE PWM values
    {1, 3, 7}, //FRS PWM values
    {1, 3, 7}, //FRE PWM values
    {1, 3, 7}, //BLS PWM values
    {1, 3, 7}, //BLE PWM values
    {1, 3, 7}, //BRS PWM values
    {1, 3, 7}, //BRE PWM values
    {1, 3, 7}, //MLS PWM values
    {1, 3, 7}, //MLE PWM values
    {1, 3, 7}, //MRS PWM values
    {1, 3, 7} //MRE PWM values
  },
  { //turn CCW
    {1, 3, 7}, //FLS PWM values
    {1, 3, 7}, //FLE PWM values
    {1, 3, 7}, //FRS PWM values
    {1, 3, 7}, //FRE PWM values
    {1, 3, 7}, //BLS PWM values
    {1, 3, 7}, //BLE PWM values
    {1, 3, 7}, //BRS PWM values
    {1, 3, 7}, //BRE PWM values
    {1, 3, 7}, //MLS PWM values
    {1, 3, 7}, //MLE PWM values
    {1, 3, 7}, //MRS PWM values
    {1, 3, 7} //MRE PWM values
  },
};

unsigned int actionID = 0;
unsigned int servoID = 0;
unsigned int PWMValuesIndex = 0;
unsigned int numberOfPWMValues = sizeof(PWMValues[actionID][servoID]) / sizeof(unsigned int);

void testPWMPin(unsigned int PWMPinNumber) {
  unsigned int i = PWMMin;
  while (i < PWMMax) {
    analogWrite(PWMPinNumber, i);
    i = i + 1;
    delay(1);
  }
}

void runTests() {
  //This function runs one or more tests.
  testPWMPin(10);
}

unsigned int getActualPWMValue(unsigned int proportionalPWMValue) {
   //proportionalPWMValue- number from 0 to 100 (silently maxed out at 100)
   if (proportionalPWMValue > 100) {
    proportionalPWMValue = 100;
   }
   //output- PWM value between PWMMin and PWMMax depending on proportionalPWMValue
   return PWMMin + (PWMMax - PWMMin) * (proportionalPWMValue / 100);
}

void setup() {

  //Good resource on Teensy PWM capabilities: https://www.pjrc.com/teensy/td_pulse.html
  
  //Set all PWM pins to have freq=50Hz.
  //Why 50Hz?
  //Answer: Because we need the PWM pins operating with a 20ms period,
  //        since that's what the SG90 Servo datasheet says is the operating period.
  //Note that you couldn't set these pins to different frequencies even if you tried,
  //since some of the pins share on-board timers and therefore must have the same frequency.
  analogWriteFrequency(FLS, 50);
  analogWriteFrequency(FLE, 50);
  analogWriteFrequency(FRS, 50);
  analogWriteFrequency(FRE, 50);
  analogWriteFrequency(BLS, 50);
  analogWriteFrequency(BLE, 50);
  analogWriteFrequency(BRS, 50);
  analogWriteFrequency(BRE, 50);
  analogWriteFrequency(MLS, 50);
  analogWriteFrequency(MLE, 50);
  analogWriteFrequency(MRS, 50);
  analogWriteFrequency(MRE, 50);

  //Set the PWM resolution to 16 bits, 
  //so that the analogWrite function can accept values 
  //in the range of 0 - 65535 (2^16 - 1),
  //where 0 is 0% duty cycle and 65535 is 100% duty cycle.
  //The servos accept a duty cycle range of 5% to 10%,
  //so analogWrite values should range from 3277 to 6553.
  //These values are set to PWMMin and PWMMax, respectively.
  analogWriteResolution(16);

  //Open a serial communication line with data rate = 9600 bits per second.
  //NOTE: The Raspberry Pi sends commands and the Arduino receives them.
  //      In the future, the Arduino will send the latest distance sensor values
  //      back to the Raspberry Pi.
  //NOTE: Make sure that the Raspberry Pi has a matching bit transmission rate,
  //      so that the Arduino is receiving new bits at the same rate that the 
  //      Raspberry Pi is sending them and vise versa.
  Serial.begin(9600);

  //Run all the tests.
  //Comment this out in production.
  runTests();
  
  Serial.println("setup done");
}

void loop() {

  //Get the current actionID.
  if (Serial.available()) {
    char ch = Serial.read();
    //Here, the Arduino reads serial USB input, 
    //which will be commands (in the form of characters) coming from the RaspberryPi.
    //To test these serial inputs out on a computer, use Tools->Serial Monitor.
    if (ch == 'q') {
      Serial.println("Received 'stand up' command");
      actionID = 0;
    }
    else if (ch == 'w') {
      Serial.println("Received 'move forward' command");
      actionID = 1;
    }
    else if (ch == 's') {
      Serial.println("Received 'move backward' command");
      actionID = 2;
    }
    else if (ch == 'a') {
      Serial.println("Received 'sidestep left' command");
      actionID = 3;
    }
    else if (ch == 'd') {
      Serial.println("Received 'sidestep right' command");
      actionID = 4;
    }
    else if (ch == 'c') {
      Serial.println("Received 'turn CW' command");
      actionID = 5;
    }
    else if (ch == 'v') {
      Serial.println("Received 'turn CCW' command");
      actionID = 6;
    }
    //Reset the PWMValuesIndex to zero.
    PWMValuesIndex = 0;
    //Get the numberOfPWMValues.
    //Assume that all of the arrays in PWMValues[actionID] are the same length,
    //so just get the length of the first array.
    numberOfPWMValues = sizeof(PWMValues[actionID][0]) / sizeof(unsigned int);
  }

  //For every servo, actuate.
  for (servoID = 0; servoID < 12; servoID++) {
    //Get the proportional (between 0 and 100) PWM value
    unsigned int proportionalPWMValue = PWMValues[actionID][servoID][PWMValuesIndex];
    //Get the actual PWM value
    unsigned int PWMValue = getActualPWMValue(proportionalPWMValue);
    //Get the servo pin number
    unsigned int servoPinNumber = servoPinNumbers[servoID];
    //Actuate the servo with the PWM value
    analogWrite(servoPinNumber, PWMValue);
  }

  //Increment PWMValuesIndex
  PWMValuesIndex++;
  //Make sure the PWMValuesIndex doesn't go past the end of the array of PWM values
  PWMValuesIndex %= numberOfPWMValues;
  
  //Have a small delay before the next call to loop()
  delay(2);
}

void turnOn() {
  // Assumes a folded position of elbows at 700, shoulders at 0.
  for (int i=0; i < 100; i++) {
    delay(20); // Whole process takes 2 seconds: 100 * 20ms = 2000ms
    analogWrite(FRS, translateBounds("FRS", i * 8));
    analogWrite(FLS, translateBounds("FLS", i * 8));
    analogWrite(BRS, translateBounds("BRS", i * 3));
    analogWrite(BLS, translateBounds("BLS", i * 3));
    analogWrite(MRS, translateBounds("MRS", i * 6));
    analogWrite(MLS, translateBounds("MLS", i * 6));
  }
  for (int i=700; i > 200; i--) {
    delay(4); // Whole process takes 2 seconds: 500 * 4ms = 2000ms
    analogWrite(FRE, translateBounds("FRE", i));
    analogWrite(FLE, translateBounds("FLE", i));
    analogWrite(BRE, translateBounds("BRE", i));
    analogWrite(BLE, translateBounds("BLE", i));
    analogWrite(MRE, translateBounds("MRE", i));
    analogWrite(MLE, translateBounds("MLE", i));
  }
}

void turnOff() {
  // Assumes a standing position of elbows at 200.

  for (int i=200; i < 700; i++) {
    delay(4); // Whole process takes 2 seconds: 500 * 4ms = 2000ms
    analogWrite(FRE, translateBounds("FRE", i));
    analogWrite(FLE, translateBounds("FLE", i));
    analogWrite(BRE, translateBounds("BRE", i));
    analogWrite(BLE, translateBounds("BLE", i));
    analogWrite(MRE, translateBounds("MRE", i));
    analogWrite(MLE, translateBounds("MLE", i));
  }
  for (int i=100; i > 0; i--) {
    delay(20); // Whole process takes 2 seconds: 100 * 20ms = 2000ms
    analogWrite(FRS, translateBounds("FRS", i * 8));
    analogWrite(FLS, translateBounds("FLS", i * 8));
    analogWrite(BRS, translateBounds("BRS", i * 3));
    analogWrite(BLS, translateBounds("BLS", i * 3));
    analogWrite(MRS, translateBounds("MRS", i * 6));
    analogWrite(MLS, translateBounds("MLS", i * 6));
  }
  //resetFunc(); //call reset 
}


unsigned int translateBounds(String servo, unsigned int value) {
  // This function maps input values from 0-1000 to the corresponding ranges for each servo, so they are 
  // calibrated to each other and none of them goes out of range and damages the robot. 
  // 0 for elbows means pushed all the way down, and 0 for shoulders means pushed all the way back.
  // 1000 for elbows means pushed all the way up (legs in the air), and 1000 for shoulders means pushed all the way forward.
  //
  // So, moving all elbows down together might look like this:
  //
  // unsigned int allElbows = 100; 
  //
  // analogWrite(FRE, translateBounds("FRE", allElbows));
  // analogWrite(FLE, translateBounds("FLE", allElbows));
  // analogWrite(BRE, translateBounds("BRE", allElbows));
  // analogWrite(BLE, translateBounds("BLE", allElbows));
  // analogWrite(MRE, translateBounds("MRE", allElbows));
  // analogWrite(MLE, translateBounds("MLE", allElbows));
  int index = -1;
  String servoNames[12] = {"FLS", "FLE", "FRS", "FRE", "BLS", "BLE", "BRS", "BRE", "MLS", "MLE", "MRS", "MRE"};
  for (int i = 0; i < 12; i++) {
    if (servo == servoNames[i]) {
      index = i; 
    }
  }
  if (index == -1) {
    return 0;
  }
  unsigned int translationTable[12][2] = {{8600, 2200},{1800, 5500},{1790, 7700},{7750, 2750},{8650, 3800},{1950, 6100},{2000, 6500},{8200, 3700},{8200, 3800},{1750, 5600},{1450, 4500},{8500, 3200}};
  
  if (translationTable[index][0] < translationTable[index][1]) {
    return (unsigned int) ((translationTable[index][1] - translationTable[index][0]) * value / 1000) + translationTable[index][0];
  } else {
    return (unsigned int) translationTable[index][0] - ((translationTable[index][0] - translationTable[index][1]) * value / 1000);
  }
}
