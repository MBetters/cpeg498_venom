////////////////////////////////////////////////////////////////
//SECTION: Pin-Number-Related and Name-Related Global Variables
////////////////////////////////////////////////////////////////

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

// Possible pins for each force sensor: 14, 15, 16, 17, 18, 19
unsigned int fsrFLL = 14; // FSR for Front Left Leg
unsigned int fsrMLL = 15; // Middle Left Leg
unsigned int fsrBLL = 16; // Back Left Leg 
unsigned int fsrFRL = 17; // Front Right Leg
unsigned int fsrMRL = 18; // Middle Right Leg
unsigned int fsrBRL = 19; // Back Right Leg

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

String legNames[6] = {"FL", "FR",
                      "ML", "MR",
                      "BL", "BR"};

//////////////////////////////////////////
//SECTION: State-Related Global Variables
//////////////////////////////////////////

enum LEG_STATE {
  UP_FORWARD,
  UP_BACKWARD,
  DOWN_FORWARD,
  DOWN_BACKWARD
};

LEG_STATE** spiderStates;
unsigned int spiderStatesIndex = 0;
unsigned int numberOfSpiderStates = 0;

LEG_STATE* currentSpiderState;
LEG_STATE* nextSpiderState;

LEG_STATE spiderStates_STAND[1][6] = {{DOWN_FORWARD, DOWN_FORWARD, 
                                       DOWN_FORWARD, DOWN_FORWARD, 
                                       DOWN_BACKWARD, DOWN_BACKWARD}};

LEG_STATE spiderStates_WALK_FORWARD[6][6] = {{DOWN_FORWARD, DOWN_BACKWARD, //stand (all legs are down) (also acts as lowering the second leg-triangle when "wrapping around" this array)
                                              DOWN_BACKWARD, DOWN_FORWARD, 
                                              DOWN_FORWARD, DOWN_BACKWARD},
                                             {DOWN_FORWARD, UP_BACKWARD, //lift first leg-triangle (FR, ML, BR)
                                              UP_BACKWARD, DOWN_FORWARD, 
                                              DOWN_FORWARD, UP_BACKWARD},
                                             {DOWN_BACKWARD, UP_FORWARD, //rotate leg-triangles (moves the spider forward)
                                              UP_FORWARD, DOWN_BACKWARD, 
                                              DOWN_BACKWARD, UP_FORWARD},
                                             {DOWN_BACKWARD, DOWN_FORWARD, //lower first leg-triangle (FR, ML, BR), going to standing position
                                              DOWN_FORWARD, DOWN_BACKWARD, 
                                              DOWN_BACKWARD, DOWN_FORWARD},
                                           //Mirror states below...
                                             {UP_BACKWARD, DOWN_FORWARD, //lift second leg-triangle (FL, MR, BL)
                                              DOWN_FORWARD, UP_BACKWARD, 
                                              UP_BACKWARD, DOWN_FORWARD},
                                             {UP_FORWARD, DOWN_BACKWARD, //rotate leg-triangles (moves the spider forward)
                                              DOWN_BACKWARD, UP_FORWARD,
                                              UP_FORWARD, DOWN_BACKWARD}};

////////////////////////////////////////
//SECTION: PWM-Related Global Variables
////////////////////////////////////////

unsigned int servoPWMValueBounds[12][2] = {{8600, 2200}, {1800, 5500},
                                           {1790, 7700}, {7750, 2750},
                                           {8650, 3800}, {1950, 6100},
                                           {2000, 6500}, {8200, 3700},
                                           {8200, 3800}, {1750, 5600},
                                           {1450, 4500}, {8500, 3200}};

unsigned int PWMMin = 3277;
unsigned int PWMMax = 6553;

//Denotes current, actual PWM output, NOT the desired postion it's moving toward. Begins at standing position.
float currentPos[12] = {800, 200, 
                        800, 200, 
                        300, 200, 
                        300, 200,
                        600, 200,
                        600, 200};

// Desired PWM position, which currentPosition slowly moves towards over many loop() cycles.
unsigned int desiredPos[12] = {800, 200, 
                               800, 200, 
                               300, 200, 
                               300, 200,
                               600, 200,
                               600, 200};

