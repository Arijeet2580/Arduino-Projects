# Automated Water Tank Controller System

## Overview
This project implements an automated water level monitoring and control system using an Arduino microcontroller. The system monitors water levels in a tank using an ultrasonic sensor, controls a water pump via a relay, provides visual feedback through an LCD display, and offers audible alerts through a buzzer when water levels are critically low.

## Features
- **Real-time water level monitoring** using ultrasonic sensor technology
- **Automated pump control** based on configurable water level thresholds
- **Visual feedback** via 16x2 LCD display showing:
  - Current water level in liters
  - Tank capacity percentage
  - System status and motor state
- **Audible alerts** with two distinct alarm patterns:
  - Warning level (intermittent beeping)
  - Critical level (continuous alarm)
- **Memory-optimized code** for efficient operation on resource-constrained Arduino platforms
- **Serial monitoring** for debugging and data logging

## Hardware Requirements
- Arduino board (Uno, Nano, or similar)
- 16x2 LCD display
- HC-SR04 ultrasonic distance sensor
- Relay module (for pump control)
- Piezo buzzer
- Water pump (compatible with your relay's specifications)
- Jumper wires and breadboard
- Power supply appropriate for your pump

## Pin Configuration
| Component | Arduino Pin |
|-----------|-------------|
| LCD RS    | 8           |
| LCD EN    | 9           |
| LCD D4    | 10          |
| LCD D5    | 11          |
| LCD D6    | 12          |
| LCD D7    | 13          |
| Ultrasonic Trigger | 7  |
| Ultrasonic Echo    | 5  |
| Relay     | 3           |
| Buzzer    | 4           |

## System Parameters
These parameters can be adjusted in the code according to your specific tank and requirements:

| Parameter | Default Value | Description |
|-----------|---------------|-------------|
| `tankHeight` | 200 cm | Maximum height of the tank |
| `tankCapacity` | 20 L | Maximum capacity of the tank in liters |
| `lowlvl` | 150 cm | Distance threshold to turn the motor OFF |
| `highlvl` | 50 cm | Distance threshold to turn the motor ON |
| `criticalLevel` | 180 cm | Distance threshold for critical water level alert |
| `warningLevel` | 160 cm | Distance threshold for warning water level alert |

## How It Works

### Water Level Measurement
The system uses an HC-SR04 ultrasonic sensor to measure the distance from the top of the tank to the water surface. This distance is then converted to water volume in liters and percentage of tank capacity.

### Motor Control Logic
The water pump is controlled using these rules:
- When the water level falls below the high threshold (`highlvl`), the pump turns ON
- When the water level rises above the low threshold (`lowlvl`), the pump turns OFF
- This hysteresis prevents rapid cycling of the pump

### Alert System
The system provides two levels of alerts:
1. **Warning Level**: When water distance exceeds `warningLevel`, the buzzer beeps intermittently
2. **Critical Level**: When water distance exceeds `criticalLevel`, the buzzer sounds continuously

### Display Information
The LCD display shows:
- Top row: Current water level in liters and percentage full
- Bottom row: Status messages (CRITICAL LOW!, WARNING LOW!, or Motor status)

## Memory Optimization Features
This code is specifically optimized for Arduino's limited memory:

1. **Appropriate Variable Types**:
   - `uint8_t` for small values (0-255)
   - `uint16_t` for mid-range values
   - `uint32_t` only for millisecond timers

2. **Bit Field for State Flags**:
   - Uses bit manipulation to store multiple boolean states in a single byte
   - Reduces RAM usage compared to separate boolean variables

3. **PROGMEM String Optimization**:
   - All string literals are stored in flash memory using the `F()` macro
   - Preserves precious SRAM for runtime variables

4. **Other Optimizations**:
   - Compact serial output
   - Conditional serial output only when connected
   - Simplified logic paths

## Installation and Setup

1. **Hardware Assembly**:
   - Connect all components according to the pin configuration table
   - Mount the ultrasonic sensor at the top of your water tank
   - Ensure the relay is properly connected to your water pump

2. **Software Installation**:
   - Install the Arduino IDE if you haven't already
   - Install the LiquidCrystal library (included in Arduino IDE)
   - Upload the provided code to your Arduino

3. **Calibration**:
   - Measure the exact height of your tank and update the `tankHeight` variable
   - Calculate the actual capacity of your tank and update the `tankCapacity` variable
   - Adjust the threshold values (`lowlvl`, `highlvl`, `warningLevel`, `criticalLevel`) based on your specific requirements

## Usage
Once installed and powered, the system works autonomously:

1. The LCD display will show "Water Controller" and "Initializing..." upon startup
2. After initialization, the display will show the current water level and system status
3. The pump will turn on and off automatically based on the water level
4. The buzzer will alert you if the water level becomes too low

## Troubleshooting

| Problem | Possible Solution |
|---------|-------------------|
| Inaccurate readings | Check sensor positioning and ensure it's pointed directly at the water surface |
| Pump not turning on/off | Verify relay connections and check threshold values in code |
| No display on LCD | Check LCD wiring and contrast adjustment |
| Continuous buzzer alarm | Check water level and sensor readings |

## Extending the Project
This code can be extended with additional features such as:
- WiFi connectivity for remote monitoring
- Data logging to SD card
- Multiple tank support
- Temperature sensing of water
- Web interface for configuration

## License
This project is released under the MIT License.

## Credits
Developed as an open-source project for water conservation and management.
