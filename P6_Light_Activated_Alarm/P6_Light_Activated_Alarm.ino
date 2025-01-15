// Define pins for components
const int ldrPin = A0;     // LDR (Light Dependent Resistor) connected to analog pin A0
const int buzzerPin = 9;   // Buzzer connected to digital pin 9
const int ledPin = 13;     // Built-in LED for visual indication

// Variables
const int lightThreshold = 500;  // Adjust this value based on your environment
int lightLevel;                  // Variable to store light readings

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Set pin modes
  pinMode(ldrPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // Initial delay to let the system stabilize
  delay(1000);
}

void loop() {
  // Read light level from LDR
  lightLevel = analogRead(ldrPin);
  
  // Print light level to Serial Monitor for debugging
  Serial.print("Light Level: ");
  Serial.println(lightLevel);
  
  // Check if light level is above threshold
  if (lightLevel > lightThreshold) {
    // Trigger alarm
    triggerAlarm();
  } else {
    // System normal - no alarm
    stopAlarm();
  }
  
  // Small delay to prevent too frequent readings
  delay(100);
}

void triggerAlarm() {
  // Activate buzzer with a beeping pattern
  tone(buzzerPin, 1000);  // 1kHz tone
  digitalWrite(ledPin, HIGH);
  delay(500);
  noTone(buzzerPin);
  digitalWrite(ledPin, LOW);
  delay(500);
}

void stopAlarm() {
  // Ensure buzzer and LED are off
  noTone(buzzerPin);
  digitalWrite(ledPin, LOW);
}
