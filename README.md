# Arduino Vision Assist

**Arduino Vision Assist** is an IoT-based assistive system designed to support visually impaired individuals.  
The project integrates **ultrasonic sensors, GPS, buzzer, vibration motor, camera, and an emergency button**, all connected through **Blynk IoT**.  
It provides **real-time obstacle detection, emergency alerts, live GPS tracking, and live video streaming** to ensure user safety and immediate caregiver response.

---

## 🚀 Features

- **Obstacle Detection**
  - Ultrasonic sensor measures distance to nearby objects.
  - Triggers **buzzer** + **motor vibration** when obstacles are detected within a set threshold.
  - Adaptive beeping: faster beeps as objects get closer.

- **Emergency Button**
  - Allows the user to send an **instant SOS alert** with GPS location.
  - Notification sent via **Blynk IoT app**.

- **Live GPS Tracking**
  - Continuously updates **latitude and longitude**.
  - Sends **live location alerts** to caregivers via Blynk.

- **Camera Integration**
  - ESP32-CAM streams **real-time video feed** online.
  - Family or caregivers can monitor the environment remotely.

- **Smart Alerts**
  - If an obstacle is detected **within 5 cm**, system sends an **urgent danger alert**.

---

## 🔮 Future Improvements

- **Gyroscope / Accelerometer**  
  Detects if the user falls down and automatically alerts caregivers.

- **AI-based Object Recognition**  
  Enhance detection beyond just distance — identify specific obstacles (cars, stairs, doors).

- **Wearable Form Factor**  
  Miniaturized design (belt, glasses, or wristband).

---

## 🛠️ Hardware Requirements

- ESP32 (with CAM support for live video)
- Ultrasonic Sensor (HC-SR04 or equivalent)
- Buzzer
- Vibration Motor (connected via TIP120 transistor)
- Push Button (emergency trigger)
- GPS Module (e.g., NEO-6M)
- Power Supply (battery pack)

---

## 📲 Software & Services

- [Arduino IDE](https://www.arduino.cc/en/software)  
- [Blynk IoT Platform](https://blynk.io/) (mobile app + cloud)  
- Libraries used:
  - `WiFi.h`
  - `WiFiClientSecure.h`
  - `BlynkSimpleEsp32.h`
  - `TinyGPS++`
  - `HardwareSerial`

---

## ⚙️ Setup Instructions

1. **Clone the repository**
   ```bash
   git clone https://github.com/MyatMinHtet30/arduino-vision-assist.git
