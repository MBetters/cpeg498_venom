///////////////////////////////////////
//SECTION: Global Variable Definitions
///////////////////////////////////////
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

String servoNames[] = {"FLS", "FLE", 
                       "FRS", "FRE",
                       "BLS", "BLE",
                       "BRS", "BRE",
                       "MLS", "MLE",
                       "MRS", "MRE"};

unsigned int servoPWMValueBounds[12][2] = {{8600, 2200},
                                           {1800, 5500},
                                           {1790, 7700},
                                           {7750, 2750},
                                           {8650, 3800},
                                           {1950, 6100},
                                           {2000, 6500},
                                           {8200, 3700},
                                           {8200, 3800},
                                           {1750, 5600},
                                           {1450, 4500},
                                           {8500, 3200}};
  

unsigned int PWMMin = 3277;
unsigned int PWMMax = 6553;

unsigned int actionID = 0;
unsigned int servoID = 0;

unsigned int** PWMValues;
unsigned int PWMValuesIndex = 0;
unsigned int numberOfPWMValues = 0;

unsigned int PWMValuesStand[12][1] = {
  {800},
  {200},
  {800},
  {200},
  {300},
  {200},
  {300},
  {200},
  {600},
  {200},
  {600},
  {200}
};

////////////////////////////////////////
//SECTION: setup() and loop() Functions
////////////////////////////////////////
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
  //runTests(); //Comment this out in production, so that it doesn't run.
  
  turnOn();
  
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
      PWMValues = (unsigned int**) PWMValuesStand;
      numberOfPWMValues = 1;
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
    else if (ch == 'x') {
      turnOff();
    }
    //Reset the PWMValuesIndex to zero.
    PWMValuesIndex = 0;
  }

  //If the PWMValuesIndex hasn't reached the end of the 2D action array,
  //then keep running the action.
  if (PWMValuesIndex < numberOfPWMValues) {
    //Actuate every servo
    for (servoID = 0; servoID < 12; servoID++) {
      //Get the proportional (between 0 and 1000) PWM value
      unsigned int proportionalPWMValue = PWMValues[servoID][PWMValuesIndex];
      //Get the servo name
      String servoName = servoNames[servoID];
      //Get the actual PWM value
      unsigned int PWMValue = translateBounds(servoID, proportionalPWMValue);
      //Get the servo pin number
      unsigned int servoPinNumber = servoPinNumbers[servoID];
      //Actuate the servo with the PWM value
      analogWrite(servoPinNumber, PWMValue);
    }
  
    //Increment PWMValuesIndex
    PWMValuesIndex++;

    //Have a small delay before the next call to loop()
    delay(2);
  }
  
}

//////////////////////////////
//SECTION: Movement Functions
//////////////////////////////
void turnOn() {
  // Assumes a folded position of elbows at 700, shoulders at 0.
  for (int i=0; i < 100; i++) {
    delay(20); // Whole process takes 2 seconds: 100 * 20ms = 2000ms
    analogWrite(FRS, translateBounds(2, i * 8));
    analogWrite(FLS, translateBounds(0, i * 8));
    analogWrite(BRS, translateBounds(6, i * 3));
    analogWrite(BLS, translateBounds(4, i * 3));
    analogWrite(MRS, translateBounds(10, i * 6));
    analogWrite(MLS, translateBounds(8, i * 6));
  }
  for (int i=700; i > 200; i--) {
    delay(4); // Whole process takes 2 seconds: 500 * 4ms = 2000ms
    analogWrite(FRE, translateBounds(3, i));
    analogWrite(FLE, translateBounds(1, i));
    analogWrite(BRE, translateBounds(7, i));
    analogWrite(BLE, translateBounds(5, i));
    analogWrite(MRE, translateBounds(11, i));
    analogWrite(MLE, translateBounds(9, i));
  }
}

void turnOff() {
  // Assumes a standing position of elbows at 200.
  for (int i=200; i < 700; i++) {
    delay(4); // Whole process takes 2 seconds: 500 * 4ms = 2000ms
    analogWrite(FRE, translateBounds(3, i));
    analogWrite(FLE, translateBounds(1, i));
    analogWrite(BRE, translateBounds(7, i));
    analogWrite(BLE, translateBounds(5, i));
    analogWrite(MRE, translateBounds(11, i));
    analogWrite(MLE, translateBounds(9, i));
  }
  for (int i=100; i > 0; i--) {
    delay(20); // Whole process takes 2 seconds: 100 * 20ms = 2000ms
    analogWrite(FRS, translateBounds(2, i * 8));
    analogWrite(FLS, translateBounds(0, i * 8));
    analogWrite(BRS, translateBounds(6, i * 3));
    analogWrite(BLS, translateBounds(4, i * 3));
    analogWrite(MRS, translateBounds(10, i * 6));
    analogWrite(MLS, translateBounds(8, i * 6));
  }
  //resetFunc(); //call reset 
}

////////////////////////////
//SECTION: Helper Functions
////////////////////////////
unsigned int translateBounds(unsigned int servoIndex, unsigned int value) {
  // This function maps input values from 0-1000 to the corresponding ranges for each servo, so they are 
  // calibrated to each other and none of them goes out of range and damages the robot. 
  // 0 for elbows means pushed all the way down, and 0 for shoulders means pushed all the way back.
  // 1000 for elbows means pushed all the way up (legs in the air), and 1000 for shoulders means pushed all the way forward.
  //
  // So, moving all elbows down together might look like this:
  //
  // unsigned int allElbows = 100;
  //
  // analogWrite(FRE, servoPWMValueBounds("FRE", allElbows));
  // analogWrite(FLE, servoPWMValueBounds("FLE", allElbows));
  // analogWrite(BRE, servoPWMValueBounds("BRE", allElbows));
  // analogWrite(BLE, servoPWMValueBounds("BLE", allElbows));
  // analogWrite(MRE, servoPWMValueBounds("MRE", allElbows));
  // analogWrite(MLE, servoPWMValueBounds("MLE", allElbows));
  
  if (servoPWMValueBounds[servoIndex][0] < servoPWMValueBounds[servoIndex][1]) {
    return (unsigned int) ((servoPWMValueBounds[servoIndex][1]
                          - servoPWMValueBounds[servoIndex][0]) * value / 1000)
                          + servoPWMValueBounds[servoIndex][0];
  } else {
    return (unsigned int) servoPWMValueBounds[servoIndex][0] 
                          - ((servoPWMValueBounds[servoIndex][0]
                            - servoPWMValueBounds[servoIndex][1]) * value / 1000);
  }
}

/////////////////
//SECTION: Tests
/////////////////
void runTests() {
  //This function runs one or more tests.
  testPWMPin(10);
}

void testPWMPin(unsigned int PWMPinNumber) {
  unsigned int i = PWMMin;
  while (i < PWMMax) {
    analogWrite(PWMPinNumber, i);
    i = i + 1;
    delay(1);
  }
}
