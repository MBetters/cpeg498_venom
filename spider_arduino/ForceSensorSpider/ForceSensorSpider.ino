
 // Current pins being used by servos: 3, 4, 5, 6, 9, 10, 20, 21, 22, 23, 25, 32
 // Possible pins for each force sensor: 14, 15, 16, 17, 18, 19
int fsrFLL = 14; // FSR for Front Left Leg
//int fsrReadingFLL;
int fsrMLL = 15; // Middle Left Leg
//int ReadingMLL; 
int fsrBLL = 16; // Back Left Leg
//int fsrReadingBLL; 
int fsrFRL = 17; // Front Right Leg
//int fsrReadingFRL;
int fsrMRL = 18; // Middle Right Leg
//int fsrReadingMRL;
int fsrBRL = 19; // Back Right Leg
//int fsrReadingBRL;
 
void setup(void) {
  Serial.begin(9600);   // Send debugging information via the Serial monitor
}
 
void loop(void) {
  getSensorRead(fsrFLL); // Gets sensor reading for FLL and outputs its respective value.
  getSensorRead(fsrMRL);
  getSensorRead(fsrBRL);
  getSensorRead(fsrFRL);
  getSensorRead(fsrMRL);
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


