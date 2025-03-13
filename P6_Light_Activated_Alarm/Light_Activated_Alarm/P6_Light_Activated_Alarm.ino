// Define pins for components
const int ldrPin = A0;     // LDR (Light Dependent Resistor) connected to analog pin A0
const int buzzerPin = 9;   // Buzzer connected to digital pin 9
const int ledPin = 13;     // Built-in LED for visual indication

// Variables
const int lightThreshold = 500;  // Adjust this value based on your environment
int lightLevel;                  // Variable to store light readings

// Timer for non-blocking alarm
unsigned long previousMillis = 0;
const long interval = 500; // Alarm interval
bool alarmState = false;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  
  // Set pin modes
  pinMode(ldrPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Read light level from LDR
  lightLevel = analogRead(ldrPin);

  // Print light level to Serial Monitor for debugging
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  // Check if light level is above threshold
  if (lightLevel > lightThreshold) {
    triggerAlarm();
  } else {
    stopAlarm();
  }

  // Small delay to prevent too frequent readings
  delay(100);
}

void triggerAlarm() {
  unsigned long currentMillis = millis();

  // Non-blocking beep pattern
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    alarmState = !alarmState; // Toggle state

    if (alarmState) {
      tone(buzzerPin, 1000);  // 1kHz tone
      digitalWrite(ledPin, HIGH);
    } else {
      noTone(buzzerPin);
      digitalWrite(ledPin, LOW);
    }
  }
}

void stopAlarm() {
  noTone(buzzerPin);
  digitalWrite(ledPin, LOW);
  alarmState = false; // Reset the alarm state
}
