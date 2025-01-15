# Digital Thermometer with I2C LCD Display

A precise and reliable digital thermometer project built using Arduino, LM35 temperature sensor, and I2C LCD display. This project provides real-time temperature monitoring with both Celsius and Fahrenheit readings.

## Features

- Real-time temperature monitoring
- Dual temperature display (°C and °F)
- Custom degree symbol for better readability
- Temperature range indicators (Hot/Normal/Low)
- 1-second refresh rate
- Serial monitoring support for debugging
- Power-efficient I2C communication
- Clear 16x2 LCD display with backlight

## Hardware Requirements

### Components
- Arduino board (Uno/Nano/Mega)
- LM35 temperature sensor
- 16x2 LCD display with I2C adapter (PCF8574)
- Jumper wires
- Breadboard (optional)
- 5V power supply

### Technical Specifications
- **LM35 Sensor**:
  - Temperature Range: 0°C to 100°C
  - Accuracy: ±0.5°C at 25°C
  - Linear + 10 mV/°C scale factor
  - Low self-heating capability
  
- **I2C LCD Display**:
  - 16x2 characters
  - Blue backlight
  - Operating Voltage: 5V
  - I2C Address: 0x27 (default)

## Pin Connections

### LM35 Temperature Sensor
```
LM35 Pin | Arduino Pin
---------|------------
VCC      | 5V
GND      | GND
OUT      | A0
```

### I2C LCD Display
```
LCD Pin | Arduino Pin
--------|------------
VCC     | 5V
GND     | GND
SDA     | A4 (SDA)
SCL     | A5 (SCL)
```

## Software Requirements

### Required Libraries
1. Wire.h (Built-in Arduino library)
2. LiquidCrystal_I2C.h

### Installation Steps
1. Open Arduino IDE
2. Go to Sketch > Include Library > Manage Libraries
3. Search for "LiquidCrystal I2C"
4. Install the library by Frank de Brabander

## Setup and Configuration

1. **Hardware Assembly**
   - Connect all components according to the pin connection diagram
   - Ensure proper power supply connections
   - Double-check I2C connections

2. **Software Setup**
   - Install required libraries
   - Upload the code to Arduino
   - Check Serial Monitor for debugging (9600 baud rate)

3. **I2C Address Configuration**
   - Default address is 0x27
   - If display doesn't work, try address 0x3F
   - Use I2C scanner sketch to find correct address

## Usage

1. Power up the system
2. LCD will show initialization message
3. After initialization, display shows:
   - Top row: Temperature in Celsius
   - Bottom row: Temperature in Fahrenheit
   - Right corner: Temperature indicator (H/N/L)

## Temperature Indicators

- **H (Hot)**: Temperature > 30°C
- **N (Normal)**: Temperature between 20°C and 30°C
- **L (Low)**: Temperature < 20°C

## Troubleshooting

1. **Display Not Working**
   - Check I2C address
   - Verify connections
   - Ensure proper library installation

2. **Incorrect Readings**
   - Calibrate LM35 sensor
   - Check power supply stability
   - Verify analog pin connection

3. **Flickering Display**
   - Check power supply
   - Verify I2C connections
   - Reduce refresh rate if needed

## Future Improvements

1. Add data logging capability
2. Implement temperature alerts
3. Add minimum/maximum temperature recording
4. Include humidity sensing
5. Implement temperature trend analysis
6. Add wireless connectivity

## Contributing

1. Fork the repository
2. Create feature branch
3. Commit changes
4. Push to branch
5. Create Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgments

- Arduino community for libraries and support
- Temperature sensing principles by National Instruments
- I2C communication protocol documentation

## Contact

- Your Name
- Email: your.email@example.com
- GitHub: [Your GitHub Profile](https://github.com/yourusername)