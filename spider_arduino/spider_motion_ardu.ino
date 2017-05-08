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

// Denotes current, actual PWM output, NOT the desired postion it's moving toward. Begins at standing position.
float currentPos[12] = {800, 200, 
                        800, 200, 
                        300, 200, 
                        300, 200,
                        600, 200,
                        600, 200};

// Desired PWM position, which currentPosition slowly moves towards over a few loop() cycles.
int desiredPos[12] = {800, 200, 
                      800, 200, 
                      300, 200, 
                      300, 200,
                      600, 200,
                      600, 200};

// Updated every new step in a sequence- array is used to know total distance from start to finish for each servo,
// so that they can run at the speed required for them to all finish at the same time.
int totalDistance[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Needed to compensate for loss of precision throughout movement increments.
float remainders[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

// Variable that keeps track of what the 'lowest' point is for the spider, to know that if a knee is low enough it 'should' be touching the ground.
unsigned int lowPoint = 0;
unsigned int SPEED = 0;

unsigned int servoPWMValueBounds[12][2] = {{8600, 2200},
                                           {1660, 5500},
                                           {1790, 7700},
                                           {7410, 2750},
                                           {8500, 3800},
                                           {1970, 6100},
                                           {2000, 6500},
                                           {8080, 3700},
                                           {8000, 3800},
                                           {1680, 5600},
                                           {1450, 4500},
                                           {8450, 3000}};

unsigned int PWMMin = 3277;
unsigned int PWMMax = 6553;

unsigned int servoID = 0;

unsigned int* PWMValues[12];
unsigned int PWMValuesIndex = 0;
unsigned int numberOfPWMValues = 0;

unsigned int DOWN = 200;
unsigned int UP = 500;
unsigned int FORWARD[3] = {700, 450, 700};
unsigned int CENTEr[3] = {600, 350, 600};
unsigned int BACK[3] = {500, 250, 500};

unsigned int STAND_ARRAY_LENGTH = 1;
unsigned int STAND_SPEED = 100;
unsigned int PWMValuesStand[12][1] = {
  {CENTEr[0]},
  {DOWN     },
  {CENTEr[0]},
  {DOWN     },
  {CENTEr[1]},
  {DOWN     },
  {CENTEr[1]},
  {DOWN     },
  {CENTEr[2]},
  {DOWN     },
  {CENTEr[2]},
  {DOWN     }
};

unsigned int CROUCH_ARRAY_LENGTH = 1;
unsigned int CROUCH_SPEED = 100;
unsigned int PWMValuesCrouch[12][1] = {
  {CENTEr[0]},
  {UP       },
  {CENTEr[0]},
  {UP       },
  {CENTEr[1]},
  {UP       },
  {CENTEr[1]},
  {UP       },
  {CENTEr[2]},
  {UP       },
  {CENTEr[2]},
  {UP       }
};

unsigned int WALK_SPEED = 100;
unsigned int WALK_ARRAY_LENGTH = 6;
unsigned int PWMValuesWalk[12][6] = {
  {FORWARD[0], FORWARD[0], FORWARD[0], BACK[0]   , BACK[0]   , BACK[0]   }, //FLS >>><<<
  {UP        , DOWN      , DOWN      , DOWN      , DOWN      , UP        }, //FLE -____-
  {BACK[0]   , BACK[0]   , BACK[0]   , FORWARD[0], FORWARD[0], FORWARD[0]}, //FRS <<<>>>
  {DOWN      , DOWN      , UP        , UP        , DOWN      , DOWN      }, //FRE __--__
  {FORWARD[1], FORWARD[1], FORWARD[1], BACK[1]   , BACK[1]   , BACK[1]   }, //BLS >>><<<
  {UP        , DOWN      , DOWN      , DOWN      , DOWN      , UP        }, //BLE -____-
  {BACK[1]   , BACK[1]   , BACK[1]   , FORWARD[1], FORWARD[1], FORWARD[1]}, //BRS <<<>>>
  {DOWN      , DOWN      , UP        , UP        , DOWN      , DOWN      }, //BRE __--__
  {BACK[2]   , BACK[2]   , BACK[2]   , FORWARD[2], FORWARD[2], FORWARD[2]}, //MLS <<<>>>
  {DOWN      , DOWN      , UP        , UP        , DOWN      , DOWN      }, //MLE __--__
  {FORWARD[2], FORWARD[2], FORWARD[2], BACK[2]   , BACK[2]   , BACK[2]   }, //MRS >>><<<
  {UP        , DOWN      , DOWN      , DOWN      , DOWN      , UP        }  //MRE -____-
};

unsigned int JUMP_ARRAY_LENGTH = 7;
unsigned int JUMP_SPEED = 100;
unsigned int PWMValuesJump[12][7] = {
  {800, 800, 800, 800, 800, 800, 800}, //FLS
  {200, 300, 400, 500, 600, 100, 200}, //FLE
  {800, 800, 800, 800, 800, 800, 800}, //FRS
  {200, 300, 400, 500, 600, 100, 400}, //FRE
  {300, 300, 300, 300, 300, 300, 300}, //BLS
  {200, 300, 400, 500, 600, 100, 200}, //BLE
  {300, 300, 300, 300, 300, 300, 300}, //BRS
  {200, 300, 400, 500, 600, 100, 400}, //BRE
  {600, 600, 600, 600, 600, 600, 600}, //MLS
  {200, 300, 400, 500, 600, 100, 300}, //MLE
  {600, 600, 600, 600, 600, 600, 600}, //MRS
  {200, 300, 400, 500, 600, 100, 300}  //MRE
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
  
  //turnOn();
  //TO START OUT WALKING:
  //for (int i=0; i < 12; i++) PWMValues[i] = PWMValuesWalk[i];
  //    numberOfPWMValues = WALK_ARRAY_LENGTH;
  //    SPEED = WALK_SPEED;
  
  Serial.println("setup done");
}

void loop() {

  //Get the current action.
  if (Serial.available()) {
    char ch = Serial.read();
    //Here, the Arduino reads serial USB input, 
    //which will be commands (in the form of characters) coming from the RaspberryPi.
    //To test these serial inputs out on a computer, use Tools->Serial Monitor.
    if (ch == 's') {
      Serial.println("Received 'stand up' command");
      for (int i=0; i < 12; i++) PWMValues[i] = PWMValuesStand[i];
      numberOfPWMValues = STAND_ARRAY_LENGTH;
      SPEED = STAND_SPEED;
    }
    else if (ch == 'w') {
      Serial.println("Received 'move forward' command");
      for (int i=0; i < 12; i++) PWMValues[i] = PWMValuesWalk[i];
      numberOfPWMValues = WALK_ARRAY_LENGTH;
      SPEED = WALK_SPEED;
    }
    else if (ch == 'j') {
      Serial.println("Received 'jump' command");
      for (int i=0; i < 12; i++) PWMValues[i] = PWMValuesJump[i];
      numberOfPWMValues = JUMP_ARRAY_LENGTH;
      SPEED = JUMP_SPEED;
    }
    else if (ch == 'c') {
      Serial.println("Received 'crouch' command");
      for (int i=0; i < 12; i++) PWMValues[i] = PWMValuesCrouch[i];
      numberOfPWMValues = CROUCH_ARRAY_LENGTH;
      SPEED = CROUCH_SPEED;
    }
    //Reset the PWMValuesIndex to zero.
    PWMValuesIndex = 0;
  }

  //If the PWMValuesIndex hasn't reached the end of the 2D action array,
  //then keep running the action.
  if (PWMValuesIndex < numberOfPWMValues && compareArray(currentPos, desiredPos)) {
    // Reset lowPoint
    lowPoint = 1000;
    Serial.print("Next step: ");
    Serial.println(PWMValuesIndex);
    //Update desiredPos for every servo
    for (servoID = 0; servoID < 12; servoID++) {
      //Get the proportional (between 0 and 1000) PWM value
      int proportionalPWMValue = PWMValues[servoID][PWMValuesIndex];
      // Update with the start-to-finish distance for the servo.
      totalDistance[servoID] = proportionalPWMValue - currentPos[servoID];
      //Actuate the servo with the PWM value
      desiredPos[servoID] = proportionalPWMValue;
      //Update lowPoint variable to reevaluate the lowest elbow value for the next step
      if (servoID % 2 == 1 && lowPoint > proportionalPWMValue) lowPoint = proportionalPWMValue;
    }
    //Increment PWMValuesIndex
    PWMValuesIndex++;
  } else if (!compareArray(currentPos, desiredPos)) {
    PWMIncrement();
    //Have a small delay before the next call to loop()
    delay(4);
  } else {
    //If the end of the action sequence has been reached, start over and repeat until new command is received.
    PWMValuesIndex = 0;
  }
}

//////////////////////////////
//SECTION: Movement Functions
//////////////////////////////

void PWMIncrement() {
  int val = analogRead(0);
  Serial.print("analog 0 is: ");
  Serial.println(val);
  // Takes an incremental step towards the 'desiredPos' array, updating the analog PWM output and the currentPos array to reflect that.
  for (servoID = 0; servoID < 12; servoID++) {
    //Find the distance required for the servo to move 1/n of the way to the destination.
    float stepSize = ((float) totalDistance[servoID]) / SPEED;
    //Add (or implicitly subtract) the step size from the current servo position
    float nextProportionalPWMValue = currentPos[servoID] + stepSize;
    // Near the end, numbers may need to be rounded so they perfectly end up where they should
    if (abs(nextProportionalPWMValue - desiredPos[servoID]) < abs(stepSize)) {
      nextProportionalPWMValue = desiredPos[servoID];
    }
    //Get the actual PWM value
    unsigned int PWMValue = translateBounds(servoID, (int) nextProportionalPWMValue);
    //Update what our current PWM output is for this servo
    currentPos[servoID] = nextProportionalPWMValue;
    //Get the servo pin number
    unsigned int servoPinNumber = servoPinNumbers[servoID];
    //Actuate the servo with the PWM value
    analogWrite(servoPinNumber, PWMValue);
  }
}

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

////////////////////////////
//SECTION: Helper Functions
////////////////////////////

boolean compareArray(float *a, int *b){
  int n;
  // test each element to be the same. if not, return false
  for (n=0; n < 12; n++) {
    if (a[n] != b[n]) return false;
  }
  return true;
}

unsigned int translateBounds(unsigned int servoIndex, int value) {
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

  if (value > 1000) Serial.println("ERROR: value over 1000");
  else if (value < 0) Serial.println("ERROR: value less than 0");
  else if (servoPWMValueBounds[servoIndex][0] < servoPWMValueBounds[servoIndex][1]) {
    return (unsigned int) ((servoPWMValueBounds[servoIndex][1]
                          - servoPWMValueBounds[servoIndex][0]) * value / 1000)
                          + servoPWMValueBounds[servoIndex][0];
  } else {
    return (unsigned int) servoPWMValueBounds[servoIndex][0] 
                          - ((servoPWMValueBounds[servoIndex][0]
                            - servoPWMValueBounds[servoIndex][1]) * value / 1000);
  }
  //Something is wrong, so program is effectively stopped here.
  while(1);
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
