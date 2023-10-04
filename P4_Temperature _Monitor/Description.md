## 4️⃣  Temperature Monitor

### 📝 Description:
Takes the temperature from the sensor(LM35) and displays it on the LCD screen.
The value of the Temperature is in  Celsius.

# Points to keep in mind :

1. While using the LM35 sensor, the voltage should be between 4V to 20V( Generally 5V Source is Preferred).
2. The LM35 sensor has 3 pins, VCC, GND, and OUT.
3. OUT pin is connected to the Analog pin of the Arduino.(Here A0)
4. In the LCD, if you do not want to use the Potentiometer, You can connect 10 k ohm resistor between the V0 pin and GND pin.
5. LED might burn down if you do not give the accurate amount of resistance on the LED anode pin connected to the 5V supply.


# Formula used to convert the voltage to temperature:
```terminal
    temperature = (adc-104)/2;
```
