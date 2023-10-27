#include <LiquidCrystal.h>
byte rs=12,byte en=11,byte d4=5,byte d5=4,byte d6=3,byte d7=2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
byte ts = A0;
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  pinMode(ts,INPUT);
  Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:
  int SV= analogRead(A0);
  delay(500);
  /*
    SENSOR CALLIBRATION
    X1 =104  X2=155
    Y1 =0     Y2=25
    m=(y2-y1)/(x2-x1)
     = 0.5
    Temperature= m*(Sensor Value -104)
    T=(SV-104)/2
  */
  int Temp = (SV-104)/2;
  lcd.setCursor(0, 0);
  delay(1000);
  lcd.print("Temperature: ");
  lcd.print(Temp);
  Serial.println(SV);
  delay(2000);
}
