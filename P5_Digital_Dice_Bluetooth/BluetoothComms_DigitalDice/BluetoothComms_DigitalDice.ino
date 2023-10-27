#include <SoftwareSerial.h>
const byte rxPin = 9;
const byte txPin = 10;
SoftwareSerial Bluetooth(rxPin,txPin);
void setup() {
  // put your setup code here, to run once:
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  Bluetooth.begin(9600);
  Serial.begin(9600);
}
String msgBuffer = " ";
String msg ="";
char data="";
void loop() {
  // put your main code here, to run repeatedly:
  
  while(Bluetooth.available() == 0){}
  char Data = (char) Bluetooth.read(); // Break the incoming data into bits
  if(data == ';'){
    msg=msgBuffer;
    msgBuffer=" ";//Clears the message Buffer for new Data
    Serial.println(msg);
    Bluetooth.print("Your chance ");
    Bluetooth.print(msg);
    int num = random(1,7);
    Bluetooth.println(num);
  }
  msgBuffer +=Data;
}
