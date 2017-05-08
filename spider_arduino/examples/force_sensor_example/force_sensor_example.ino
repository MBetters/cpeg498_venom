
 // Current pins being used by servos: 3, 4, 5, 6, 9, 10, 20, 21, 22, 23, 25, 32
// Possible pins for each force sensor: 14, 15, 16, 17, 18, 19
unsigned int fsrFLL = 14; // FSR for Front Left Leg
unsigned int fsrBLL = 16; // Back Left Leg 
unsigned int fsrFRL = 17; // Front Right Leg
unsigned int fsrBRL = 19; // Back Right Leg

 
void setup(void) {
  Serial.begin(9600);   // Send debugging information via the Serial monitor
}
 
void loop(void) {
  getSensorRead(fsrFLL); // Gets sensor reading for FLL and outputs its respective value.
  getSensorRead(fsrBRL);
  getSensorRead(fsrFRL);
  getSensorRead(fsrBRL);
  delay(100);
}

unsigned int getSensorRead(unsigned int SensorPinNumber)
{
  if (SensorPinNumber == 14)
  {
    Serial.print("FLL reading = ");
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
  else if (SensorPinNumber == 19)
  {
    Serial.print("BRL reading = ");
    Serial.println(analogRead(SensorPinNumber));
  }
  
}


