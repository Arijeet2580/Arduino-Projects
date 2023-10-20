# 1️⃣ Validator

In this Project, I have used a HC - 05 Bluetooth module to communicate with the Arduino that checks whether the username and password given by the user is correct or not.

If the User is valid user, It outputs valid user along with the RGB LED turning to Green.

Whereas, If the User is invalid user, It outputs invalid user along with the LED turning to red.

![Circuit Diagram in Proteus](https://github.com/Arijeet2580/Arduino-Projects/assets/115724080/1a9afc9c-b315-4a11-9621-8e28226c3ba8)
<br>
<br>
https://github.com/Arijeet2580/Arduino-Projects/assets/115724080/c4cbb5c2-00f0-4ba5-a622-883f81552579


## Tech Stack Used:
1. Bluetooth Module(HC-05/HC-06)
2. RGB Led

## Knowledge Required:
1. Bluetooth Communication

```terminal
#include <SoftwareSerial.h>
SoftwareSerial Bluetooth(10,11)//RX TX
```
