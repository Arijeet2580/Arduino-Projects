#include <SoftwareSerial.h> //Bluetooth Communication
#include <EEPROM.h> // For storing credentials

// Pin Definitions
const byte relay = 2;    // For Solenoid Lock
const byte redLED = 9;   // PWM pin for red LED
const byte greenLED = 10; // PWM pin for green LED
const byte buzzer = 4;   // Optional buzzer for audio feedback

// Communication
SoftwareSerial Bluetooth(11, 12); // RX, TX

// Security settings
String defaultPassword = "mango";
String defaultUserName = "mangoboy";
String currentPassword;
String currentUserName;
const int maxLoginAttempts = 3;
int loginAttempts = 0;
unsigned long lockoutEndTime = 0;
const unsigned long lockoutDuration = 30000; // 30 seconds lockout

// EEPROM addresses
const int passwordAddress = 0;
const int usernameAddress = 50;

// System state
boolean isLocked = true;
unsigned long lastActivity = 0;
const unsigned long autoLockTime = 30000; // Auto-lock after 30 seconds of inactivity

// LED states
unsigned long ledBlinkTime = 0;
const int blinkInterval = 500; // LED blink interval in ms
boolean ledState = false;

void setup() {
  Serial.begin(9600); // For debugging
  Bluetooth.begin(9600);
  
  // Initialize pins
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  // Initialize lock state
  digitalWrite(relay, LOW); // Default closed system
  
  // Load credentials from EEPROM or use defaults
  loadCredentials();
  
  // Initial LED indication - Red for locked
  Rled();
  
  // Welcome message
  Serial.println("Bluetooth Lock System Initialized");
  Serial.println("System is LOCKED");
}

void loadCredentials() {
  // Check if EEPROM has been initialized
  if (EEPROM.read(passwordAddress) != 255) {
    currentPassword = readStringFromEEPROM(passwordAddress);
    currentUserName = readStringFromEEPROM(usernameAddress);
  } else {
    // Use default credentials
    currentPassword = defaultPassword;
    currentUserName = defaultUserName;
    
    // Store defaults in EEPROM
    writeStringToEEPROM(passwordAddress, currentPassword);
    writeStringToEEPROM(usernameAddress, currentUserName);
  }
  
  Serial.println("Credentials loaded:");
  Serial.print("Username: ");
  Serial.println(currentUserName);
  Serial.print("Password: ");
  Serial.println(currentPassword);
}

void writeStringToEEPROM(int address, String data) {
  // Save length first
  EEPROM.write(address, data.length());
  
  // Then save each character
  for (int i = 0; i < data.length(); i++) {
    EEPROM.write(address + 1 + i, data[i]);
  }
}

String readStringFromEEPROM(int address) {
  // Read string length
  int length = EEPROM.read(address);
  
  // Read each character
  String data = "";
  for (int i = 0; i < length; i++) {
    data += char(EEPROM.read(address + 1 + i));
  }
  
  return data;
}

bool checkCredentials(String inputUser, String inputPass) {
  // Remove any trailing characters
  inputUser.trim();
  inputPass.trim();
  
  // Check for system admin command
  if (inputUser == "admin" && inputPass == "resetlock") {
    resetToDefault();
    return false; // Still report failure to prevent unlock
  }
  
  // Check username first
  if (inputUser != currentUserName) {
    Bluetooth.println("Invalid username!");
    return false;
  }
  
  // Then check password
  if (inputPass != currentPassword) {
    Bluetooth.println("Invalid password!");
    return false;
  }
  
  // Both matched
  return true;
}

void resetToDefault() {
  Bluetooth.println("SYSTEM RESET to default credentials");
  currentPassword = defaultPassword;
  currentUserName = defaultUserName;
  
  // Store in EEPROM
  writeStringToEEPROM(passwordAddress, currentPassword);
  writeStringToEEPROM(usernameAddress, currentUserName);
  
  // Alert via serial for debugging
  Serial.println("System reset to defaults");
}

void changePassword(String newPassword) {
  if (newPassword.length() < 4) {
    Bluetooth.println("Password too short! Minimum 4 characters.");
    return;
  }
  
  currentPassword = newPassword;
  writeStringToEEPROM(passwordAddress, currentPassword);
  Bluetooth.println("Password changed successfully!");
  Serial.println("Password changed");
}

void Rled() {
  analogWrite(redLED, 255);
  analogWrite(greenLED, 0);
}

void Gled() {
  analogWrite(redLED, 0);
  analogWrite(greenLED, 255);
}

void blinkRedLED() {
  if (millis() - ledBlinkTime >= blinkInterval) {
    ledBlinkTime = millis();
    ledState = !ledState;
    analogWrite(redLED, ledState ? 255 : 0);
  }
}

void LockOpen() {
  digitalWrite(relay, HIGH);
  Gled();
  beep(2); // Two short beeps
  isLocked = false;
  lastActivity = millis();
  Serial.println("Lock OPENED");
}

void LockClose() {
  digitalWrite(relay, LOW);
  Rled();
  beep(1); // One long beep
  isLocked = true;
  Serial.println("Lock CLOSED");
}

