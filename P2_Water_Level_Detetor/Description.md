# ESP32 Water Level Controller System

## 🌊 Project Overview
This project implements an advanced water level monitoring and control system using an ESP32 microcontroller. The system monitors water levels using an ultrasonic sensor, automatically controls a water pump based on configurable thresholds, and provides multiple communication interfaces for monitoring and control.

## 💧 Features

### Core Functionality
- **Automatic Water Level Control**: Automatically operates a water pump to maintain water levels between configurable thresholds
- **Real-time Water Level Monitoring**: Uses ultrasonic sensor technology for accurate, non-contact level measurement
- **LCD Display**: Provides visual feedback showing current water level, percentage full, and system status
- **Configurable Thresholds**: User-adjustable thresholds for pump operation, warnings, and critical alerts
- **Status Indicators**: RGB LED and buzzer for visual and audible status alerts

### Advanced Features
- **Bluetooth Interface**: Configure and monitor the system remotely via Bluetooth using simple text commands
- **WiFi Connectivity**: Optional WiFi connectivity for remote monitoring and integration with home automation systems
- **Motor Safety Protection**: Prevents motor damage with runtime limitations and cooldown periods
- **Persistent Settings**: Stores configuration in non-volatile memory, surviving power outages
- **System Statistics**: Tracks and reports motor runtime, cycles, average water levels, and more
- **Manual Override**: Switch between automatic and manual operation modes

## 📌 Hardware Requirements
- ESP32 Development Board
- Ultrasonic Sensor (HC-SR04)
- 16x2 LCD Display
- Relay Module (for pump control)
- Piezo Buzzer
- RGB LED
- Water Pump/Motor
- Power Supply
- Breadboard & Jumper Wires

## 📊 Pin Configuration

| Component        | ESP32 Pin |
|------------------|-----------|
| LCD RS           | GPIO 12   |
| LCD EN           | GPIO 14   |
| LCD D4           | GPIO 27   |
| LCD D5           | GPIO 26   |
| LCD D6           | GPIO 25   |
| LCD D7           | GPIO 33   |
| Ultrasonic Trig  | GPIO 5    |
| Ultrasonic Echo  | GPIO 18   |
| Relay            | GPIO 19   |
| Buzzer           | GPIO 23   |
| Red LED          | GPIO 21   |
| Green LED        | GPIO 22   |
| Blue LED         | GPIO 13   |

## 💻 Setup Instructions

### Hardware Setup
1. Connect the components according to the pin configuration table
2. Securely mount the ultrasonic sensor at the top of the water tank
3. Ensure the relay is properly connected to control your water pump
4. Connect your ESP32 to a stable power supply

### Software Installation
1. Install the Arduino IDE (2.0+ recommended)
2. Install ESP32 board support in Arduino IDE:
   - Go to File > Preferences
   - Add `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json` to Additional Board Manager URLs
   - Go to Tools > Board > Boards Manager and install "ESP32 by Espressif Systems"
3. Install required libraries:
   - LiquidCrystal
   - Preferences (included with ESP32 board package)
4. Configure your WiFi credentials in the code (optional)
5. Upload the sketch to your ESP32

## 🔤 Bluetooth Commands

Connect to "ESP32_WaterController" via Bluetooth using a serial terminal app and send these commands:

| Command        | Description                                          |
|----------------|------------------------------------------------------|
| `status`       | Show current system status                           |
| `motoron`      | Turn motor ON manually                               |
| `motoroff`     | Turn motor OFF manually                              |
| `auto`         | Return to automatic control mode                     |
| `set:param:value` | Set parameters (see below)                        |
| `reset`        | Reset all settings to default values                 |
| `stats`        | Show system statistics                               |
| `help`         | Display available commands                           |

### Configurable Parameters (with `set:` command):
- `high`: High level threshold (motor OFF point)
- `low`: Low level threshold (motor ON point)
- `warning`: Warning level threshold
- `critical`: Critical level threshold
- `height`: Tank height in cm
- `capacity`: Tank capacity in liters

**Example:** `set:low:60` sets the low water threshold to 60cm

## 📐 System Configuration

### Default Settings
- Tank Height: 200 cm
- Tank Capacity: 20 liters
- Low Level Threshold (motor OFF): 150 cm
- High Level Threshold (motor ON): 50 cm
- Warning Level: 160 cm
- Critical Level: 180 cm

The system measures distance from the sensor to the water surface. A **smaller** distance value indicates a **higher** water level.

## 🛡️ Safety Features
- **Motor Runtime Protection**: Automatically shuts off the pump after 5 minutes of continuous operation
- **Motor Cooldown Period**: Enforces a 1-minute cooldown period after maximum runtime
- **Threshold Constraints**: Prevents invalid configurations
- **Alert System**: Visual and audible notifications for low water conditions
- **Watchdog Timer**: ESP32's built-in watchdog timer prevents system hangs

## 🔍 Monitoring System Status

### LCD Display
The LCD shows:
- Current water level in liters
- Tank percentage full
- System status (Normal, Warning, Critical)
- Motor status (ON/OFF) or Manual mode indication

### RGB LED Indicators
- **Green**: System normal
- **Yellow**: Warning level reached
- **Red**: Critical water level
- **Blue**: Motor is running

### Buzzer Alerts
- **No Sound**: Normal operation
- **Intermittent Beep**: Warning level reached
- **Continuous Beep**: Critical water level

## 📱 Mobile Monitoring
The system sends regular updates via Bluetooth that can be received and processed by a compatible mobile app. The data format is:
```
UPDATE|distance|liters|percent|motorStatus|systemStatus
```

Example: `UPDATE|45|16|80|OFF|NORMAL`

## 🔌 Power Requirements
- Operating Voltage: 5V DC
- Current Draw: ~200mA (excluding pump)
- Recommended power supply: 5V 2A power adapter or USB power bank for the ESP32
- The water pump should have a separate power supply appropriate for its specifications

## 📈 Future Enhancements
- Web interface for remote management
- Cloud connectivity for remote monitoring
- Historical data logging and trend analysis
- Integration with Home Assistant, Google Home, or Amazon Alexa
- Multiple sensor support for large or complex tanks
- Mobile app development
- Flow meter integration to track water consumption

## 🛠️ Troubleshooting

| Problem | Possible Solution |
|---------|------------------|
| Erratic sensor readings | Check sensor positioning, ensure it's perpendicular to water surface |
| Motor not activating | Check relay connections and thresholds configuration |
| LCD not displaying | Check I2C connections and address setting |
| System restarting | Ensure power supply is adequate (2A+ recommended) |
| WiFi not connecting | Check credentials and signal strength |

## 📄 License
This project is open source and available under the MIT License.

## 👨‍💻 Credits
Created by [Your Name]

## 📚 Further Resources
- [ESP32 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [Ultrasonic Sensor Guide](https://lastminuteengineers.com/arduino-sr04-ultrasonic-sensor-tutorial/)
- [Project Repository](https://github.com/yourusername/esp32-water-controller)
