//For the echo and trigger pins can be any pin, just changed the value according to what pins are available.
//Sonar 1
unsigned int echoPin1 =13;
unsigned int initPin1 =12;
 unsigned int distance1 =0;

//Sonar 2
 unsigned int echoPin2 =8;
unsigned int initPin2 =7;
 unsignedint distance2 =0;

void setup() {
  
  pinMode(initPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(initPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  Serial.begin(9600);
  
}

void loop() {
  distance1 = getDistance(initPin1, echoPin1);
  printDistance(1, distance1);
  delay(150);
  
  distance2 = getDistance(initPin2, echoPin2);
  printDistance(2, distance2);
  delay(150);
  Serial.println(" ");
  
}

unsigned int getDistance (int initPin, int echoPin){

 digitalWrite(initPin, HIGH);
 delayMicroseconds(10); 
 digitalWrite(initPin, LOW); 
 unsigned long pulseTime = pulseIn(echoPin, HIGH); 
 int distance = pulseTime/58;
 return distance;
 
}
 
 void printDistance(int id, int dist){
  
     Serial.print(id);
     Serial.print(" ");
    if (dist >= 120 || dist <= 0 ){
      Serial.println(" Out of range");
    }else
    
    Serial.print(dist, DEC);
    Serial.println(" cm");
    
 }
