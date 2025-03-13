#include <LiquidCrystal.h>

// LCD Pin Configuration
byte rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Temperature Sensor Pin
const byte ts = A0;
const byte buzzer = 8; // Buzzer Pin
const byte relay = 7; // Relay Pin for Fan Control

// Calibration Constants
const int X1 = 104, X2 = 155;
const int Y1 = 0, Y2 = 25;
const float m = 0.5;  // Slope (Y2-Y1) / (X2-X1)

// Temperature Thresholds
const int overheatThreshold = 30; // Temperature to turn on fan
const int ambientThreshold = 25;  // Temperature to turn off fan

// Timer for Sensor Reading
unsigned long previousMillis = 0;
const long interval = 1000;  // Read temperature every 1000ms

// Fan State
bool fanState = false;

void setup() {
  lcd.begin(16, 2);
  pinMode(ts, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(buzzer, LOW); // Ensure buzzer is off at startup
  digitalWrite(relay, LOW); // Ensure fan is off at startup
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Sensor Reading
    int SV = analogRead(ts);

    // Temperature Calculation
    int Temp = (SV - X1) * m;

    // LCD Display
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperature: ");
    lcd.print(Temp);
    lcd.print((char)223); // Degree Symbol
    lcd.print("C");

    // Serial Monitor Output
    Serial.print("Sensor Value: ");
    Serial.print(SV);
    Serial.print(" | Temperature: ");
    Serial.print(Temp);
    Serial.println("C");

    // Buzzer Activation for Overheating
    if (Temp >= overheatThreshold) {
      digitalWrite(buzzer, HIGH);
    } else {
      digitalWrite(buzzer, LOW);
    }

    // Fan Control using Relay
    if (Temp >= overheatThreshold) {
      if (!fanState) {
        digitalWrite(relay, HIGH); // Turn on fan
        fanState = true;
      }
    } else if (Temp <= ambientThreshold) {
      if (fanState) {
        digitalWrite(relay, LOW); // Turn off fan
        fanState = false;
      }
    }
  }
}
