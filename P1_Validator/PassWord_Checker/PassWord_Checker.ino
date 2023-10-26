#include <SoftwareSerial.h> //Bluetooth Communication
byte relay=2;// For Solenoid Lock
String password = "mango" ;
String userName= "mangoboy";
byte Red=9;//PWM pins
byte Green=10;//PWM pins
SoftwareSerial Bluetooth(11,12);//RX TX
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // For Debugging
  pinMode(Red,OUTPUT);
  pinMode(Green,OUTPUT);
  Bluetooth.begin(9600);
  digitalWrite(relay,LOW);  //By default closed system
}
boolean Check(String input,String pass){
  int i;
  if(input.length()!= pass.length()){
    Bluetooth.println("Wrong length password");
    return false;
  }
  else{
    for(i=0;i<pass.length();i++){
      if(pass.charAt(i)!=input.charAt(i)){
        Bluetooth.println("Wrong PassWord");
        return false;
      }
    }
  }
}
void Rled(){
  analogWrite(Red,255);
  analogWrite(Green,0);
}
void Gled(){
  analogWrite(Red,0);
  analogWrite(Green,255);
}
void LockOpen(){
  digitalWrite(relay,HIGH);
}
void LockClose(){
  digitalWrite(relay,LOW);
}
void loop() {
  // put your main code here, to run repeatedly:
  Bluetooth.print("Enter UserName: ");
  while(Bluetooth.available()==0){}
  String UserInput=Bluetooth.readString();
  Bluetooth.println(UserInput);
  Bluetooth.print("Enter your password: ");
  while(Bluetooth.available()==0){}
  String PassInput=Bluetooth.readString();
  Bluetooth.println(PassInput);
  if(Check(PassInput,password) == true && Check(UserInput,userName) == true){
    Gled();
    Bluetooth.println("Valid User");
    LockOpen();  //Solenoid Lock Opens

  }
  else{
    Rled();
    Bluetooth.println("Invalid User");
    LockClose();   //Solenoid Lock Closes
  }
}