// Updated every new step in a sequence- array is used to know total distance from start to finish for each servo,
// so that they can run at the speed required for them to all finish at the same time.
unsigned int totalDistance[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Needed to compensate for loss of precision throughout movement increments.
float remainders[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

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

  //Get the current action.
  if (Serial.available()) {
    char ch = Serial.read();
    //Here, the Arduino reads serial USB input, 
    //which will be commands (in the form of characters) coming from the RaspberryPi.
    //To test these serial inputs out on a computer, use Tools->Serial Monitor.
    if (ch == 'q') {
      Serial.println("Received 'stand up' command");
      spiderStates = (LEG_STATE**) spiderStates_STAND;
      numberOfSpiderStates = 1;
    }
    else if (ch == 'w') {
      Serial.println("Received 'move forward' command");
      spiderStates = (LEG_STATE**) spiderStates_WALK_FORWARD;
      numberOfSpiderStates = 6;
    }
    else if (ch == 's') {
      Serial.println("Received 'move backward' command");
    }
    else if (ch == 'a') {
      Serial.println("Received 'sidestep left' command");
    }
    else if (ch == 'd') {
      Serial.println("Received 'sidestep right' command");
    }
    else if (ch == 'c') {
      Serial.println("Received 'turn CW' command");
    }
    else if (ch == 'v') {
      Serial.println("Received 'turn CCW' command");
    }
    else if (ch == 'x') {
      turnOff();
    }
    //Reset the PWMValuesIndex to zero.
    spiderStatesIndex = 0;
  }

  //If the spiderStatesIndex hasn't reached the end of the 2D spiderStates array,
  //then keep running the action described by spiderStates.
  if (spiderStatesIndex < numberOfSpiderStates) {
    //Move every leg
    bool nextLegStatesHaveBeenReached[6] = {false, false, false, false, false, false};
    unsigned int legIndex = 0;
    for (legIndex = 0; legIndex < 6; legIndex++) {
      LEG_STATE currentLegState = currentSpiderState[legIndex];
      LEG_STATE nextLegState = nextSpiderState[legIndex];
      bool nextLegStateHasBeenReached = moveLeg(legIndex, currentLegState, nextLegState);
      nextLegStatesHaveBeenReached[legIndex] = nextLegStateHasBeenReached;
    }

    bool nextSpiderStateHasBeenReached = true;
    for (legIndex = 0; legIndex < 6; legIndex++) {
      if (nextLegStatesHaveBeenReached[legIndex] == false) {
        nextSpiderStateHasBeenReached = false;
        break;
      }
    }

    if (nextSpiderStateHasBeenReached) {
      //The currentSpiderState has successfully reached the nextSpiderState, 
      //so assign the nextSpiderState to the currentSpiderState
      currentSpiderState = nextSpiderState;
      //Increment spiderStatesIndex (to point to the next spider state)
      spiderStatesIndex++;
      //Get the nextSpiderState
      nextSpiderState = spiderStates[spiderStatesIndex];

      //Recalculate the currentPos, desiredPos, and totalDistance arrays
      //according to the currentSpiderState and nextSpiderState.
      recalculatePWMArrays(currentSpiderState, nextSpiderState);
    }

    //Have a small delay before the next call to loop()
    delay(2);
  }

  getSensorRead(fsrFLL); // Gets sensor reading for FLL and outputs its respective value.
  getSensorRead(fsrMRL);
  getSensorRead(fsrBRL);
  getSensorRead(fsrFRL);
  getSensorRead(fsrMRL);
  getSensorRead(fsrBRL);
}

///////////////////////////////////////////
//SECTION: turnOn() and turnOff() Functions
///////////////////////////////////////////
void turnOn() {
  // Assumes a folded position of elbows at 700, shoulders at 0.
  for (int i=0; i < 100; i++) {
    delay(20); // Whole process takes 2 seconds: 100 * 20ms = 2000ms
    analogWrite(FLS, translateBounds(0, i * 8));
    analogWrite(FRS, translateBounds(2, i * 8));
    analogWrite(BLS, translateBounds(4, i * 3));
    analogWrite(BRS, translateBounds(6, i * 3));
    analogWrite(MLS, translateBounds(8, i * 6));
    analogWrite(MRS, translateBounds(10, i * 6));
  }
  for (int i=700; i > 200; i--) {
    delay(4); // Whole process takes 2 seconds: 500 * 4ms = 2000ms
    analogWrite(FLE, translateBounds(1, i));
    analogWrite(FRE, translateBounds(3, i));
    analogWrite(BLE, translateBounds(5, i));
    analogWrite(BRE, translateBounds(7, i));
    analogWrite(MLE, translateBounds(9, i));
    analogWrite(MRE, translateBounds(11, i));
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

/////////////////////////////
//SECTION: moveLeg() Function
/////////////////////////////
bool moveLeg(unsigned int legIndex, LEG_STATE currentState, LEG_STATE nextState) {
  bool nextLegStateHasBeenReached = false;
  
  String legName = legNames[legIndex];
  String currentStateName = getStateName(currentState);
  String nextStateName = getStateName(nextState);
  Serial.println("Moving the " + legName + " leg from the " + currentStateName + " state to the " + nextStateName + " state.");

  unsigned int shoulderServoIndex = getShoulderServoIndex(legName);
  unsigned int elbowServoIndex = getElbowServoIndex(legName);

  //TODO: Implement the bodies of these conditionals.
  if (currentState == UP_FORWARD && nextState == UP_BACKWARD) {
    //Interpolate and move the leg accordingly
    PWMIncrement(shoulderServoPinNumber);
  } else if (currentState == UP_FORWARD && nextState == DOWN_FORWARD) {
    //check force sensor...
    //then if force sensor is high, do nothing
    //else, move the leg by a constant tiny bit
  } else if (currentState == UP_FORWARD && nextState == DOWN_BACKWARD) {
    
  } else if (currentState == UP_BACKWARD && nextState == UP_FORWARD) {
    
  } else if (currentState == UP_BACKWARD && nextState == DOWN_FORWARD) {
    
  } else if (currentState == UP_BACKWARD && nextState == DOWN_BACKWARD) {
    
  } else if (currentState == DOWN_FORWARD && nextState == DOWN_BACKWARD) {
    
  } else if (currentState == DOWN_FORWARD && nextState == UP_FORWARD) {
    
  } else if (currentState == DOWN_FORWARD && nextState == UP_BACKWARD) {
    
  } else if (currentState == DOWN_BACKWARD && nextState == DOWN_FORWARD) {
    
  } else if (currentState == DOWN_BACKWARD && nextState == UP_FORWARD) {
    
  } else if (currentState == DOWN_BACKWARD && nextState == UP_BACKWARD) {
    
  }

  return nextLegStateHasBeenReached;
}

//////////////////////////////
//SECTION: Interpolation Code
//////////////////////////////
void PWMIncrement(unsigned int servoIndex) {
  // Takes an incremental step towards the 'desiredPos' array, updating the analog PWM output and the currentPos array to reflect that.
  //Find the distance required for the servo to move 1/n of the way to the destination.
  float stepSize = ((float) totalDistance[servoIndex]) / 100;
  //Add (or implicitly subtract) the step size from the current servo position
  float nextProportionalPWMValue = currentPos[servoIndex] + stepSize;
  // Near the end, numbers may need to be rounded so they perfectly end up where they should
  if (abs(nextProportionalPWMValue - desiredPos[servoIndex]) < abs(stepSize)) {
    nextProportionalPWMValue = desiredPos[servoIndex];
  }
  //Get the actual PWM value
  unsigned int PWMValue = translateBounds(servoIndex, (int) nextProportionalPWMValue);
  //Update what our current PWM output is for this servo
  currentPos[servoIndex] = nextProportionalPWMValue;
  //Get the servo pin number
  unsigned int servoPinNumber = servoPinNumbers[servoIndex];
  //Actuate the servo with the PWM value
  analogWrite(servoPinNumber, PWMValue);
}

////////////////////////////
//SECTION: Helper Functions
////////////////////////////
String getStateName(LEG_STATE state) {
  if (state == UP_FORWARD) {
    return "UP_FORWARD";
  } else if (state == UP_BACKWARD) {
    return "UP_FORWARD";
  } else if (state == DOWN_FORWARD) {
    return "DOWN_FORWARD";
  } else if (state == DOWN_BACKWARD) {
    return "DOWN_BACKWARD";
  }
}
                                  
unsigned int getShoulderServoIndex(String legName) {
  if (legName == "FL") {
    return 0;
  } else if (legName == "FR") {
    return 2;
  } else if (legName == "BL") {
    return 4;
  } else if (legName == "BR") {
    return 6;
  } else if (legName == "ML") {
    return 8;
  } else if (legName == "MR") {
    return 10;
  }
  return 0; //return zero by default
}

unsigned int getElbowServoIndex(String legName) {
  if (legName == "FL") {
    return 1;
  } else if (legName == "FR") {
    return 3;
  } else if (legName == "BL") {
    return 5;
  } else if (legName == "BR") {
    return 7;
  } else if (legName == "ML") {
    return 9;
  } else if (legName == "MR") {
    return 11;
  }
  return 0; //return zero by default
}

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
  // analogWrite(FRE, translateBounds("FRE", allElbows));
  // analogWrite(FLE, translateBounds("FLE", allElbows));
  // analogWrite(BRE, translateBounds("BRE", allElbows));
  // analogWrite(BLE, translateBounds("BLE", allElbows));
  // analogWrite(MRE, translateBounds("MRE", allElbows));
  // analogWrite(MLE, translateBounds("MLE", allElbows));
  
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

void recalculatePWMArrays(LEG_STATE* currentSpiderState, LEG_STATE* nextSpiderState) {
  //Proportional PWM Notes:
  //    All zeroes means all back and down
  //    All thousands means all forward and up

  //Change currentPos according to currentSpiderState
  unsigned int currentPosIndex = 0;
  unsigned int currentStateIndex = 0;
  for (currentPosIndex = 0; currentPosIndex < 12; currentPosIndex++) {
    LEG_STATE currentLegState = currentSpiderState[currentStateIndex];
    if (currentPosIndex % 2 == 0) {
      currentStateIndex++;
    }
    unsigned int pos = 0;
    switch (currentLegState) {
      case UP_FORWARD:
        if (currentPosIndex % 2 == 0) { //shoulder
          
        } else { //elbow
          
        }
      case UP_BACKWARD: return "UP_BACKWARD";
      case DOWN_FORWARD: return "DOWN_FORWARD";
      case DOWN_BACKWARD: return "DOWN_BACKWARD";
    }
    currentPos[currentPosIndex] = pos;
  }
  return;
}

////////////////////////
//SECTION: Force Sensors
////////////////////////
unsigned int getSensorRead(unsigned int SensorPinNumber)
{
  if (SensorPinNumber == 14)
  {
    Serial.print("FLL reading = ");
    Serial.println(analogRead(SensorPinNumber));
  }
  else if (SensorPinNumber == 15)
  {
    Serial.print("MLL reading = ");
    Serial.println(analogRead(SensorPinNumber));
  }
  else if (SensorPinNumber == 16)
  {
    Serial.print("BLL reading = ");
    Serial.println(analogRead(SensorPinNumber));
  }
  else if (SensorPinNumber == 17)
  {
    Serial.print("FRL reading = ");
    Serial.println(analogRead(SensorPinNumber));
  }
  else if (SensorPinNumber == 18)
  {
    Serial.print("MRL reading = ");
    Serial.println(analogRead(SensorPinNumber));
  }
  else if (SensorPinNumber == 19)
  {
    Serial.print("BRL reading = ");
    Serial.println(analogRead(SensorPinNumber));
  }
  
}

/////////////////
//SECTION: Tests
/////////////////
void runTests() {
  //This function runs one or more tests.

  //Test the servos' operations
  testPWMPin(FLS);
  testPWMPin(FLE);
  testPWMPin(FRS);
  testPWMPin(FRE);
  testPWMPin(BLS);
  testPWMPin(BLE);
  testPWMPin(BRS);
  testPWMPin(BRE);
  testPWMPin(MLS);
  testPWMPin(MLE);
  testPWMPin(MRS);
  testPWMPin(MRE);
}

void testPWMPin(unsigned int PWMPinNumber) {
  unsigned int i = PWMMin;
  while (i < PWMMax) {
    analogWrite(PWMPinNumber, i);
    i = i + 1;
    delay(1);
  }
}
