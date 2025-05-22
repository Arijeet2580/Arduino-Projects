# 🌡️ DHT22-Based Smart Fan Controller using Arduino

## 📘 Project Description

This Arduino project is a **temperature-controlled fan automation system** using the **DHT22 digital temperature sensor**, a **relay module**, and a **buzzer**. It displays the current temperature on a **16x2 LCD**, triggers a **buzzer alarm** when the environment overheats, and automatically switches a fan **ON or OFF** using a relay based on temperature thresholds.

It is ideal for projects involving **environment monitoring**, **basic automation**, and **embedded control systems**.

---

## 🛠️ Hardware Required

| Component          | Description                                |
|--------------------|--------------------------------------------|
| Arduino UNO/Nano   | Microcontroller                            |
| DHT22 Sensor       | Digital temperature & humidity sensor      |
| 16x2 LCD           | Displays temperature readings              |
| Relay Module       | Controls fan or load                       |
| Buzzer             | Audible overheat alarm                     |
| Jumper Wires       | For wiring connections                     |
| Power Source       | USB or 5V regulated supply                 |

---

## ⚙️ Features

- 📡 **Digital Temperature Sensing** using DHT22  
- 📺 **LCD Display** of live temperature with degree symbol  
- 🔊 **Buzzer Alert** when temperature ≥ 30°C  
- 🌬️ **Fan Automation**:
  - ON at ≥ 30°C
  - OFF at ≤ 25°C  
- 🖥️ **Serial Debugging Output** for sensor readings

---

## 📦 Required Libraries

> **These must be installed before compiling the code:**

1. **DHT sensor library by Adafruit**  
2. **Adafruit Unified Sensor library**

### 📥 Installation (Arduino IDE)

1. Go to **Sketch → Include Library → Manage Libraries...**
2. Search and install:
   - `DHT sensor library` by Adafruit
   - `Adafruit Unified Sensor` by Adafruit

If you skip this step, you'll get a **"No such file or directory" error for Adafruit_Sensor.h** during compilation.

---

## 💻 Code Behavior

- Reads temperature from DHT22 every second
- If temp ≥ 30°C:
  - Activates buzzer
  - Turns ON relay (fan)
- If temp ≤ 25°C:
  - Deactivates buzzer
  - Turns OFF relay (fan)
- Displays current temperature on 16x2 LCD
- Logs readings to Serial Monitor

---

## 🔌 Pin Configuration

| Component        | Arduino Pin |
|------------------|-------------|
| DHT22 DATA       | D9          |
| Relay IN         | D7          |
| Buzzer           | D8          |
| LCD RS           | D12         |
| LCD EN           | D11         |
| LCD D4-D7        | D5, D4, D3, D2 |
| Power/GND        | 5V, GND     |

---

## 🧪 How to Test

1. Connect the circuit as per pin layout
2. Upload the code to Arduino
3. Open Serial Monitor (9600 baud) to observe logs
4. Apply heat to the DHT22 (use a warm surface or breathe over it)
5. Observe fan activation and buzzer triggering on LCD and relay

---

## 🐞 Debugging Tips

- **"Adafruit_Sensor.h: No such file or directory"**  
  → You haven’t installed the Adafruit Unified Sensor Library.

- **Sensor shows NaN or 'Sensor Error' on LCD**  
  → Check wiring (pull-up resistor on DHT22 DATA line recommended), power supply stability, and pin definition.

- **LCD not showing output**  
  → Check contrast potentiometer or verify pin mapping.

---

## 🔮 Future Enhancements

Here are some improvements you can build on:

- 💧 **Add Humidity Display** using `dht.readHumidity()`  
- 📲 **Wi-Fi/IoT Dashboard** (via ESP32) to monitor remotely  
- 📈 **Temperature Logging** to SD card or cloud  
- 🔧 **User-Configurable Thresholds** via buttons or web UI  
- 🔋 **Battery Voltage Monitoring** for portable versions  
- 🎛️ **OLED Upgrade** for better visuals and more data  
- 🧠 **PID Control** for fan speed based on temperature ramp

---

## 📜 License

MIT License — Use, modify, and share freely with attribution.

---

## 🤝 Contribute

Found a bug or want to add an IoT extension?  
Open a pull request or suggest features — contributions are welcome!

---
