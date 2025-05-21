// Updated for ESP32 and optimized for memory
#include <EEPROM.h>
#include "BluetoothSerial.h"

// Use BluetoothSerial for ESP32
BluetoothSerial Bluetooth;

// Pin Definitions
const byte RELAY_PIN = 2;
const byte RED_LED_PIN = 9;
const byte GREEN_LED_PIN = 10;
const byte BUZZER_PIN = 4;

// EEPROM Addresses
constexpr int PASSWORD_ADDR = 0;
constexpr int USERNAME_ADDR = 50;

// Credentials
const char* DEFAULT_USER = "mangoboy";
const char* DEFAULT_PASS = "mango";
String currentUser;
String currentPass;

// State and Timers
constexpr int MAX_ATTEMPTS = 3;
constexpr unsigned long LOCKOUT_TIME = 30000;
constexpr unsigned long AUTOLOCK_TIME = 30000;
int attempts = 0;
unsigned long lockoutUntil = 0;
unsigned long lastAction = 0;

// Lock state
bool isLocked = true;
bool ledOn = false;
unsigned long ledTimer = 0;
constexpr unsigned long BLINK_INTERVAL = 500;

// Function Prototypes
void loadCredentials();
void writeStringEEPROM(int addr, const String& str);
String readStringEEPROM(int addr);
void beep(int type);
void lock();
void unlock();
void blinkRed();
void handleCommand(const String& cmd);
String readWithTimeout();
void authenticate();
void resetDefaults();
void changePassword(const String& newPass);
String repeatChar(char c, int count);

void setup() {
  Serial.begin(115200);
  Bluetooth.begin("ESP32_Lock");

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  loadCredentials();
  analogWrite(RED_LED_PIN, 255);
  analogWrite(GREEN_LED_PIN, 0);

  Serial.println("ESP32 Bluetooth Lock Initialized - Locked");
}

void loop() {
  if (!isLocked && millis() - lastAction > AUTOLOCK_TIME) {
    Bluetooth.println("Auto-locking due to inactivity");
    lock();
  }

  if (lockoutUntil > millis()) blinkRed();

  if (Bluetooth.available()) {
    String input = Bluetooth.readStringUntil('\n');
    input.trim();
    lastAction = millis();
    handleCommand(input);
  }
}

void loadCredentials() {
  if (EEPROM.read(PASSWORD_ADDR) != 255) {
    currentPass = readStringEEPROM(PASSWORD_ADDR);
    currentUser = readStringEEPROM(USERNAME_ADDR);
  } else {
    currentUser = DEFAULT_USER;
    currentPass = DEFAULT_PASS;
    writeStringEEPROM(PASSWORD_ADDR, currentPass);
    writeStringEEPROM(USERNAME_ADDR, currentUser);
  }
}

void writeStringEEPROM(int addr, const String& str) {
  int len = str.length();
  EEPROM.write(addr, len);
  for (int i = 0; i < len; i++) EEPROM.write(addr + 1 + i, str[i]);
  EEPROM.commit();
}

String readStringEEPROM(int addr) {
  int len = EEPROM.read(addr);
  String data = "";
  for (int i = 0; i < len; i++) data += char(EEPROM.read(addr + 1 + i));
  return data;
}

void lock() {
  digitalWrite(RELAY_PIN, LOW);
  analogWrite(RED_LED_PIN, 255);
  analogWrite(GREEN_LED_PIN, 0);
  beep(1);
  isLocked = true;
  Serial.println("Locked");
}

void unlock() {
  digitalWrite(RELAY_PIN, HIGH);
  analogWrite(RED_LED_PIN, 0);
  analogWrite(GREEN_LED_PIN, 255);
  beep(2);
  isLocked = false;
  lastAction = millis();
  Serial.println("Unlocked");
}

void beep(int type) {
  for (int i = 0; i < type; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
}

void blinkRed() {
  if (millis() - ledTimer > BLINK_INTERVAL) {
    ledTimer = millis();
    ledOn = !ledOn;
    analogWrite(RED_LED_PIN, ledOn ? 255 : 0);
  }
}

void handleCommand(const String& cmd) {
  if (cmd == "status") {
    Bluetooth.println(isLocked ? "LOCKED" : "UNLOCKED");
  } else if (cmd == "open") {
    if (!isLocked) Bluetooth.println("Already unlocked");
    else authenticate();
  } else if (cmd == "close") {
    if (isLocked) Bluetooth.println("Already locked");
    else {
      Bluetooth.println("Closing lock");
      lock();
    }
  } else if (cmd.startsWith("newpass:") && !isLocked) {
    changePassword(cmd.substring(8));
  } else if (cmd == "help") {
    Bluetooth.println("Commands: open, close, status, newpass:<new>, help");
  } else {
    Bluetooth.println("Unknown command. Type 'help'");
  }
}

void authenticate() {
  if (lockoutUntil > millis()) {
    Bluetooth.printf("Try again in %lu seconds\n", (lockoutUntil - millis()) / 1000);
    return;
  }

  if (lockoutUntil && lockoutUntil < millis()) {
    attempts = 0;
    lockoutUntil = 0;
  }

  Bluetooth.println("Enter Username:");
  String user = readWithTimeout();
  if (user == "") return;
  Bluetooth.println(user);

  Bluetooth.println("Enter Password:");
  String pass = readWithTimeout();
  if (pass == "") return;
  Bluetooth.println(repeatChar('*', pass.length()));

  if (user == "admin" && pass == "resetlock") {
    resetDefaults();
    return;
  }

  if (user == currentUser && pass == currentPass) {
    attempts = 0;
    Bluetooth.println("Access Granted");
    unlock();
  } else {
    attempts++;
    Bluetooth.printf("Invalid (%d/%d)\n", attempts, MAX_ATTEMPTS);
    if (attempts >= MAX_ATTEMPTS) {
      lockoutUntil = millis() + LOCKOUT_TIME;
      Bluetooth.println("Too many attempts. Locked out.");
      beep(3);
    }
    lock();
  }
}

String readWithTimeout() {
  String data = "";
  unsigned long start = millis();
  while (millis() - start < 30000) {
    if (Bluetooth.available()) {
      char c = Bluetooth.read();
      if (c == '\n' || c == '\r') {
        if (data.length()) break;
      } else {
        data += c;
      }
    }
    if (lockoutUntil > millis()) blinkRed();
  }
  return data;
}

void resetDefaults() {
  currentUser = DEFAULT_USER;
  currentPass = DEFAULT_PASS;
  writeStringEEPROM(USERNAME_ADDR, currentUser);
  writeStringEEPROM(PASSWORD_ADDR, currentPass);
  Bluetooth.println("System reset to default credentials");
}

void changePassword(const String& newPass) {
  if (newPass.length() < 4) {
    Bluetooth.println("Password too short");
    return;
  }
  currentPass = newPass;
  writeStringEEPROM(PASSWORD_ADDR, currentPass);
  Bluetooth.println("Password changed successfully");
}

// Helper function to repeat a character
String repeatChar(char c, int count) {
  String result = "";
  for (int i = 0; i < count; i++) {
    result += c;
  }
  return result;
}
