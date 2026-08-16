# Hardware Connections

## 1. Main Controller

**ESP32** is used as the main controller of the Scan-Go Smart Trolley.

It handles:
- Wi-Fi communication
- Firebase communication
- Motor control
- IR sensor input
- Ultrasonic obstacle detection
- Robotic arm control through the PCA9685 servo driver

---

## 2. Motor Driver

The trolley uses a motor driver to control two DC motors.

| Motor Driver Pin | ESP32 Pin |
|---|---:|
| ENA | GPIO 32 |
| ENB | GPIO 33 |
| IN1 | GPIO 25 |
| IN2 | GPIO 26 |
| IN3 | GPIO 27 |
| IN4 | GPIO 14 |

### Motor Control

- ENA and ENB control the motor speed using PWM.
- IN1 and IN2 control the first motor direction.
- IN3 and IN4 control the second motor direction.
- The trolley moves forward when both motors rotate in the forward direction.

---

## 3. IR Sensors

Three IR sensors are used for black-line/path following.

| IR Sensor | ESP32 Pin |
|---|---:|
| Left (L) | GPIO 34 |
| Center (C) | GPIO 35 |
| Right (R) | GPIO 39 |

The sensors are used to detect the path and identify nodes along the trolley's route.

The ESP32 reads the three sensor values and controls the motors accordingly.

---

## 4. Ultrasonic Sensor

An ultrasonic sensor is used for front obstacle detection.

| Ultrasonic Pin | ESP32 Pin |
|---|---:|
| TRIG | GPIO 18 |
| ECHO | GPIO 19 |

The system continuously measures the distance in front of the trolley.

If an obstacle is detected within the configured safety distance, the trolley stops.

**Obstacle detection threshold:** 15 cm

---

## 5. PCA9685 Servo Driver

A PCA9685 16-channel PWM servo driver is used to control the robotic arm.

The PCA9685 communicates with the ESP32 using I2C.

| PCA9685 Pin | ESP32 Pin |
|---|---:|
| SDA | GPIO 21 |
| SCL | GPIO 22 |

**I2C Address:** `0x40`

**PWM Frequency:** 50 Hz

---

## 6. Robotic Arm

The robotic arm uses four servo motors.

| Servo | PCA9685 Channel |
|---|---:|
| Base | Channel 0 |
| Shoulder | Channel 1 |
| Elbow | Channel 2 |
| Gripper | Channel 3 |

The arm performs a pick-and-place operation when the trolley reaches the required product node.

### Arm Operation

1. Move the base toward the product.
2. Open the gripper.
3. Move the shoulder and elbow toward the product.
4. Close the gripper.
5. Lift the product.
6. Rotate the base toward the trolley.
7. Open the gripper to place the product.
8. Return the arm to its initial position.

---

## 7. Power Supply

The ESP32, motor driver, sensors, and servo system require appropriate power supplies.

The motors and servos should be supplied from a suitable external power source rather than directly from the ESP32.

A common ground should be maintained between the ESP32 and connected control electronics where required.

---

## 8. Hardware Summary

| Component | Purpose |
|---|---|
| ESP32 | Main controller and Wi-Fi communication |
| Motor Driver | Controls DC motors |
| 2 DC Motors | Drives the trolley |
| 3 IR Sensors | Line/path following and node detection |
| Ultrasonic Sensor | Obstacle detection |
| PCA9685 | Servo motor control |
| 4 Servo Motors | Robotic arm movement |
| Robotic Arm | Product pick-and-place |
