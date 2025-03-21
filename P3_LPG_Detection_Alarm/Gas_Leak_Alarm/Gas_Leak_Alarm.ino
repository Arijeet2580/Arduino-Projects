#include <Servo.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

// System Configuration
bool debugMode = false;          // Debug mode flag, toggled by button

// Pin Definitions
const byte RX = 6, TX = 7;        // Bluetooth pins
const byte rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;  // LCD pins
const byte GAS_SENSOR = A0;       // MQ-5 Gas sensor pin
const byte BUZZER = 5;            // Buzzer pin
const byte SERVO = 4;             // Servo motor pin
const byte LED_RED = 3;           // Red LED pin
const byte LED_GREEN = 2;         // Green LED pin
const byte DEBUG_BUTTON = A1;     // Debug button pin

// System Constants
const uint16_t DANGER_THRESHOLD = 700; // Gas danger threshold (analog value)
const uint16_t WARNING_THRESHOLD = 500; // Gas warning threshold
const uint16_t SERVO_OPEN_ANGLE = 100; // Angle to open window
const uint8_t SERVO_CLOSED_ANGLE = 0; // Angle to close window
const uint8_t EEPROM_CALIB_ADDR = 0;  // EEPROM address for calibration value
const uint8_t EEPROM_DEBUG_ADDR = 10; // EEPROM address for debug settings
const unsigned long LCD_REFRESH_RATE = 1000; // LCD refresh in ms
const unsigned long BT_REFRESH_RATE = 3000;  // Bluetooth refresh in ms
const unsigned long DEBUG_INTERVAL = 500;    // Debug data refresh rate

// Gas calculation constants
const float AMBIENT_VALUE = 60.0; // Sensor reading in ambient air
const float PPM_FACTOR = 1.351;   // Conversion factor for PPM calculation

// Alarm tones (Hz)
const int alarmTones[] = {440, 494, 523, 672};
const int numTones = 4;

// Button debounce
const unsigned long DEBOUNCE_DELAY = 50;
unsigned long lastDebounceTime = 0;
int lastButtonState = HIGH;
int buttonState = HIGH;

// Objects
SoftwareSerial Bluetooth(RX, TX);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Servo Servo1;

// Variables
uint16_t gasValue = 0;
uint16_t ppm = 0;
bool windowOpen = false;
bool inDangerState = false;
bool inWarningState = false;
unsigned long lastLcdUpdate = 0;
unsigned long lastBtUpdate = 0;
unsigned long lastDebugUpdate = 0;
unsigned long lastToneChange = 0;
uint16_t currentTone = 0;
float calibrationFactor = PPM_FACTOR; // Default value

// System Statistics
unsigned long systemStartTime = 0;
unsigned long lastAlarmTime = 0;
uint16_t alarmCount = 0;
uint16_t windowOperations = 0;
uint16_t maxGasReading = 0;

// Function Prototypes
void displayStatus();
void handleGasLevel();
void playAlarm(bool continuous);
void stopAlarm();
void openWindow();
void closeWindow();
void calibrateSensor();
void processCommand(String command);
void handleDebugButton();
void toggleDebugMode();
void saveDebugMode();
void loadDebugMode();
void displayDebugInfo();
void runSelfTest();
void logEvent(String event);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("Gas Detection System Starting");
  
  // Initialize Bluetooth
  Bluetooth.begin(9600);
  
  // Initialize LCD
  lcd.begin(16, 2);
  
  // Configure pins
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(DEBUG_BUTTON, INPUT_PULLUP); // Use internal pull-up resistor
  
  // Attach servo
  Servo1.attach(SERVO);
  Servo1.write(SERVO_CLOSED_ANGLE);
  
  // Load settings from EEPROM
  loadSettings();
  
  // Display welcome message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas Detection");
  lcd.setCursor(0, 1);
  lcd.print("System v2.1");
  delay(2000);
  
  // Initial state
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  
  // Record system start time
  systemStartTime = millis();
  
  // Startup message
  Bluetooth.println("Gas Detection System Online");
  Bluetooth.println("Type 'help' for commands");
  
  // Self-test if debug mode is active
  if (debugMode) {
    runSelfTest();
  }
  
  // Log startup event
  logEvent("System started");
}

