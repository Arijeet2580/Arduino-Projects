String password = "mango" ;
String userName= "mangoboy";
byte ledRed=13;
byte ledGreen=11;
void setup() {
  // put your setup code here, to run once:
  pinMode(ledRed,OUTPUT);
  pinMode(ledGreen,OUTPUT);
  Serial.begin(9600);
}
boolean Check(String input,String pass){
  int i;
  boolean count=true;
  if(input.length()!= pass.length()){
    Serial.println("Wrong length of given password");
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
void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Enter UserName: ");
  while(Serial.available()==0){}
  String inputUser=Serial.readString();
  Serial.print("Enter your password: ");
  while(Serial.available()==0){}
  String inputpass=Serial.readString();
  delay(10);
  Serial.println();
  Serial.print("Your Username:");
  Serial.println(inputUser);
  Serial.println();
  Serial.print("Your password:");
  Serial.println(inputpass);
  if(Check(inputpass,password) == true && Check(inputUser,userName) == true){
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledRed,LOW);
    Serial.println("Valid User");
  }
  else{
    digitalWrite(ledGreen,LOW);
    digitalWrite(ledRed,HIGH);
    Serial.println("Invalid User");
  }
}
