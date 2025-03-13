#include <LiquidCrystal.h>

// LCD Pin Configuration
byte rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pin Definitions
const byte trigPin = 7, echoPin = 5, relay = 3, buzzer = 4;

// Water Level Thresholds
const int lowlvl = 150;       // Motor OFF Threshold
const int highlvl = 50;       // Motor ON Threshold
const int criticalLevel = 180;  // Critical Water Level for Buzzer Alert
const int warningLevel = 160;   // Warning Water Level for Intermittent Buzzer

// Variables for Measurement
long duration, distance, litre;
int tankHeight = 200;        // Maximum height in cm
int tankCapacity = 20;       // Maximum capacity in liters

// Timer for Sensor Reading
unsigned long previousMillis = 0;
const long interval = 500;   // Read every 500ms

// Timer for Buzzer Patterns
unsigned long buzzerMillis = 0;
const long buzzerInterval = 1000;  // Buzzer pattern interval
bool buzzerState = false;

// Motor State
bool motorState = false;
bool inWarningZone = false;
bool inCriticalZone = false;

void setup() {
  // Initialize pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  // Start serial communication
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.begin(16, 2);
  
  // Initial state
  digitalWrite(trigPin, LOW);
  digitalWrite(relay, LOW);
  digitalWrite(buzzer, LOW);
  
  // Display startup message
  lcd.setCursor(0, 0);
  lcd.print("Water Controller");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Read sensor at specified intervals
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Measure Distance
    distance = getDistance();
    
    // Convert Distance to Litres (with bounds checking)
    if (distance > tankHeight) distance = tankHeight;
    if (distance < 0) distance = 0;
    
    litre = map(distance, tankHeight, 10, 0, tankCapacity);
    
    // Constrain litre value to valid range
    litre = constrain(litre, 0, tankCapacity);
    
    // Calculate percentage full
    int percentFull = map(litre, 0, tankCapacity, 0, 100);
    
    // Update display
    updateDisplay(litre, percentFull);
    
    // Motor Control
    controlMotor(distance);
    
    // Update alarm states
    updateAlarmStates(distance);
    
    // Log to serial
    logToSerial(distance, litre, percentFull);
  }
  
  // Handle buzzer patterns independently of sensor readings
  handleBuzzer(currentMillis);
}

// Function to Get Distance from Ultrasonic Sensor
long getDistance() {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Set the trigger pin high for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echo pin, convert the time to distance in cm
  duration = pulseIn(echoPin, HIGH, 30000); // Added timeout for reliability
  
  // If timeout occurred, return max distance
  if (duration == 0) return tankHeight;
  
  return (duration * 0.0343) / 2; // Speed of sound = 343 m/s = 0.0343 cm/µs
}

// Update the LCD display
void updateDisplay(int litre, int percentFull) {
  lcd.setCursor(0, 0);
  lcd.print("Level: ");
  lcd.print(litre);
  lcd.print("L (");
  lcd.print(percentFull);
  lcd.print("%) ");
  
  lcd.setCursor(0, 1);
  
  // Display status message
  if (inCriticalZone) {
    lcd.print("CRITICAL LOW!");
  } else if (inWarningZone) {
    lcd.print("WARNING LOW! ");
  } else if (motorState) {
    lcd.print("Motor: ON     ");
  } else {
    lcd.print("Motor: OFF    ");
  }
}

// Control the motor based on water level
void controlMotor(int distance) {
  if (distance > lowlvl) {
    if (motorState) {
      motorState = false;
      digitalWrite(relay, LOW);
    }
  } else if (distance < highlvl) {
    if (!motorState) {
      motorState = true;
      digitalWrite(relay, HIGH);
    }
  }
}

// Update alarm states based on water level
void updateAlarmStates(int distance) {
  // Check for critical low water level
  inCriticalZone = (distance > criticalLevel);
  
  // Check for warning water level
  inWarningZone = (distance > warningLevel && distance <= criticalLevel);
}

// Handle buzzer patterns based on alarm states
void handleBuzzer(unsigned long currentMillis) {
  if (inCriticalZone) {
    // Continuous alarm for critical level
    digitalWrite(buzzer, HIGH);
  } 
  else if (inWarningZone) {
    // Intermittent alarm for warning level
    if (currentMillis - buzzerMillis >= buzzerInterval) {
      buzzerMillis = currentMillis;
      buzzerState = !buzzerState;
      digitalWrite(buzzer, buzzerState);
    }
  } 
  else {
    // No alarm needed
    digitalWrite(buzzer, LOW);
  }
}

// Log data to serial for monitoring
void logToSerial(int distance, int litre, int percentFull) {
  Serial.printf("Distance: %d cm, Water: %d L, %d Percentage\n",distance,litre,percentFull);
  Serial.printf("Motor: %s\n",motorState ? "ON" : "OFF");
  Serial.print("Status: ");
  
  if (inCriticalZone) {
    Serial.println("CRITICAL");
  } else if (inWarningZone) {
    Serial.println("WARNING");
  } else {
    Serial.println("NORMAL");
  }
}
