#include <LiquidCrystal.h>

byte rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte trigPin = 7, echoPin = 5, relay = 3;
const int lowlvl = 150, highlvl = 50;

long duration, distance, litre;

unsigned long previousMillis = 0;
const long interval = 500; // Reading every 500ms

bool motorState = false; 

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relay, OUTPUT);
  
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  digitalWrite(trigPin, LOW);
  digitalWrite(relay, LOW);
}

void loop() {
  // Delay for stable reading
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Ultrasonic Measurement
    distance = getDistance();

    // Map Distance to Litre
    litre = map(distance, 200, 10, 0, 20); // 200cm = 0L, 10cm = 20L

    // Display Water Level
    lcd.setCursor(0, 0);
    lcd.print("Water Level:");
    lcd.setCursor(0, 1);
    lcd.print(litre);
    lcd.print("L     ");

    // Motor Control
    if (distance > lowlvl) {
      if (motorState) {
        motorState = false;
        digitalWrite(relay, LOW);
        lcd.setCursor(10, 0);
        lcd.print("OFF ");
      }
    } else if (distance < highlvl) {
      if (!motorState) {
        motorState = true;
        digitalWrite(relay, HIGH);
        lcd.setCursor(10, 0);
        lcd.print("ON  ");
      }
    }
  }
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return (pulseIn(echoPin, HIGH) * 0.0343) / 2; // Convert to cm
}
