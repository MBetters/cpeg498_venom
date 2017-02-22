//Sonar 1
int echoPin1 =13;
int initPin1 =12;
int distance1 =0;

//Sonar 2
int echoPin2 =8;
int initPin2 =7;
int distance2 =0;

/*//Sonar 3
int echoPin3 =8;
int initPin3 =9;
int distance3 =0;

//Sonar 4
int echoPin4 =12;
int initPin4 =13;
int distance4 =0;*/

void setup() {
  
  pinMode(initPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(initPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  /*pinMode(initPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(initPin4, OUTPUT);
  pinMode(echoPin4, INPUT);*/
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
  
  /*distance3 = getDistance(initPin3, echoPin3);
  printDistance(3, distance3);
  delay(150);
  
  distance4 = getDistance(initPin4, echoPin4);
  printDistance(4, distance4);*/

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
