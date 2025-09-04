# Smart Navigation Transportation Robot 🤖

![Arduino](https://img.shields.io/badge/Platform-Arduino-blue)
![ESP8266](https://img.shields.io/badge/Controller-ESP8266-orange)
![Python](https://img.shields.io/badge/VoiceControl-Python-green)
![License](https://img.shields.io/badge/License-MIT-lightgrey)

---

## Overview

The **Smart Navigation Transportation Robot** is an **autonomous robot** designed to carry heavy loads efficiently while following pre-defined routes. The robot combines **IoT, web interfaces, and voice commands** to simplify logistics tasks and reduce human effort.  

Key capabilities include:  
- Following **pre-set routes** reliably using NodeMCU web server.  
- **Obstacle avoidance** using ultrasonic sensors.  
- **Crane operation** for picking and placing items.  
- **Voice command control** using Python and Google Speech Recognition API.  

🏆 This project won **1st place** in **PDC 3.0 College Project Competition**.

---

## Features

| Feature                       | Description |
|-------------------------------|-------------|
| Autonomous Navigation          | Follow routes set via web interface |
| Obstacle Detection             | Ultrasonic sensor prevents collisions |
| Crane Operation                | Up/Down & Left/Right control via web |
| Voice Commands                 | Forward, Backward, Left, Right, Stop |
| Heavy Load Handling            | Four DC motors for locomotion, two for crane |
| Web-Based Control              | NodeMCU web server for robot and crane |

---

## Components Used

| Component                        | Quantity | Description |
|---------------------------------|----------|-------------|
| DC Gear Motor (100 RPM)          | 4        | Robot locomotion |
| DC Gear Motor (300 RPM)          | 2        | Crane mechanism |
| Motor Driver (L293D)             | 2        | Motor control |
| ESP8266 NodeMCU                   | 1        | Main controller |
| Ultrasonic Sensor                 | 1        | Obstacle detection |
| 12V Cooling Fan                   | 1        | Prevents overheating |
| 3.7V Li-ion Battery (2600mAh)    | 2-3      | Power source |

---

## Hardware Setup

1. Connect **4 DC motors** to **L293D** for robot movement.  
2. Connect **2 DC motors** to another **L293D** for crane control.  
3. Wire **ESP8266 NodeMCU** pins according to the code.  
4. Attach **Ultrasonic Sensor** at the front of the robot.  
5. Power system using **3.7V Li-ion batteries**.  
6. Install **12V fan** near NodeMCU and motor driver to prevent overheating.  

**Circuit Diagram:**  
![Circuit Diagram](Docs/circuit_diagram.png)

**Robot Images:**  
![Front View](Hardware_Images/robot_front.jpg)  
![Top View](Hardware_Images/robot_top.jpg)  
![Crane Mechanism](Hardware_Images/crane_mechanism.jpg)

---

## Software Setup

### 1️⃣ NodeMCU Motion Control
- Upload `ESP_Robot_Motion/main_motion.ino` using **Arduino IDE**.  
- Update WiFi credentials (`ssid` & `password`).  
- Features via web interface:  
  - Set destination routes  
  - Navigate to destinations  
  - Execute voice commands  

### 2️⃣ NodeMCU Crane Control
- Upload `ESP_Robot_Crane/crane_control.ino` using **Arduino IDE**.  
- Control two motors via web buttons:  
  - Motor 1: Up/Down  
  - Motor 2: Left/Right  

### 3️⃣ Voice Command Control
- Run Python script: `Voice_Command_Control/voice_control.py`  
- Install dependencies:  
```bash
pip install requests speechrecognition pyaudio

- Ensure your computer and NodeMCU are on the **same Wi-Fi network**.
- You can test the **web interface first** to see if motors respond.
- Once verified, use the **Python voice control**.
- Make sure **microphone is connected** and working.

## Usage

### Web Control:
1. Open your browser → NodeMCU IP.
2. Set route timings.
3. Click "Go to Destination".
4. Operate crane via web buttons.

### Voice Control:
1. Run `voice_control.py`.
2. Speak a command: `forward`, `backward`, `left`, `right`, `stop`.
3. Robot responds in real-time.