void beep(int pattern) {
  if (pattern == 1) { // Long beep
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
  } else if (pattern == 2) { // Two short beeps
    for (int i = 0; i < 2; i++) {
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(100);
    }
  } else if (pattern == 3) { // Alarm - three short beeps
    for (int i = 0; i < 3; i++) {
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(100);
    }
  }
}

void processCommand(String command) {
  command.trim();
  
  if (command == "open" && !isLocked) {
    // Already open
    Bluetooth.println("Lock is already open");
  }
  else if (command == "close" && isLocked) {
    // Already closed
    Bluetooth.println("Lock is already closed");
  }
  else if (command == "open" && isLocked) {
    // Request credentials to open
    Bluetooth.println("Authentication required to open lock");
    requestAuthentication();
  }
  else if (command == "close" && !isLocked) {
    // Close lock
    Bluetooth.println("Closing lock");
    LockClose();
  }
  else if (command == "status") {
    // Report status
    Bluetooth.print("Lock is currently ");
    Bluetooth.println(isLocked ? "LOCKED" : "UNLOCKED");
  }
  else if (command.startsWith("newpass:") && !isLocked) {
    // Change password - only when unlocked for security
    String newPass = command.substring(8);
    changePassword(newPass);
  }
  else if (command == "help") {
    // Show available commands
    Bluetooth.println("Available commands:");
    Bluetooth.println("open - Open the lock (requires authentication)");
    Bluetooth.println("close - Close the lock");
    Bluetooth.println("status - Check lock status");
    Bluetooth.println("newpass:xxxx - Change password (when unlocked)");
    Bluetooth.println("help - Show this help");
  }
  else {
    Bluetooth.println("Unknown command. Type 'help' for available commands.");
  }
}

void requestAuthentication() {
  // Check if in lockout period
  if (lockoutEndTime > millis()) {
    long secondsLeft = (lockoutEndTime - millis()) / 1000;
    Bluetooth.print("Too many failed attempts. Try again in ");
    Bluetooth.print(secondsLeft);
    Bluetooth.println(" seconds");
    return;
  }
  
  // Reset login attempts if lockout period has passed
  if (lockoutEndTime != 0 && lockoutEndTime < millis()) {
    loginAttempts = 0;
    lockoutEndTime = 0;
  }
  
  // Request username
  Bluetooth.print("Enter Username: ");
  String userInput = readBluetoothWithTimeout();
  
  if (userInput == "") {
    Bluetooth.println("No input received. Authentication canceled.");
    return;
  }
  
  Bluetooth.println(userInput);
  
  // Request password
  Bluetooth.print("Enter Password: ");
  String passInput = readBluetoothWithTimeout();
  
  if (passInput == "") {
    Bluetooth.println("No input received. Authentication canceled.");
    return;
  }
  
  // Print asterisks instead of the actual password for security
  Bluetooth.println("*".repeat(passInput.length()));
  
  // Check credentials
  if (checkCredentials(userInput, passInput)) {
    Bluetooth.println("Valid User - Access Granted");
    loginAttempts = 0; // Reset attempts on success
    LockOpen();
  } else {
    loginAttempts++;
    Bluetooth.print("Invalid credentials. Attempt ");
    Bluetooth.print(loginAttempts);
    Bluetooth.print(" of ");
    Bluetooth.println(maxLoginAttempts);
    
    // Check if max attempts reached
    if (loginAttempts >= maxLoginAttempts) {
      lockoutEndTime = millis() + lockoutDuration;
      Bluetooth.print("Too many failed attempts. Locked out for ");
      Bluetooth.print(lockoutDuration / 1000);
      Bluetooth.println(" seconds");
      beep(3); // Alarm pattern
    }
    
    LockClose();
  }
}

String readBluetoothWithTimeout() {
  String received = "";
  unsigned long startTime = millis();
  const unsigned long timeout = 30000; // 30 seconds timeout
  
  while (millis() - startTime < timeout) {
    if (Bluetooth.available() > 0) {
      char inChar = Bluetooth.read();
      
      // Check for end of input (newline or carriage return)
      if (inChar == '\n' || inChar == '\r') {
        if (received.length() > 0) {
          break;
        }
      } else {
        received += inChar;
      }
    }
    
    // Update any blinking if needed during wait
    if (lockoutEndTime > millis()) {
      blinkRedLED();
    }
  }
  
  return received;
}

void loop() {
  // Check for auto-lock timeout
  if (!isLocked && (millis() - lastActivity > autoLockTime)) {
    Bluetooth.println("Auto-locking due to inactivity");
    LockClose();
  }
  
  // Handle lockout period - blink red LED
  if (lockoutEndTime > millis()) {
    blinkRedLED();
  }
  
  // Check for incoming Bluetooth data
  if (Bluetooth.available() > 0) {
    String input = Bluetooth.readString();
    input.trim();
    
    // Update activity timestamp
    lastActivity = millis();
    
    // Process the command
    processCommand(input);
  }
}
