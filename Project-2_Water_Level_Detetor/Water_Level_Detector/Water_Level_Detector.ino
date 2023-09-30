byte trigPin = 7;
byte echoPin = 5;
byte motor   = 3;
//UltraSonic Sensor is ranges from 3cm to 4m
// The Size of the water tank is 2m
//Let's take Water level when motor needs to be ON be 1.5m so 150 cm
byte lowlvl = 150;
//Let's take Water level when motor needs to be OFF be 0.5m so 50 cm
byte highlvl= 50;
void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(motor,OUTPUT);
  Serial.begin(9600);
  digitalWrite(trigPin,LOW);//Initialising the trigPin to LOW at first
  digitalWrite(motor,LOW); // At first, Motor should always be OFF then the motor can run accoroding to the condition
}
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(30);    //Delay recommended > 10 microseconds accoroding to datasheet of HC-SR04
  digitalWrite(trigPin,LOW);
  long duration = pulseIn(echoPin,HIGH);
  //Counts the duration the echoPin is HIGH .
  //or it returns the time taken the sound to reach the reciever
  long distance = (duration * 0.0344)/2;
  //344 m/s to 0.0344 cm/microsecods
  //the distance is divided by 2 because the echopin counts the time it take to
  //reach the object and come  from there.
  //The value returned by the distance is in cm
  Serial.println(distance);
  if(distance > 150 )
  {
    Serial.println("Switching OFF the motor");
    digitalWrite(motor,LOW);
  }
  else
  {
    Serial.println("Switching ON the motor");
    digitalWrite(motor,HIGH);
  }

}
