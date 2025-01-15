// Include Wire Library for I2C
#include <Wire.h> 
// Include NewLiquidCrystal Library for I2C
#include <LiquidCrystal_I2C.h>

// Configure LCD with I2C address (0x27 for most common LCD backpacks)
// Set the LCD number of columns and rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define LM35 sensor pin
const int lm35Pin = A0;

// Variables for temperature readings
float tempC;
float tempF;
unsigned long previousMillis = 0;
const long interval = 1000;  // Update interval (1 second)

// Custom degree symbol character
byte degreeSymbol[8] = {
  0b00110,
  0b01001,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  // Initialize I2C LCD
  lcd.init();
  // Turn on LCD backlight
  lcd.backlight();
  
  // Create custom character for degree symbol
  lcd.createChar(0, degreeSymbol);
  
  // Display initial message
  lcd.setCursor(0, 0);
  lcd.print("Digital Thermo");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Read and calculate temperature
    int sensorValue = analogRead(lm35Pin);
    tempC = (sensorValue * 5.0 / 1024.0) * 100;  // Convert to Celsius
    tempF = (tempC * 9.0 / 5.0) + 32;            // Convert to Fahrenheit
    
    // Update display
    updateDisplay();
    
    // Print to Serial for debugging
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C / ");
    Serial.print(tempF);
    Serial.println("°F");
  }
}

void updateDisplay() {
  // First row - Celsius
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempC, 1);
  lcd.write(0);  // Display custom degree symbol
  lcd.print("C   ");
  
  // Second row - Fahrenheit
  lcd.setCursor(0, 1);
  lcd.print("     ");
  lcd.print(tempF, 1);
  lcd.write(0);  // Display custom degree symbol
  lcd.print("F   ");
  
  // Add indicator for temperature range
  lcd.setCursor(15, 0);
  if (tempC > 30) {
    lcd.print("HOT TEMP");  // Hot
  } else if (tempC < 20) {
    lcd.print("LOW TEMP");  // Low
  } else {
    lcd.print("NORMAL TEMP");  // Normal
  }
}