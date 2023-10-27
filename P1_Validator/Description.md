# 1️⃣ Validator

In this Project, I have used a HC - 05 Bluetooth module 
(Master| Slave) to communicate with the Arduino that checks whether the username and password given by the user is correct or not.

If the User is valid user, It outputs valid user along with the RGB LED turning to Green.

Whereas, If the User is invalid user, It outputs invalid user along with the LED turning to red.

## Tech Stack Used:
1. Bluetooth Module(HC-05/HC-06)
2. RGB Led

## Knowledge Required:
1. Bluetooth Communication

```terminal
#include <SoftwareSerial.h>
SoftwareSerial Bluetooth(10,11)//RX TX
```
#### Memory Requirement
Sketch uses 5432 bytes (16%) of program storage space. Maximum is 32256 bytes.
Global variables use 441 bytes (21%) of dynamic memory, leaving 1607 bytes for local variables. Maximum is 2048 bytes.