void loop() {
  // Check debug button
  handleDebugButton();
  
  // Read gas sensor value
  gasValue = analogRead(GAS_SENSOR);
  
  // Update max reading if needed
  if (gasValue > maxGasReading) {
    maxGasReading = gasValue;
  }
  
  // Calculate PPM with calibration factor
  ppm = calibrationFactor * (gasValue - AMBIENT_VALUE);
  if (ppm < 0) ppm = 0; // Prevent negative PPM values
  
  // Handle gas levels and appropriate responses
  handleGasLevel();
  
  // Update LCD at specified intervals
  if (millis() - lastLcdUpdate >= LCD_REFRESH_RATE) {
    lastLcdUpdate = millis();
    
    if (debugMode) {
      displayDebugInfo();
    } else {
      displayStatus();
    }
  }
  
  // Send data to Bluetooth at specified intervals
  if (millis() - lastBtUpdate >= BT_REFRESH_RATE) {
    lastBtUpdate = millis();
    
    if (debugMode) {
      // Send more detailed data in debug mode
      Bluetooth.println("---DEBUG DATA---");
      Bluetooth.print("Gas Raw: ");
      Bluetooth.print(gasValue);
      Bluetooth.print(" | PPM: ");
      Bluetooth.println(ppm);
      Bluetooth.print("Max Gas: ");
      Bluetooth.print(maxGasReading);
      Bluetooth.print(" | Window: ");
      Bluetooth.println(windowOpen ? "Open" : "Closed");
      Bluetooth.print("Alarms: ");
      Bluetooth.print(alarmCount);
      Bluetooth.print(" | Window Ops: ");
      Bluetooth.println(windowOperations);
      Bluetooth.print("Uptime: ");
      Bluetooth.print((millis() - systemStartTime) / 1000);
      Bluetooth.println(" seconds");
      Bluetooth.println("-------------");
    } else {
      // Standard data output
      Bluetooth.print("Gas Level: ");
      Bluetooth.print(ppm);
      Bluetooth.print(" PPM (Raw: ");
      Bluetooth.print(gasValue);
      Bluetooth.println(")");
      Bluetooth.print("Status: ");
      
      if (inDangerState) {
        Bluetooth.println("DANGER - Window Open");
      } else if (inWarningState) {
        Bluetooth.println("WARNING - Monitor");
      } else {
        Bluetooth.println("Normal");
      }
    }
  }
  
  // Process any Bluetooth commands
  if (Bluetooth.available()) {
    String command = Bluetooth.readStringUntil('\n');
    processCommand(command);
  }
}

void displayStatus() {
  lcd.clear();
  
  // First line: PPM and raw value
  lcd.setCursor(0, 0);
  lcd.print("PPM:");
  lcd.print(ppm);
  lcd.print(" Raw:");
  lcd.print(gasValue);
  
  // Second line: Status message
  lcd.setCursor(0, 1);
  
  if (inDangerState) {
    lcd.print("DANGER! LPG LEAK");
  } else if (inWarningState) {
    lcd.print("WARNING! Check Gas");
  } else {
    lcd.print("Normal - Safe");
  }
}

void handleGasLevel() {
  // DANGER level
  if (gasValue > DANGER_THRESHOLD) {
    // First time entering danger state
    if (!inDangerState) {
      inDangerState = true;
      inWarningState = false;
      openWindow();
      alarmCount++;
      lastAlarmTime = millis();
      Bluetooth.println("ALERT: Dangerous gas level detected!");
      Bluetooth.println("Window opened for ventilation");
      logEvent("DANGER: Gas level " + String(gasValue) + " (PPM: " + String(ppm) + ")");
    }
    
    // Continue alarm while in danger
    playAlarm(true);
    
    // Visual indicators
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
  }
  // WARNING level
  else if (gasValue > WARNING_THRESHOLD) {
    // First time entering warning state
    if (!inWarningState && !inDangerState) {
      inWarningState = true;
      alarmCount++;
      lastAlarmTime = millis();
      Bluetooth.println("WARNING: Elevated gas level detected");
      logEvent("WARNING: Gas level " + String(gasValue) + " (PPM: " + String(ppm) + ")");
    }
    
    // Intermittent alarm for warning
    playAlarm(false);
    
    // Visual indicators - blinking red
    digitalWrite(LED_RED, (millis() / 500) % 2);
    digitalWrite(LED_GREEN, LOW);
  }
  // NORMAL level
  else {
    // If returning to normal from danger/warning
    if (inDangerState || inWarningState) {
      inDangerState = false;
      inWarningState = false;
      stopAlarm();
      
      // Close window only if it was danger state
      if (windowOpen) {
        closeWindow();
        Bluetooth.println("Gas levels normalized. Window closed");
        logEvent("NORMAL: Gas levels normalized. Window closed.");
      } else {
        Bluetooth.println("Gas levels returned to normal");
        logEvent("NORMAL: Gas levels returned to normal");
      }
    }
    
    // Visual indicators
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }
}

