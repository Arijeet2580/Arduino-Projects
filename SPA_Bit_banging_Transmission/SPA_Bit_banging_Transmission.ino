// Pin definitions
const int TX_PIN = 12;     // Pin used for transmission
const int BAUD_RATE = 9600; // Transmission rate in bits per second
const long BIT_DELAY = 1000000 / BAUD_RATE; // Microseconds per bit

// Function prototypes
void transmitByte(byte data);
void transmitString(const char* str);

void setup() {
  // Initialize regular Serial for debugging
  Serial.begin(9600);
  Serial.println("Arduino Bit Banging Transmitter");
  
  // Set the TX pin as output
  pinMode(TX_PIN, OUTPUT);
  digitalWrite(TX_PIN, HIGH); // Idle state is HIGH (for standard UART)
  
  delay(1000); // Startup delay
}

void loop() {
  // Example: transmit a message every 2 seconds
  transmitString("Hello World!");
  delay(2000);
}

/**
 * Transmits a single byte using bit banging technique
 * 
 * @param data The byte to transmit
 */
void transmitByte(byte data) {
  // Start bit (always LOW)
  digitalWrite(TX_PIN, LOW);
  delayMicroseconds(BIT_DELAY);
  
  // Data bits (LSB first for UART)
  for (int i = 0; i < 8; i++) {
    // Check if the current bit is 1 or 0
    if (data & 0x01) {
      digitalWrite(TX_PIN, HIGH);
    } else {
      digitalWrite(TX_PIN, LOW);
    }
    
    // Shift to the next bit
    data >>= 1;
    
    // Wait for one bit period
    delayMicroseconds(BIT_DELAY);
  }
  
  // Stop bit (always HIGH)
  digitalWrite(TX_PIN, HIGH);
  delayMicroseconds(BIT_DELAY);
  
  // Extra delay for stability
  delayMicroseconds(BIT_DELAY / 2);
}

/**
 * Transmits a null-terminated string using bit banging
 * 
 * @param str The string to transmit
 */
void transmitString(const char* str) {
  Serial.print("Transmitting: ");
  Serial.println(str);
  
  // Transmit each character in the string
  while (*str != '\0') {
    transmitByte(*str++);
  }
}