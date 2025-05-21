# ESP32 Bluetooth Lock System

## 📱 Project Overview
This project implements a secure electronic lock system using an ESP32 microcontroller with Bluetooth connectivity. Users can unlock the device remotely via Bluetooth using a username and password authentication system. The system features automatic locking, security timeouts, and visual/audio feedback.

## 🔒 Features
- **Bluetooth Authentication**: Securely unlock your device using username and password
- **Auto-Lock**: System automatically locks after 30 seconds of inactivity
- **Security Lockout**: After 3 failed attempts, system locks for 30 seconds
- **Visual Indicators**: RGB LED provides status feedback (Green = Unlocked, Red = Locked)
- **Audio Feedback**: Buzzer provides audio confirmation of actions
- **Password Management**: Change password via Bluetooth commands
- **EEPROM Storage**: Credentials persist across power cycles
- **Factory Reset**: Admin reset command available for recovery

## 🛠️ Hardware Requirements
- ESP32 Development Board
- Relay Module
- RGB LED
- Buzzer
- Power Supply
- Electronic Lock Mechanism (solenoid/magnetic lock)
- Breadboard & Jumper Wires

## 📌 Pin Configuration
| Component | ESP32 Pin |
|-----------|-----------|
| Relay     | GPIO 2    |
| Red LED   | GPIO 9    |
| Green LED | GPIO 10   |
| Buzzer    | GPIO 4    |

## 💻 Tech Stack Used
1. ESP32 with Built-in Bluetooth
2. RGB LED for visual feedback
3. Relay for lock control
4. EEPROM for persistent storage
5. Buzzer for audio feedback

## 📚 Knowledge Required
1. Bluetooth Communication using ESP32 BluetoothSerial
2. Arduino programming
3. Basic understanding of electronic components
4. Simple security concepts

## 📊 Memory Requirements
- **Original HC-05 Version**:
  - Sketch: 5432 bytes (16%) of program storage
  - Global variables: 441 bytes (21%) of dynamic memory

- **ESP32 Version**:
  - Sketch: ~80KB (~25%) of program storage
  - Global variables: ~10KB (~3%) of dynamic memory

## 🔌 Installation & Setup
1. Connect the hardware components according to the pin configuration
2. Install the Arduino IDE and ESP32 board support
3. Install required libraries: BluetoothSerial
4. Upload the sketch to your ESP32

```arduino
// Required libraries
#include <EEPROM.h>
#include "BluetoothSerial.h"
```

## 📲 Usage Instructions
1. Power on the system
2. Connect to "ESP32_Lock" via Bluetooth from your smartphone
3. Use one of the following commands:
   - `status`: Check if the lock is open or closed
   - `open`: Begin authentication process to unlock
   - `close`: Lock the system
   - `newpass:<password>`: Change the current password (must be unlocked)
   - `help`: Display available commands

### Default Credentials
- Username: `mangoboy`
- Password: `mango`

### Authentication Process
1. Send `open` command
2. Enter username when prompted
3. Enter password when prompted
4. System will respond with "Access Granted" or "Invalid"

### System Reset
In case of forgotten credentials, use the admin reset:
- Username: `admin`
- Password: `resetlock`

## ⚠️ Security Considerations
- The default username and password should be changed after installation
- Bluetooth communication has a limited range (~10m)
- The system includes brute-force protection with a timeout after 3 failed attempts
- Consider additional physical security measures for critical installations

## 🔄 State Diagram
```
┌───────────┐   Authentication   ┌───────────┐
│           │  ────────────────> │           │
│  LOCKED   │                    │ UNLOCKED  │
│  (RED)    │ <───────────────── │  (GREEN)  │
└───────────┘    Auto-timeout    └───────────┘
      ^                                |
      |                                |
      └────────────────────────────────┘
            Manual lock command
```

## 🛠️ Troubleshooting
- **LED not working**: Check pin connections and polarity
- **Bluetooth not connecting**: Verify ESP32 is powered and Bluetooth is enabled
- **Authentication failing**: Try factory reset with admin credentials
- **System locks out**: Wait 30 seconds after 3 failed attempts

## 🔄 Future Enhancements
- Web interface for remote management
- Multiple user accounts
- Access logs
- Time-based access control
- Integration with home automation systems
- Mobile app development
