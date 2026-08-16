# Scan-Go Smart Trolley

## Project Overview

The Scan-Go Smart Trolley is a smart shopping system designed to automate product collection and improve the shopping experience. The system combines a mobile application, Firebase Real-Time Database, and an ESP32-based trolley.

## Features

- Product selection through the mobile application
- QR-based product identification
- Firebase-based communication between the application and trolley
- Autonomous trolley navigation using IR sensors
- Product destination node detection
- Ultrasonic obstacle detection
- Robotic arm for pick-and-place operations
- Scan confirmation before proceeding to the next product
- Completion status through Firebase

## Hardware

- ESP32
- IR sensors
- Ultrasonic sensor
- L298N motor driver
- DC motors
- PCA9685 servo driver
- Servo motors
- Robotic arm

## Software and Technologies

- Embedded C/C++
- Arduino IDE
- Firebase Realtime Database
- ESP32
- IR sensor-based line following
- Ultrasonic obstacle detection

## ESP32 Trolley Firmware

The ESP32 firmware handles Firebase communication, autonomous navigation, obstacle detection, destination node detection, and robotic-arm pick-and-place operations.

## Project Architecture

Mobile Application → Firebase Realtime Database → ESP32 Trolley

## Project Status

Academic project completed as part of the undergraduate engineering project.
