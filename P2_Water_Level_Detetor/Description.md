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
The code uses **bit manipulation** as a memory-efficient way to store multiple boolean flags in a single byte. Let me break down this clever technique:

## The Problem
Instead of using separate `bool` variables for each system state (which would use 1 byte each), the code packs multiple flags into a single `uint8_t` variable.

## The Solution: Bit Fields

```cpp
// Single byte to store all system states
uint8_t systemState = 0;

// Bit position definitions
#define MOTOR_STATE 0    // Bit 0: Motor ON/OFF
#define WARNING_ZONE 1   // Bit 1: Warning level active
#define CRITICAL_ZONE 2  // Bit 2: Critical level active
```

## Visual Representation

```
systemState byte: [ 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 ]
                  [   unused    | C | W | M ]

Where:
- Bit 0 (M): Motor State (0=OFF, 1=ON)
- Bit 1 (W): Warning Zone (0=normal, 1=warning)
- Bit 2 (C): Critical Zone (0=normal, 1=critical)
- Bits 3-7: Unused (available for future features)
```

## The Three Macros Explained

### 1. **SET_BIT** - Turn a bit ON (set to 1)
```cpp
#define SET_BIT(reg, bit) (reg |= (1 << bit))
```

**How it works:**
- `(1 << bit)` creates a mask with only the target bit set
- `|=` performs bitwise OR to set that bit to 1

**Example:**
```cpp
// Turn motor ON (set bit 0)
SET_BIT(systemState, MOTOR_STATE);

// Step by step:
// 1 << 0 = 00000001 (mask)
// systemState = 00000000 (before)
// systemState |= 00000001
// systemState = 00000001 (after - motor ON)
```

### 2. **CLEAR_BIT** - Turn a bit OFF (set to 0)
```cpp
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))
```

**How it works:**
- `(1 << bit)` creates a mask with target bit set
- `~` inverts the mask (target bit becomes 0, others become 1)
- `&=` performs bitwise AND to clear only that bit

**Example:**
```cpp
// Turn motor OFF (clear bit 0)
CLEAR_BIT(systemState, MOTOR_STATE);

// Step by step:
// 1 << 0 = 00000001
// ~(1 << 0) = 11111110 (inverted mask)
// systemState = 00000001 (before)
// systemState &= 11111110
// systemState = 00000000 (after - motor OFF)
```

### 3. **READ_BIT** - Check if a bit is ON or OFF
```cpp
#define READ_BIT(reg, bit) ((reg >> bit) & 1)
```

**How it works:**
- `>> bit` shifts the target bit to position 0
- `& 1` masks out all bits except the least significant bit
- Returns 1 if bit was set, 0 if it was clear

**Example:**
```cpp
// Check if motor is ON
bool motorIsOn = READ_BIT(systemState, MOTOR_STATE);

// Step by step:
// systemState = 00000101 (motor ON, warning ON)
// systemState >> 0 = 00000101 (no shift for bit 0)
// 00000101 & 1 = 1 (motor is ON)

// Check if warning is active
bool warningActive = READ_BIT(systemState, WARNING_ZONE);
// systemState >> 1 = 00000010 (shift right 1)
// 00000010 & 1 = 0... wait, that's wrong!
// Actually: 00000010 & 00000001 = 0
// Let me recalculate: 00000101 >> 1 = 00000010
// 00000010 & 1 = 0... 

// Actually, let me fix this:
// systemState = 00000101 (bits 0 and 2 set)
// For WARNING_ZONE (bit 1): 00000101 >> 1 = 00000010
// 00000010 & 1 = 0 (bit 1 is NOT set)
// For MOTOR_STATE (bit 0): 00000101 >> 0 = 00000101  
// 00000101 & 1 = 1 (bit 0 IS set)
```

## Real Usage in the Code

```cpp
// Motor control logic
if (distance > lowlvl) {
    if (READ_BIT(systemState, MOTOR_STATE)) {      // Check if motor is currently ON
        CLEAR_BIT(systemState, MOTOR_STATE);       // Turn motor OFF
        digitalWrite(relay, LOW);
    }
}

// Alarm state management
if (distance > criticalLevel) {
    SET_BIT(systemState, CRITICAL_ZONE);           // Activate critical alarm
    CLEAR_BIT(systemState, WARNING_ZONE);         // Deactivate warning
}
```

## Benefits of This Approach

1. **Memory Efficiency**: 3 boolean states in 1 byte instead of 3 bytes
2. **Atomic Operations**: All state changes happen in single instructions
3. **Fast Execution**: Bit operations are extremely fast on microcontrollers
4. **Scalable**: Can easily add more flags (5 more bits available)
5. **Clean Code**: Macros make the operations readable and reusable

## Alternative Without Bit Manipulation

```cpp
// Less efficient approach:
bool motorState = false;      // 1 byte
bool warningZone = false;     // 1 byte  
bool criticalZone = false;    // 1 byte
// Total: 3 bytes vs 1 byte with bit manipulation
```

This bit manipulation technique is a classic embedded programming optimization that saves precious RAM while maintaining code clarity through well-designed macros.
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
