unsigned int FLS = 32; //front-left shoulder pin number, tested
unsigned int FLE = 21; //front-left elbow pin number, tested
unsigned int FRS = 10; //front-right shoulder pin number, tested
unsigned int FRE = 20; //front-right elbow pin number, tested
unsigned int BLS = 25; //back-left shoulder pin number, tested
unsigned int BLE = 22; //back-left elbow pin number, tested
unsigned int BRS = 6; //back-right shoulder pin number, tested
unsigned int BRE = 9; //back-right elbow pin number, tested
unsigned int MLS = 4; //middle-left shoulder, tested
unsigned int MLE = 23; //middle-left elbow, tested
unsigned int MRS = 5; //middle-right shoulder, tested
unsigned int MRE = 3; //middle-right elbow, tested

unsigned int allElbows = 600;
bool goingDown = false;

void setup() {
  
  //Set 8 of the PWM pins (pins 
  //3 = MRE (8600, 3800)
  //4 = MLS (8000, 3800)
  //5 = MRS (1500, 4500)
  //6 = BRS (2000, 6500)
  //9 = BRE (8200, 3700)
  //10 = FRS (2000, 8000)
  //20 = FRE (7750, 3500)
  //21 = FLE (1800, 5400)
  //22 = BLE (1950, 6100)
  //23 = MLE (1750, 5700)
  //25 = BLS (8500, 3800)
  //32 = FLS (8300, 2200)
  //to have freq=50Hz.
  //Why 50Hz?
  //Answer: Because we need the PWM pins operating at 20ms period,
  //        since that's what the SG90 Servo datasheet says is the operating period.

  // Set pin variables:

  //{FLS, FLE, 
  // FRS, FRE,
  // BLS, BLE,
  // BRS, BRE,
  // MLS, MLE,
  // MRS, MRE};
  
  analogWriteFrequency(3, 50);
  analogWriteFrequency(4, 50);
  analogWriteFrequency(5, 50);
  analogWriteFrequency(6, 50);
  analogWriteFrequency(20, 50);
  analogWriteFrequency(21, 50);
  analogWriteFrequency(22, 50);
  analogWriteFrequency(23, 50);
  analogWriteFrequency(25, 50);
  
  //Open a serial communication line with data rate = 9600 bits per second.
  //NOTE: The Raspberry Pi sends commands and the Arduino receives them.
  //      Nothing is sent back the other way.
  //NOTE: Make sure that the Raspberry Pi has a matching bit transmission rate,
  //      so that the Arduino is receiving new bits at the same rate that the 
  //      Raspberry Pi is sending them.
  Serial.begin(9600);
  analogWriteResolution(16);
  Serial.println("setup done");
}

void loop() {

  //Un-comment these two lines when you do initial servo testing
  //test_all_legs();
  delay(100);

  //Everything inside the "if (Serial.available()) { ... }" block is 
  //a check for the different inputs.
  //To test these inputs out on a computer, use Tools->Serial Monitor
  if (not goingDown and allElbows > 0) {
    allElbows -= 1;
  } else if (goingDown and allElbows < 1000) {
    allElbows += 1;
  } else if (goingDown) {
    goingDown = false;
  } else if (not goingDown) {
    goingDown = true;
  }
  
  analogWrite(FRE, translateBounds("FRE", allElbows));
  analogWrite(FLE, translateBounds("FLE", allElbows));
  analogWrite(BRE, translateBounds("BRE", allElbows));
  analogWrite(BLE, translateBounds("BLE", allElbows));
  analogWrite(MRE, translateBounds("MRE", allElbows));
  analogWrite(MLE, translateBounds("MLE", allElbows));

  if (Serial.available()) {
    char ch = Serial.read();
    //Here, the Arduino reads serial USB input, 
    //which will be commands (in the form of characters) coming from the RaspberryPi.
    //The Arduino must translate those commands into servo PWM pin commands.

    if (ch == 'w') {
      Serial.println("Received 'move forward' command");
    }
    else if (ch == 'a') {
      Serial.println("Received 'move left' command");
      //move_left();
    }
    else if (ch == 's') {
      Serial.println("Received 'move backward' command");
    }
    else if (ch == 'd') {
      Serial.println("Received 'move right' command");
      //move_right();
    }
  }
}

unsigned int translateBounds(String servo, unsigned int value) {
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
  unsigned int translationTable[12][2] = {{8300, 2200},{1800, 5400},{2000, 8000},{7750, 3500},{8500, 3800},{1950, 6100},{2000, 6500},{8200, 3700},{8000, 3800},{1750, 5700},{1500, 4500},{8500, 3800}};
  
  if (translationTable[index][0] < translationTable[index][1]) {
    return (unsigned int) ((translationTable[index][1] - translationTable[index][0]) * value / 1000) + translationTable[index][0];
  } else {
    return (unsigned int) translationTable[index][0] - ((translationTable[index][0] - translationTable[index][1]) * value / 1000);
  }
}
