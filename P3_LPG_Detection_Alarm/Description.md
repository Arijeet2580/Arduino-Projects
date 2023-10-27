# 3️⃣ LPG Detection Alarm System

A MQ-05 Gas Sensor is Constantly Detecting the Sensor Value and displaying the Concentration of the LPG in the Environment by the LCD Screen For Nearby person and via Bluetooth For Distant Person.
Green Light will Glow.


Whenever LPG gas is detected in the Nearby Environment, a Distress Signal will be Sent via Bluetooth and LCD will display LPG Present and Red LED will Glow.

But With the distress Signal, The Servo Motor changes from 0th Position to 90deg or Perpendicular Pulling the Window hence, The Harmful Flammable LPG gas gets a free path so that the situation danger can be Decreased.

## Components Required
1. MQ2 Gas Sensor
2. Buzzer
3. Servo Motor
4. LCD Screen 
5. Bluetooth Module(Master | Slave) HC-05

# Tech Stack:
1. Servo Library
```Terminal
#include <Servo.h>
Servo Servo1;            
```
Servo1 is the Instance of the Class Servo using an Constructor

2. LiquidCrystal Library

LCD Screen display for the Project
```terminal
#include <LiquidCrystal.h>
byte rs=8,en=9,d4=10,d5=11,d6=12,d7=13;
LiquidCrystal lcd (rs,en,d4,d5,d6,d7);
```

3. SoftwareSerial Library
```terminal
#include <SoftwareSerial.h>
byte TX=10,RX=11;
SoftwareSerial Bluetooth (RX, TX);
```
For Distress Signal Transfer to the Bluetooth Device where,

The Alarm System is the Master and the receiving Device is the Slave that must be Bluetooth Enabled.

#### Memory Requirement:
Sketch uses 4696 bytes (14%) of program storage space. Maximum is 32256 bytes.

Global variables use 246 bytes (12%) of dynamic memory, leaving 1802 bytes for local variables. Maximum is 2048 bytes.