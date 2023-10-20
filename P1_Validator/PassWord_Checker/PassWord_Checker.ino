#include <SoftwareSerial.h>
String password = "mango" ;
String userName= "mangoboy";
byte red=9;//PWM pins
byte green=10;//PWM pins
SoftwareSerial Bluetooth(11,12);//RX TX
void setup() {
  // put your setup code here, to run once:
  pinMode(ledRed,OUTPUT);
  pinMode(ledGreen,OUTPUT);
  Bluetooth.begin(9600);
}
boolean Check(String input,String pass){
  int i;
  boolean count=true;
  if(input.length()!= pass.length()){
    Bluetooth.println("Wrong length of given password");
    return false;
  }
  else{
    for(i=0;i<pass.length();i++){
      if(pass.charAt(i)!=input.charAt(i)){
        count=false;
      }
      if(count == false){
        return false;
      }
      else{
        return true;
      }
    }
  }
}
void Rled(){
  analogWrite(red,255);
  analogWrite(green,0);
}
void Gled(){
  analogWrite(red,0);
  analogWrite(green,255);
}
void loop() {
  // put your main code here, to run repeatedly:
  Bluetooth.print("Enter UserName: ");
  while(Bluetooth.available()==0){}
  String inputUser=Bluetooth.readString();
  Bluetooth.println(inputUser);
  Bluetooth.print("Enter your password: ");
  while(Bluetooth.available()==0){}
  String inputpass=Bluetooth.readString();
  Bluetooth.println(inputpass);
  if(Check(inputpass,password) == true && Check(inputUser,userName) == true){
    Gled();
    Bluetooth.println("Valid User");
  }
  else{
    Rled();
    Bluetooth.println("Invalid User");
  }
}
