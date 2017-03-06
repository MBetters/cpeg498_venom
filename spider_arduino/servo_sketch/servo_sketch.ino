//These are the pin numbers of the different servos.
unsigned int FLS = 3; //front-left shoulder pin number
unsigned int FLE = 4; //front-left elbow pin number
unsigned int FRS = 5; //front-right shoulder pin number
unsigned int FRE = 6; //front-right elbow pin number
unsigned int BLS = 20; //back-left shoulder pin number
unsigned int BLE = 21; //back-left elbow pin number
unsigned int BRS = 22; //back-right shoulder pin number
unsigned int BRE = 23; //back-right elbow pin number

//These are the current PWM values of the different servos.
unsigned int FLSCurrentPWM = 3277;
unsigned int FLECurrentPWM = 3277;
unsigned int FRSCurrentPWM = 3277;
unsigned int FRECurrentPWM = 3277;
unsigned int BLSCurrentPWM = 3277;
unsigned int BLECurrentPWM = 3277;
unsigned int BRSCurrentPWM = 3277;
unsigned int BRECurrentPWM = 3277;

//These are the next PWM values of the different servos.
unsigned int FLSNextPWM;
unsigned int FLENextPWM;
unsigned int FRSNextPWM;
unsigned int FRENextPWM;
unsigned int BLSNextPWM;
unsigned int BLENextPWM;
unsigned int BRSNextPWM;
unsigned int BRENextPWM;

//This gives the current action.
//Possible values: stand still, forward, backward, left, right, turn CW, turn CCW
String currentAction = "forward";

void setup() {

  //Good resource on Teensy PWM capabilities: https://www.pjrc.com/teensy/td_pulse.html
  
  //Set all PWM pins to have freq=50Hz.
  //Why 50Hz?
  //Answer: Because we need the PWM pins operating at 20ms period,
  //        since that's what the SG90 Servo datasheet says is the operating period.
  //Note that you couldn't set these to different frequencies even if you tried,
  //since some of the pins share on-board timers.
  analogWriteFrequency(FLS, 50);
  analogWriteFrequency(FLE, 50);
  analogWriteFrequency(FRS, 50);
  analogWriteFrequency(FRE, 50);
  analogWriteFrequency(BLS, 50);
  analogWriteFrequency(BLE, 50);
  analogWriteFrequency(BRS, 50);
  analogWriteFrequency(BRE, 50);

  //Set the PWM resolution to 16 bits, 
  //so that analogWrite takes 0-65535,
  //where 0 is 0% duty cycle and 65535 is 100% duty cycle.
  //The servos accept a duty cycle range of 5% to 10%,
  //so analogWrite values should range from 3277 to 6553.
  analogWriteResolution(16);

  //Open a serial communication line with data rate = 9600 bits per second.
  //NOTE: The Raspberry Pi sends commands and the Arduino receives them.
  //      In the future, the Arduino will send the latest distance sensor values
  //      back to the Raspberry Pi.
  //NOTE: Make sure that the Raspberry Pi has a matching bit transmission rate,
  //      so that the Arduino is receiving new bits at the same rate that the 
  //      Raspberry Pi is sending them and vise versa.
  Serial.begin(9600);
  
  Serial.println("setup done");
}

void loop() {

  //Move the front-left leg.
  FLSNextPWM = getNextPWM(currentAction, "FLS", FLSCurrentPWM);
  FLENextPWM = getNextPWM(currentAction, "FLE", FLECurrentPWM);
  analogWrite(FLS, FLSNextPWM);
  analogWrite(FLE, FLENextPWM);
  FLSCurrentPWM = FLSNextPWM;
  FLECurrentPWM = FLENextPWM;
  //Move the front-right leg.
  FRSNextPWM = getNextPWM(currentAction, "FRS", FRSCurrentPWM);
  FRENextPWM = getNextPWM(currentAction, "FRE", FRECurrentPWM);
  analogWrite(FRS, FRSNextPWM);
  analogWrite(FRE, FRENextPWM);
  FRSCurrentPWM = FRSNextPWM;
  FRECurrentPWM = FRENextPWM;
  //Move the back-left leg.
  BLSNextPWM = getNextPWM(currentAction, "BLS", BLSCurrentPWM);
  BLENextPWM = getNextPWM(currentAction, "BLE", BLECurrentPWM);
  analogWrite(BLS, BLSNextPWM);
  analogWrite(BLE, BLENextPWM);
  BLSCurrentPWM = BLSNextPWM;
  BLECurrentPWM = BLENextPWM;
  //Move the back-right leg.
  BRSNextPWM = getNextPWM(currentAction, "BRS", BRSCurrentPWM);
  BRENextPWM = getNextPWM(currentAction, "BRE", BRECurrentPWM);
  analogWrite(BRS, BRSNextPWM);
  analogWrite(BRE, BRENextPWM);
  BRSCurrentPWM = BRSNextPWM;
  BRECurrentPWM = BRENextPWM;
  
  //Everything inside the "if (Serial.available()) { ... }" block is 
  //a check for the different inputs.
  //To test these inputs out on a computer, use Tools->Serial Monitor
  if (Serial.available()) {
    char ch = Serial.read();
    //Here, the Arduino reads serial USB input, 
    //which will be commands (in the form of characters) coming from the RaspberryPi.
    //The Arduino must translate those commands into servo PWM pin commands.
    if (ch == 'w') {
      Serial.println("Received 'forward' command");
      currentAction = "forward";
    }
    else if (ch == 'a') {
      Serial.println("Received 'left' command");
      currentAction = "left";
    }
    else if (ch == 's') {
      Serial.println("Received 'backward' command");
      currentAction = "backward";
    }
    else if (ch == 'd') {
      Serial.println("Received 'right' command");
      currentAction = "right";
    }
    else if (ch == 'c') {
      Serial.println("Received 'turn CW' command");
      currentAction = "turn CW";
    }
    else if (ch == 'v') {
      Serial.println("Received 'turn CCW' command");
      currentAction = "turn CCW";
    }
  }

  //Have a small delay before the next call to loop()
  delay(2);
}

unsigned int getNextPWM(String currentAction, String servoName, unsigned int currentPWM) {
  //TODO: Implement this function.
  //      It will use sinusoids to compute servo PWM signals
}

