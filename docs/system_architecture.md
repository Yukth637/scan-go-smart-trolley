# System Architecture

## Overview

The Scan-Go Smart Trolley consists of a mobile application, Firebase Real-Time Database, and an ESP32-based robotic trolley.

The system allows users to select products through the mobile application. Product and navigation information is synchronized through Firebase. The ESP32 retrieves the required tasks and controls the trolley movement and robotic arm.

---

## System Components

The main components of the system are:

1. Mobile Application
2. Firebase Real-Time Database
3. ESP32 Controller
4. Motor Driver
5. DC Motors
6. IR Sensors
7. Ultrasonic Sensor
8. PCA9685 Servo Driver
9. Robotic Arm
10. Product/Node Path

---

## System Flow

```text
+----------------------+
|   Mobile Application |
+----------+-----------+
           |
           | Product Selection
           | Start / Scan
           v
+----------------------+
| Firebase Realtime DB |
+----------+-----------+
           |
           | Tasks / Commands
           v
+----------------------+
|        ESP32         |
|   Main Controller    |
+----+------+------+---+
     |      |      |
     |      |      |
     v      v      v
  Motors   IR    Ultrasonic
  Driver  Sensors   Sensor
     |
     v
+----------------------+
|   Trolley Movement   |
+----------+-----------+
           |
           | Target Node
           v
+----------------------+
|   Robotic Arm        |
| PCA9685 + Servos     |
+----------+-----------+
           |
           v
     Product Pickup
