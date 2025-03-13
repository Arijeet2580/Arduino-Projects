// BLUETOOTH COMMUNICATION 7 6 HC-05
#include <SoftwareSerial.h>
#define tx 7
#define rx 6
SoftwareSerial bSerial (tx, rx);

int redVal = 0;
int blueVal = 0;
int greenVal = 0;

const char START_TOKEN = '?';
const char END_TOKEN = ';';
const int MAX_WORDS = 30;  // Maximum words allowed

#define red 11
#define blue 10
#define green 9

char data;
String messageBuffer = "";
boolean communicationStarted = false;
int wordCount = 0;

void setLed(String message);

void setup() {
  bSerial.begin(9600); // Set your desired baud rate
  Serial.begin(9600); // For Debugging
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {
  if (bSerial.available()) {
    data = bSerial.read();

    if (data == START_TOKEN) {
      communicationStarted = true;
      messageBuffer = "";  // Clear the buffer
      wordCount = 0;  // Reset word count
      bSerial.println("Communication started");
      bSerial.println("Enter the value in this format: r=xxx&b=xxx&g=xxx");
      bSerial.println("No given value should be greater than 255");
    }

    if (communicationStarted) {
      if (data != END_TOKEN) {
        messageBuffer += data;
        if (data == ' ' || data == '\n' || data == '\r') {
          wordCount++;
        }
        if (wordCount > MAX_WORDS) {
          bSerial.println("Alarm: Message exceeds 30 words");
          communicationStarted = false;
        }
      } else {
        bSerial.println("Communication ended");
        messageBuffer.trim();
        messageBuffer.remove(0, 1);  // Remove START_TOKEN
        processMessage(messageBuffer);
        communicationStarted = false;
      }
    }
  }
}

void processMessage(String message) {
  bSerial.println("Received Message: " + message);
  setLed(message);
}

void setLed(String message) {
  int startPos, endPos;

  // Parse red value
  startPos = message.indexOf("r=") + 2;
  endPos = message.indexOf("&", startPos);
  redVal = (endPos == -1) ? message.substring(startPos).toInt() : message.substring(startPos, endPos).toInt();
  analogWrite(red, constrain(redVal, 0, 255));

  // Parse blue value
  startPos = message.indexOf("b=") + 2;
  endPos = message.indexOf("&", startPos);
  blueVal = (endPos == -1) ? message.substring(startPos).toInt() : message.substring(startPos, endPos).toInt();
  analogWrite(blue, constrain(blueVal, 0, 255));

  // Parse green value
  startPos = message.indexOf("g=") + 2;
  greenVal = message.substring(startPos).toInt();
  analogWrite(green, constrain(greenVal, 0, 255));
}
