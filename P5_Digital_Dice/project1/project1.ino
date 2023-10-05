byte button = 3;
void setup(){
  pinMode(button,INPUT);
  Serial.begin(9600);
  digitalWrite(button,HIGH);
}
void loop(){
  byte num = random(1,7)// Gives random number from 1 to 6
  Serial.println("Your chance: ");
  Serial.println(num);
}