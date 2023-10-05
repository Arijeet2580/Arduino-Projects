#include <SoftwareSerial.h>
const byte rxPin = 9;
const byte txPin = 10;
SoftwareSerial bt(rxPin,txPin);
void setup() {
  // put your setup code here, to run once:
  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
  Serial.begin(9600);
  bt.begin(9600);
}
String msgBuffer = "";
String msg ="";
char data="";
void loop() {
  // put your main code here, to run repeatedly:
  
  while(bt.available() == 0){}
  char Data = (char) bt.read(); // Break the incoming data into bits
  msgBuffer +=Data;
  if(data == ';'){
    msg=msgBuffer;
    msgBuffer=" ";//Clears the message Buffer for new Data
    Serial.println(msg);
    bt.print("Your chance ");
    bt.print(msg);
    int num = random(1,7);
    bt.println(num);
  }
}
