# RGB Color Mixer

This uses a Serial Communication to take a message Response model from the user and takes the combination of the color in message and parses into a RGB value and displays the color on the RGB LED.

This project can also used by using Bluetooth Communication.

We just need to change all the Serial object with the Bluetooth Object.
And add
```terminal
    #include <SoftwareSerial.h>
    SoftwareSerial.h bluetooth(10,11);
    bluetooth.begin(9600);
```
Switch every Serial object with bluetooth object.

# Schematic View

[Download PDF](https://github.com/Arijeet2580/Arduino-Projects/blob/main/P7_Color_Mixer/Schematic_View.pdf)
