# Scan-Go Smart Trolley

An ESP32-based smart shopping trolley designed to automate product collection using autonomous path navigation, QR-code-based product identification, Firebase communication, obstacle detection, and a robotic arm.

## Project Overview

The Scan-Go Smart Trolley is designed to improve the shopping experience by reducing the need for manually searching for products inside a store.

The system combines:

- A mobile application
- Firebase Realtime Database
- ESP32-based trolley control
- IR-based path following
- Ultrasonic obstacle detection
- Robotic arm for product handling
- QR-code-based product identification

The mobile application communicates with Firebase, while the ESP32 retrieves the required tasks and controls the trolley hardware.

---

## Objectives

- Automate product collection inside a store.
- Navigate the trolley to predefined product locations.
- Identify products using QR codes.
- Synchronize shopping tasks using Firebase.
- Detect obstacles in front of the trolley.
- Use a robotic arm for product pick-and-place operations.
- Provide a connected mobile application for the shopping process.

---

## System Architecture

```text
+----------------------+
|   Mobile Application |
+----------+-----------+
           |
           | Product / Task Data
           v
+----------------------+
| Firebase Realtime DB |
+----------+-----------+
           |
           | Commands / Tasks
           v
+----------------------+
|        ESP32         |
|   Main Controller    |
+----+------+------+---+
     |      |      |
     |      |      |
     v      v      v
  Motor    IR    Ultrasonic
  Driver Sensors  Sensor
     |
     v
+----------------------+
|   Trolley Movement   |
+----------+-----------+
           |
           | Target Node
           v
+----------------------+
|    PCA9685 Driver    |
+----------+-----------+
           |
           v
+----------------------+
|    Robotic Arm       |
|    4 Servo Motors    |
+----------------------+
