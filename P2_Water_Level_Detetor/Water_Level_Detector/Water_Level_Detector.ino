#include <LiquidCrystal.h>

// LCD Pin Configuration - using uint8_t instead of byte for consistency
uint8_t rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pin Definitions
const uint8_t trigPin = 7, echoPin = 5, relay = 3, buzzer = 4;

// Water Level Thresholds
const uint8_t lowlvl = 150;      // Motor OFF Threshold
const uint8_t highlvl = 50;      // Motor ON Threshold
const uint8_t criticalLevel = 180; // Critical Water Level for Buzzer Alert
const uint8_t warningLevel = 160;  // Warning Water Level for Intermittent Buzzer

// Variables for Measurement
uint16_t duration;
uint8_t distance, litre;
const uint8_t tankHeight = 200;     // Maximum height in cm
const uint8_t tankCapacity = 20;    // Maximum capacity in liters

// Timer for Sensor Reading
uint32_t previousMillis = 0;
const uint16_t interval = 500;   // Read every 500ms

// Timer for Buzzer Patterns
uint32_t buzzerMillis = 0;
const uint16_t buzzerInterval = 1000;  // Buzzer pattern interval
bool buzzerState = false;

// State flags combined into a single byte - bit field
uint8_t systemState = 0;
// Bit positions
#define MOTOR_STATE 0
#define WARNING_ZONE 1
#define CRITICAL_ZONE 2

// Macros for bit manipulation
#define SET_BIT(reg, bit) (reg |= (1 << bit))
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))
#define READ_BIT(reg, bit) ((reg >> bit) & 1)

// Function prototypes
uint8_t getDistance();
void updateDisplay(uint8_t litre, uint8_t percentFull);
void controlMotor(uint8_t distance);
void updateAlarmStates(uint8_t distance);
void handleBuzzer(uint32_t currentMillis);

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
  
  // Initial state - all LOW
  digitalWrite(trigPin, LOW);
  digitalWrite(relay, LOW);
  digitalWrite(buzzer, LOW);
  
  // Display startup message
  lcd.print(F("Water Controller"));
  lcd.setCursor(0, 1);
  lcd.print(F("Initializing..."));
  delay(2000);
  lcd.clear();
}

void loop() {
  uint32_t currentMillis = millis();
  
  // Read sensor at specified intervals
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Measure Distance
    distance = getDistance();
    
    // Convert Distance to Litres (with bounds checking)
    if (distance > tankHeight) distance = tankHeight;
    
    litre = map(distance, tankHeight, 10, 0, tankCapacity);
    
    // Constrain litre value to valid range
    if (litre > tankCapacity) litre = tankCapacity;
    
    // Calculate percentage full
    uint8_t percentFull = map(litre, 0, tankCapacity, 0, 100);
    
    // Update display
    updateDisplay(litre, percentFull);
    
    // Motor Control
    controlMotor(distance);
    
    // Update alarm states
    updateAlarmStates(distance);
    
    // Log to serial (only when connected to save processing)
    if (Serial) {
      Serial.print(F("D:"));
      Serial.print(distance);
      Serial.print(F("cm W:"));
      Serial.print(litre);
      Serial.print(F("L "));
      Serial.print(percentFull);
      Serial.print(F("% M:"));
      Serial.print(READ_BIT(systemState, MOTOR_STATE) ? F("ON") : F("OFF"));
      Serial.print(F(" S:"));
      
      if (READ_BIT(systemState, CRITICAL_ZONE)) {
        Serial.println(F("CRIT"));
      } else if (READ_BIT(systemState, WARNING_ZONE)) {
        Serial.println(F("WARN"));
      } else {
        Serial.println(F("OK"));
      }
    }
  }
  
  // Handle buzzer patterns independently of sensor readings
  handleBuzzer(currentMillis);
}

// Function to Get Distance from Ultrasonic Sensor
uint8_t getDistance() {
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
void updateDisplay(uint8_t litre, uint8_t percentFull) {
  lcd.setCursor(0, 0);
  lcd.print(F("Level: "));
  lcd.print(litre);
  lcd.print(F("L ("));
  lcd.print(percentFull);
  lcd.print(F("%) "));
  
  lcd.setCursor(0, 1);
  
  // Display status message based on system state
  if (READ_BIT(systemState, CRITICAL_ZONE)) {
    lcd.print(F("CRITICAL LOW!"));
  } else if (READ_BIT(systemState, WARNING_ZONE)) {
    lcd.print(F("WARNING LOW! "));
  } else {
    lcd.print(F("Motor: "));
    lcd.print(READ_BIT(systemState, MOTOR_STATE) ? F("ON     ") : F("OFF    "));
  }
}

// Control the motor based on water level
void controlMotor(uint8_t distance) {
  if (distance > lowlvl) {
    if (READ_BIT(systemState, MOTOR_STATE)) {
      CLEAR_BIT(systemState, MOTOR_STATE);
      digitalWrite(relay, LOW);
    }
  } else if (distance < highlvl) {
    if (!READ_BIT(systemState, MOTOR_STATE)) {
      SET_BIT(systemState, MOTOR_STATE);
      digitalWrite(relay, HIGH);
    }
  }
}

// Update alarm states based on water level
void updateAlarmStates(uint8_t distance) {
  // Check for critical low water level
  if (distance > criticalLevel) {
    SET_BIT(systemState, CRITICAL_ZONE);
    CLEAR_BIT(systemState, WARNING_ZONE);
  }
  // Check for warning water level
  else if (distance > warningLevel) {
    SET_BIT(systemState, WARNING_ZONE);
    CLEAR_BIT(systemState, CRITICAL_ZONE);
  }
  else {
    CLEAR_BIT(systemState, WARNING_ZONE);
    CLEAR_BIT(systemState, CRITICAL_ZONE);
  }
}

// Handle buzzer patterns based on alarm states
void handleBuzzer(uint32_t currentMillis) {
  if (READ_BIT(systemState, CRITICAL_ZONE)) {
    // Continuous alarm for critical level
    digitalWrite(buzzer, HIGH);
  } 
  else if (READ_BIT(systemState, WARNING_ZONE)) {
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
