#include <SoftwareSerial.h>

const byte rxPin = 9;
const byte txPin = 10;
SoftwareSerial Bluetooth(rxPin, txPin);

String msgBuffer = "";
String msg = "";
char data = '';

void setup() {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Bluetooth.begin(9600);
  Serial.begin(9600);
}

void loop() {
  if (Bluetooth.available() > 0) {
    char Data = (char)Bluetooth.read(); // Read the incoming data

    if (Data == ';') { // End of message delimiter
      msg = msgBuffer;
      msgBuffer = ""; // Clear buffer for new data
      Serial.println("Received: " + msg);

      // Send response back to Bluetooth
      Bluetooth.print("Your chance: ");
      Bluetooth.print(msg);
      int num = random(1, 7);
      Bluetooth.println(" " + String(num));
    } else {
      msgBuffer += Data; // Append data to buffer
    }
  }
}
