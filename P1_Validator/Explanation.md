# ESP32 Bluetooth Lock Code Explanation

## Headers and Library Includes
```cpp
#include <EEPROM.h>
#include "BluetoothSerial.h"
```
- **EEPROM.h**: Provides persistent storage functionality to save credentials even after power loss
- **BluetoothSerial.h**: Enables Bluetooth communication for ESP32, allowing wireless control

## Global Objects
```cpp
BluetoothSerial Bluetooth;
```
Creates a Bluetooth serial communication object for wireless connectivity.

## Pin Definitions
```cpp
const byte RELAY_PIN = 2;
const byte RED_LED_PIN = 9;
const byte GREEN_LED_PIN = 10;
const byte BUZZER_PIN = 4;
```
Defines hardware connections:
- **RELAY_PIN**: Controls the physical lock mechanism
- **RED_LED_PIN**: Indicates locked state or errors
- **GREEN_LED_PIN**: Indicates unlocked state
- **BUZZER_PIN**: Provides audio feedback

## EEPROM Memory Addresses
```cpp
constexpr int PASSWORD_ADDR = 0;
constexpr int USERNAME_ADDR = 50;
```
Defines memory locations for storing credentials persistently:
- Password stored starting at address 0
- Username stored starting at address 50

## Default Credentials
```cpp
const char* DEFAULT_USER = "mangoboy";
const char* DEFAULT_PASS = "mango";
String currentUser;
String currentPass;
```
Sets factory default login credentials and variables to hold current credentials.

## Security and Timing Constants
```cpp
constexpr int MAX_ATTEMPTS = 3;
constexpr unsigned long LOCKOUT_TIME = 30000;
constexpr unsigned long AUTOLOCK_TIME = 30000;
```
- **MAX_ATTEMPTS**: Maximum failed login attempts before lockout
- **LOCKOUT_TIME**: 30-second penalty period after too many failed attempts
- **AUTOLOCK_TIME**: 30 seconds of inactivity before automatic locking

## State Variables
```cpp
int attempts = 0;
unsigned long lockoutUntil = 0;
unsigned long lastAction = 0;
bool isLocked = true;
bool ledOn = false;
unsigned long ledTimer = 0;
constexpr unsigned long BLINK_INTERVAL = 500;
```
Tracks system state:
- Failed login attempt counter
- Lockout expiration timestamp
- Last user activity timestamp
- Current lock state
- LED blinking state and timing

## Setup Function
```cpp
void setup() {
  // Initialize serial communication and Bluetooth
  // Configure GPIO pins
  // Load saved credentials
  // Set initial locked state
}
```
Initializes the system:
- Sets up serial communication at 115200 baud
- Configures Bluetooth with device name "ESP32_Lock"
- Sets pin modes for outputs
- Loads credentials from EEPROM
- Sets initial state to locked with red LED on

## Main Loop
```cpp
void loop() {
  // Auto-lock check
  // Handle lockout blinking
  // Process Bluetooth commands
}
```
Continuously:
- Checks for auto-lock timeout due to inactivity
- Handles LED blinking during lockout periods
- Processes incoming Bluetooth commands

## Credential Management Functions

### loadCredentials()
Reads stored username and password from EEPROM, or sets defaults if first run.

### writeStringEEPROM() & readStringEEPROM()
Custom functions to store and retrieve strings in EEPROM since it only handles bytes natively.

## Lock Control Functions

### lock()
- Deactivates relay (secures lock)
- Turns on red LED, turns off green LED
- Plays single beep
- Updates lock state

### unlock()
- Activates relay (releases lock)
- Turns on green LED, turns off red LED
- Plays double beep
- Updates lock state and activity timestamp

## User Interface Functions

### beep()
Generates audio feedback with different patterns:
- 1 beep: Lock engaged
- 2 beeps: Lock released
- 3 beeps: Security lockout activated

### blinkRed()
Creates blinking red LED effect during security lockout periods.

## Command Processing

### handleCommand()
Processes user commands received via Bluetooth:
- **"status"**: Reports current lock state
- **"open"**: Initiates authentication process
- **"close"**: Manually locks the system
- **"newpass:xxx"**: Changes password (only when unlocked)
- **"help"**: Shows available commands

## Authentication System

### authenticate()
Comprehensive login process:
- Checks for active lockout period
- Prompts for username and password
- Handles special admin reset command
- Validates credentials against stored values
- Manages failed attempt counter and lockout

### readWithTimeout()
Safely reads user input with 30-second timeout to prevent system hanging.

## Security Features

### resetDefaults()
Emergency reset function accessible with admin credentials ("admin"/"resetlock").

### changePassword()
Allows password changes with minimum length validation (4 characters).

## Utility Functions

### repeatChar()
Helper function that creates strings of repeated characters (used for password masking with asterisks).

## Security Features Summary
- **Brute Force Protection**: 3-attempt limit with 30-second lockout
- **Auto-lock**: Automatic locking after 30 seconds of inactivity
- **Persistent Storage**: Credentials survive power cycles
- **Emergency Reset**: Admin override for recovery
- **Password Masking**: Hides password input with asterisks
- **Timeout Protection**: Prevents system hanging on user input

This code creates a robust, secure Bluetooth-controlled lock system with multiple layers of protection against unauthorized access.
