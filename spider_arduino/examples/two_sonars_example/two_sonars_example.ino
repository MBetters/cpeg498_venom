//Sonar 1
int echoPin1 =13;
int initPin1 =12;
int distance1 =0;

//Sonar 2
int echoPin2 =8;
int initPin2 =7;
int distance2 =0;

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
 

  delay(500);
  
}

int getDistance (int initPin, int echoPin){

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
