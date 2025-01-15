#include <Servo.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
// Bluetooth Initialisation
const byte RX = 6,TX = 7;
SoftwareSerial Bluetooth(RX,TX);
//LiquidCrystal Initialisation
const byte rs=8,en=9,d4=10,d5=11,d6=12,d7=13;
LiquidCrystal lcd (rs,en,d4,d5,d6,d7);
// Servo Initialisation
Servo Servo1; 
byte Gas=A0,buzzer=5;
const int DANGER=700; //MQ-5 GAS SENSOR DANGER level: 700 analog Value
/*
0ppm    --> 60  (Ambient Air)
1000ppm --> 800
X1=60 Y1=0
X2=800 Y2=1000
m=(y2-y1)/(x2-x1)
m=1.351
ppm=m*(Sensor Value -60)
  = 1.351*(SV-60)
*/
const byte SERVO=4;
byte SVal=0;
byte ppm=0;
byte red=3;
byte green=2;
int a[4]={440,494,523,672};
void setup() {
  // put your setup code here, to run once:
  Bluetooth.begin(9600);
  lcd.begin(16, 2);
  pinMode(RX,INPUT);
  pinMode(TX,OUTPUT);
  Servo1.attach(SERVO);
  pinMode(buzzer,OUTPUT);
}
void loop() {
  SVal=analogRead(Gas);
  ppm= 1.351*(SVal-60);
  //Bluetooth Communication 
  Bluetooth.print("Concentration Level: ");
  Bluetooth.println(ppm);
  // LCD communication 
  lcd.setCursor(0,0);
  lcd.print("PPM: ");
  lcd.print(ppm);
  if(SVal > DANGER){
    Servo1.write(100);   //In Practice, 100 degrees Required for the desired Result to Open the Window
    delay(30); //Mechanical Response Time
    lcd.setCursor(0,1);
    lcd.print("LPG Present");
    Bluetooth.println("LPG Detected Nearby Opening the Nearby Window");
    for(int i=0;i<4;i++){
      tone(buzzer,a[i],300); 
      delay(1000);
    }
    noTone(buzzer);
    digitalWrite(red,HIGH);
    digitalWrite(green,LOW);      
  }
  else{
    lcd.setCursor(0,1);
    lcd.print("LPG Absent");    
    digitalWrite(red,LOW);
    digitalWrite(green,HIGH);
  }
  lcd.clear();
}
