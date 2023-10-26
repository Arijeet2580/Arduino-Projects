# 2️⃣ Water Level Detector

In this Detector, Ultrasonic Sensor is used to detect the water level of the Water Tank. the value from the Sensor is mapped to a distance and later into the water level in Litre to be shown in the lcd screen.

Then the value is conditioned to see whether the relay need to be high or not. That checks whether the motor is ON or not

The output then scrolled down to show the status of the motor.
### LCD Screen OUTPUT format
WATER LVL: 20L<br>
MOTOR: OFF


# Technology Used:
1. Ultrasonic Sensor HC SR04
2. Relay
3. lcd Screen 16X2 
# Knowledge Required 
```terminal
long duration = pulseIn(echoPin,HIGH);
```
Counts the duration the echoPin is HIGH or it returns the time taken the sound to reach the receiver
```terminal
long distance = (duration * 0.0344)/2;
```
344 m/s to 0.0344 cm/microseconds<br>
The distance is divided by 2 because the echopin counts the time it take to reach the object and come from there.

```terminal
byte trigPin = 7,echoPin = 5,motor   = 3;
```
UltraSonic Sensor is ranges from 3cm to 4m <br>
The Size of the water tank is 2m Let's take Water level when motor needs to be ON be 1.5m so 150cm <br>
Let's take Water level when motor needs to be OFF be 0.5m so 50 cm