void playAlarm(bool continuous) {
  // Continuous alarm for danger
  if (continuous) {
    // Cycle through alarm tones
    if (millis() - lastToneChange > 300) {
      lastToneChange = millis();
      tone(BUZZER, alarmTones[currentTone]);
      currentTone = (currentTone + 1) % numTones;
    }
  }
  // Intermittent alarm for warning
  else {
    // Beep pattern: on for 100ms, off for 1900ms
    unsigned long cycle = millis() % 2000;
    if (cycle < 100) {
      tone(BUZZER, alarmTones[0]);
    } else {
      noTone(BUZZER);
    }
  }
}

void stopAlarm() {
  noTone(BUZZER);
}

void openWindow() {
  Servo1.write(SERVO_OPEN_ANGLE);
  windowOpen = true;
  windowOperations++;
  Serial.println("Window opened");
}

void closeWindow() {
  Servo1.write(SERVO_CLOSED_ANGLE);
  windowOpen = false;
  windowOperations++;
  Serial.println("Window closed");
}

void calibrateSensor() {
  Bluetooth.println("Starting calibration...");
  Bluetooth.println("Please ensure sensor is in clean air");
  Bluetooth.println("Wait 10 seconds...");
  
  // Countdown
  for (int i = 10; i > 0; i--) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calibrating...");
    lcd.setCursor(0, 1);
    lcd.print("Wait ");
    lcd.print(i);
    lcd.print(" seconds");
    
    Bluetooth.print(i);
    Bluetooth.println(" seconds remaining");
    delay(1000);
  }
  
  // Take multiple readings for better accuracy
  int sum = 0;
  int samples = 10;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Taking samples");
  
  for (int i = 0; i < samples; i++) {
    sum += analogRead(GAS_SENSOR);
    lcd.setCursor(i, 1);
    lcd.print(".");
    delay(200);
  }
  
  int avgReading = sum / samples;
  
  // Calculate new calibration factor
  // Assuming 0 PPM at ambient air
  if (avgReading > AMBIENT_VALUE) {
    calibrationFactor = 1.0 / (avgReading - AMBIENT_VALUE) * 100;
    if (calibrationFactor <= 0 || calibrationFactor > 10) {
      calibrationFactor = PPM_FACTOR; // Use default if result is unreasonable
      Bluetooth.println("Calibration failed! Using default value");
      logEvent("Calibration failed - using default");
    } else {
      // Save to EEPROM (multiplied by 100 to store as int)
      EEPROM.write(EEPROM_CALIB_ADDR, (int)(calibrationFactor * 100));
      Bluetooth.println("Calibration successful!");
      Bluetooth.print("New calibration factor: ");
      Bluetooth.println(calibrationFactor);
      logEvent("Calibration successful: " + String(calibrationFactor));
    }
  } else {
    Bluetooth.println("Calibration failed! Sensor reading too low");
    logEvent("Calibration failed - reading too low");
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibration");
  lcd.setCursor(0, 1);
  lcd.print("Complete!");
  delay(2000);
}

