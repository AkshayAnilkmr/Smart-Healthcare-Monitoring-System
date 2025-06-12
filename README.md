# Smart Healthcare Monitoring System

An IoT-based wearable system using ESP32 to monitor vitals like heart rate, temperature, and SpO2 in real-time, with Firebase integration for remote health tracking.

## Components
- ESP32
- Pulse Sensor (analog)
- MLX90614 (non-contact temperature)
- MAX30100 (Pulse Oximeter)
- Firebase Realtime Database

## Features
- Real-time health data monitoring
- Cloud storage of vitals
- Emergency alert system

## How to Use
1. Clone this repo.
2. Open `SmartHealthcare.ino` in Arduino IDE.
3. Fill `config.h` with your WiFi and Firebase credentials.
4. Upload to ESP32.

## Output Example
Pulse: 72 | Temp: 36.7°C | BPM: 75.3 | SpO2: 97.1%
