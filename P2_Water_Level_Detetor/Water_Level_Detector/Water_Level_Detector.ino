#include <LiquidCrystal.h>
byte rs=8,en=9,d4=10,d5=11,d6=12,d7=13;
LiquidCrystal lcd (rs,en,d4,d5,d6,d7);
byte trigPin = 7,echoPin = 5,relay= 3;
byte lowlvl = 150, highlvl= 50;
void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(relay,OUTPUT);
  Serial.begin(9600); //Debugging
  lcd.begin(16,2);//16X2 lcd screen
  digitalWrite(trigPin,LOW);
  digitalWrite(relay,LOW);
}
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(30);    //Delay recommended > 10 microseconds accoroding to datasheet of HC-SR04
  digitalWrite(trigPin,LOW);
  long duration = pulseIn(echoPin,HIGH);
  long distance = (duration * 0.0344)/2;
  /*
  200cm -> 0L  |   10cm->20L
  */
  int litre= map(distance,200,10,0.20);
  lcd.setCursor(0,0);
  lcd.print("Water Level: ");
  lcd.setCursor(0,1);
  lcd.print(litre);
  lcd.print("L");
  lcd.setCursor(16,0);
  lcd.autoScroll();
  if(distance > 150 )
  {
    lcd.print("MOTOR: OFF");
    digitalWrite(relay,LOW);
  }
  else
  {
    lcd.print("MOTOR ON ");
    digitalWrite(relay,HIGH);
  }
  lcd.noAutoScroll();
  lcd.clear();
}