void processCommand(String command) {
  command.trim();
  
  Serial.print("Command received: ");
  Serial.println(command);
  
  if (command == "help") {
    Bluetooth.println("Available commands:");
    Bluetooth.println("status - Show system status");
    Bluetooth.println("open - Open window manually");
    Bluetooth.println("close - Close window manually");
    Bluetooth.println("calibrate - Calibrate gas sensor");
    Bluetooth.println("reset - Reset system");
    Bluetooth.println("thresholds - Show alarm thresholds");
    Bluetooth.println("debug - Toggle debug mode");
    Bluetooth.println("test - Run system self-test");
    Bluetooth.println("stats - Show system statistics");
  }
  else if (command == "status") {
    Bluetooth.println("System Status:");
    Bluetooth.print("Gas Level: ");
    Bluetooth.print(ppm);
    Bluetooth.println(" PPM");
    Bluetooth.print("Raw Sensor Value: ");
    Bluetooth.println(gasValue);
    Bluetooth.print("Window: ");
    Bluetooth.println(windowOpen ? "Open" : "Closed");
    Bluetooth.print("Alarm State: ");
    if (inDangerState) Bluetooth.println("DANGER");
    else if (inWarningState) Bluetooth.println("WARNING");
    else Bluetooth.println("Normal");
    Bluetooth.print("Debug Mode: ");
    Bluetooth.println(debugMode ? "ON" : "OFF");
  }
  else if (command == "open") {
    openWindow();
    Bluetooth.println("Window opened manually");
    logEvent("Window opened manually via BT");
  }
  else if (command == "close") {
    closeWindow();
    Bluetooth.println("Window closed manually");
    logEvent("Window closed manually via BT");
  }
  else if (command == "calibrate") {
    calibrateSensor();
  }
  else if (command == "reset") {
    Bluetooth.println("Resetting system...");
    closeWindow();
    stopAlarm();
    inDangerState = false;
    inWarningState = false;
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    Bluetooth.println("System reset complete");
    logEvent("System reset via BT command");
  }
  else if (command == "thresholds") {
    Bluetooth.println("Alarm Thresholds:");
    Bluetooth.print("Warning Level: ");
    Bluetooth.print(WARNING_THRESHOLD);
    Bluetooth.print(" (");
    Bluetooth.print(calibrationFactor * (WARNING_THRESHOLD - AMBIENT_VALUE));
    Bluetooth.println(" PPM)");
    Bluetooth.print("Danger Level: ");
    Bluetooth.print(DANGER_THRESHOLD);
    Bluetooth.print(" (");
    Bluetooth.print(calibrationFactor * (DANGER_THRESHOLD - AMBIENT_VALUE));
    Bluetooth.println(" PPM)");
  }
  else if (command == "debug") {
    toggleDebugMode();
    Bluetooth.print("Debug mode ");
    Bluetooth.println(debugMode ? "enabled" : "disabled");
    logEvent("Debug mode " + String(debugMode ? "enabled" : "disabled") + " via BT");
  }
  else if (command == "test") {
    Bluetooth.println("Running system self-test...");
    runSelfTest();
    logEvent("Self-test executed via BT");
  }
  else if (command == "stats") {
    Bluetooth.println("System Statistics:");
    Bluetooth.print("Uptime: ");
    unsigned long uptime = millis() - systemStartTime;
    Bluetooth.print(uptime / 1000 / 60 / 60); // hours
    Bluetooth.print(" hours, ");
    Bluetooth.print((uptime / 1000 / 60) % 60); // minutes
    Bluetooth.print(" minutes, ");
    Bluetooth.print((uptime / 1000) % 60); // seconds
    Bluetooth.println(" seconds");
    
    Bluetooth.print("Alarms triggered: ");
    Bluetooth.println(alarmCount);
    
    Bluetooth.print("Window operations: ");
    Bluetooth.println(windowOperations);
    
    Bluetooth.print("Maximum gas reading: ");
    Bluetooth.print(maxGasReading);
    Bluetooth.print(" (");
    Bluetooth.print(calibrationFactor * (maxGasReading - AMBIENT_VALUE));
    Bluetooth.println(" PPM)");
    
    if (lastAlarmTime > 0) {
      Bluetooth.print("Time since last alarm: ");
      unsigned long timeSinceAlarm = (millis() - lastAlarmTime) / 1000;
      Bluetooth.print(timeSinceAlarm / 60 / 60); // hours
      Bluetooth.print(" hours, ");
      Bluetooth.print((timeSinceAlarm / 60) % 60); // minutes
      Bluetooth.print(" minutes, ");
      Bluetooth.print(timeSinceAlarm % 60); // seconds
      Bluetooth.println(" seconds");
    } else {
      Bluetooth.println("No alarms since startup");
    }
  }
  else {
    Bluetooth.println("Unknown command. Type 'help' for available commands");
  }
}

void handleDebugButton() {
  // Read the current button state
  int reading = digitalRead(DEBUG_BUTTON);
  
  // Check if the button state changed
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  
  // If the button state has been stable for the debounce delay
  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    // If the button state has changed
    if (reading != buttonState) {
      buttonState = reading;
      
      // If the button is pressed (LOW because of INPUT_PULLUP)
      if (buttonState == LOW) {
        toggleDebugMode();
        Serial.print("Debug mode toggled by button to: ");
        Serial.println(debugMode ? "ON" : "OFF");
        logEvent("Debug mode " + String(debugMode ? "enabled" : "disabled") + " via button");
      }
    }
  }
  
  // Save the reading for the next loop
  lastButtonState = reading;
}

void toggleDebugMode() {
  debugMode = !debugMode;
  saveDebugMode();
  
  // Beep to indicate mode change
  if (debugMode) {
    // Two beeps for debug ON
    tone(BUZZER, 880, 100);
    delay(150);
    tone(BUZZER, 880, 100);
  } else {
    // One beep for debug OFF
    tone(BUZZER, 440, 200);
  }
  
  // Clear LCD immediately
  lcd.clear();
  if (debugMode) {
    lcd.setCursor(0, 0);
    lcd.print("DEBUG MODE ON");
    lcd.setCursor(0, 1);
    lcd.print("Press for normal");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Normal mode");
    lcd.setCursor(0, 1);
    lcd.print("System ready");
  }
  delay(1000);
}

