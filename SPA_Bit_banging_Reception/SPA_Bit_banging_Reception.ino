// Pin definitions
const int RX_PIN = 11;     // Pin used for reception
const int BAUD_RATE = 9600; // Reception rate in bits per second
const long BIT_DELAY = 1000000 / BAUD_RATE; // Microseconds per bit
const long HALF_BIT_DELAY = BIT_DELAY / 2;  // Half bit delay for sampling in the middle

// Buffer for received data
const int BUFFER_SIZE = 64;
char receiveBuffer[BUFFER_SIZE];
int bufferIndex = 0;

// Function prototypes
byte receiveByte();
bool receiveString();

void setup() {
  // Initialize regular Serial for debugging
  Serial.begin(9600);
  Serial.println("Arduino Bit Banging Receiver");
  
  // Set the RX pin as input with pull-up
  pinMode(RX_PIN, INPUT_PULLUP);
  
  delay(1000); // Startup delay
}

void loop() {
  // Check if a start bit is detected
  if (digitalRead(RX_PIN) == LOW) {
    // Start bit detected, attempt to receive a full string
    if (receiveString()) {
      // Print the received data
      Serial.print("Received: ");
      Serial.println(receiveBuffer);
      
      // Clear the buffer
      memset(receiveBuffer, 0, BUFFER_SIZE);
      bufferIndex = 0;
    }
  }
  
  // Small delay to prevent CPU overload
  delayMicroseconds(10);
}

/**
 * Receives a single byte using bit banging technique
 * 
 * @return The received byte
 */
byte receiveByte() {
  byte data = 0;
  
  // Wait for start bit to finish (we've already detected it)
  delayMicroseconds(BIT_DELAY);
  
  // Sample in the middle of each bit
  for (int i = 0; i < 8; i++) {
    // Wait for half a bit period to sample in the middle
    delayMicroseconds(BIT_DELAY);
    
    // Shift the received bit into our data byte
    data >>= 1;
    if (digitalRead(RX_PIN) == HIGH) {
      data |= 0x80; // Set the MSB if we read a HIGH
    }
  }
  
  // Wait for the stop bit
  delayMicroseconds(BIT_DELAY);
  
  // Check if the stop bit is HIGH as expected
  if (digitalRead(RX_PIN) != HIGH) {
    // Framing error detected
    Serial.println("Framing error!");
    return 0; // Return null character on error
  }
  
  return data;
}

/**
 * Receives a string of characters until timeout or buffer full
 * 
 * @return true if reception was successful, false otherwise
 */
bool receiveString() {
  unsigned long startTime = micros();
  const unsigned long timeout = BIT_DELAY * 20; // Timeout after 20 bit periods
  
  // Skip the start bit that was detected in the main loop
  delayMicroseconds(BIT_DELAY);
  
  while (bufferIndex < BUFFER_SIZE - 1) {
    // Check for timeout between characters
    if (micros() - startTime > timeout) {
      // End of transmission
      receiveBuffer[bufferIndex] = '\0'; // Null-terminate the string
      return true;
    }
    
    // Wait for start bit (LOW)
    if (digitalRead(RX_PIN) == LOW) {
      // Receive one byte
      char receivedChar = receiveByte();
      
      // Store the character
      receiveBuffer[bufferIndex++] = receivedChar;
      
      // Reset timeout
      startTime = micros();
    }
    
    // Small delay to prevent CPU overload
    delayMicroseconds(10);
  }
  
  // Buffer full
  receiveBuffer[bufferIndex] = '\0'; // Null-terminate the string
  return true;
}