void saveDebugMode() {
  EEPROM.write(EEPROM_DEBUG_ADDR, debugMode ? 1 : 0);
}

void loadSettings() {
  // Load calibration factor
  if (EEPROM.read(EEPROM_CALIB_ADDR) != 255) { // Check if EEPROM has been written to
    calibrationFactor = EEPROM.read(EEPROM_CALIB_ADDR) / 100.0;
    Serial.print("Loaded calibration factor: ");
    Serial.println(calibrationFactor);
  }
  
  // Load debug mode setting
  if (EEPROM.read(EEPROM_DEBUG_ADDR) != 255) {
    debugMode = (EEPROM.read(EEPROM_DEBUG_ADDR) == 1);
    Serial.print("Loaded debug mode: ");
    Serial.println(debugMode ? "ON" : "OFF");
  }
}

void displayDebugInfo() {
  lcd.clear();
  
  // Rotate between different debug screens
  unsigned long displayCycle = (millis() / 3000) % 3;
  
  if (displayCycle == 0) {
    // Screen 1: Sensor values
    lcd.setCursor(0, 0);
    lcd.print("Raw:");
    lcd.print(gasValue);
    lcd.print(" Max:");
    lcd.print(maxGasReading);
    
    lcd.setCursor(0, 1);
    lcd.print("PPM:");
    lcd.print(ppm);
    lcd.print(" Cal:");
    lcd.print(calibrationFactor);
  }
  else if (displayCycle == 1) {
    // Screen 2: System stats
    lcd.setCursor(0, 0);
    lcd.print("Alarms:");
    lcd.print(alarmCount);
    lcd.print(" Win:");
    lcd.print(windowOperations);
    
    lcd.setCursor(0, 1);
    lcd.print("Up:");
    unsigned long uptime = (millis() - systemStartTime) / 1000 / 60; // minutes
    lcd.print(uptime);
    lcd.print("m");
    lcd.print(" D:");
    lcd.print(debugMode ? "ON" : "OFF");
  }
  else {
    // Screen 3: Thresholds
    lcd.setCursor(0, 0);
    lcd.print("Wrn:");
    lcd.print(WARNING_THRESHOLD);
    lcd.print(" Dng:");
    lcd.print(DANGER_THRESHOLD);
    
    lcd.setCursor(0, 1);
    lcd.print("State:");
    if (inDangerState) lcd.print("DANGER");
    else if (inWarningState) lcd.print("WARNING");
    else lcd.print("NORMAL");
  }
}

void runSelfTest() {
  Bluetooth.println("Starting self-test sequence");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Self-Test Mode");
  lcd.setCursor(0, 1);
  lcd.print("Testing LEDs...");
  
  // Test LEDs
  Bluetooth.println("Testing LEDs...");
  
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  delay(500);
  
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  delay(500);
  
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  delay(500);
  
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  delay(500);
  
  // Restore normal LED state
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  
  // Test buzzer
  lcd.setCursor(0, 1);
  lcd.print("Testing buzzer...");
  Bluetooth.println("Testing buzzer...");
  
  for (int i = 0; i < numTones; i++) {
    tone(BUZZER, alarmTones[i], 200);
    delay(300);
  }
  
  // Test servo
  lcd.setCursor(0, 1);
  lcd.print("Testing window... ");
  Bluetooth.println("Testing window operation...");
  
  openWindow();
  delay(1000);
  closeWindow();
  
  // Test gas sensor
  lcd.setCursor(0, 1);
  lcd.print("Testing sensor... ");
  Bluetooth.println("Testing gas sensor...");
  
  int sensorReading = analogRead(GAS_SENSOR);
  Bluetooth.print("Current gas reading: ");
  Bluetooth.println(sensorReading);
  
  if (sensorReading < 10) {
    Bluetooth.println("WARNING: Sensor reading very low, may be disconnected");
  } else if (sensorReading > 900) {
    Bluetooth.println("WARNING: Sensor reading very high, may be faulty or gas present");
  } else {
    Bluetooth.println("Sensor reading in normal range");
  }
  
  // Test complete
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Self-Test Done");
  lcd.setCursor(0, 1);
  lcd.print("All systems ready");
  
  Bluetooth.println("Self-test complete");
  Bluetooth.println("System ready for operation");
  
  delay(2000);
}

void logEvent(String event) {
  // Print to serial for debugging/logging
  Serial.print("EVENT [");
  Serial.print(millis() / 1000);
  Serial.print("s]: ");
  Serial.println(event);
}
