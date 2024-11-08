# WiFi Deauthentication Detector

This project utilizes an ESP8266 microcontroller to create a WiFi deauthentication packet detector. It scans specific WiFi channels for deauth packets, indicating potential attacks on nearby networks. When an attack is detected, it activates an LED and a buzzer while displaying relevant information on an OLED screen.

## Features

- **Channel Scanning**: Monitors specified WiFi channels for deauthentication and disassociation frames.
- **Attack Detection**: Activates when a minimum number of deauth packets are detected within a specified time frame.
- **OLED Display**: Provides visual feedback of the device status and the number of detected attacks.
- **Audible Alerts**: Uses a buzzer to indicate an attack has been detected.

## Hardware Requirements

- **ESP8266**: Microcontroller with WiFi capability.
- **OLED Display**: 128x64 pixel display compatible with the Adafruit SSD1306 library.
- **Buzzer**: For audio alerts.
- **LED**: Indicator for attack detection.
- **Connecting Wires**: For wiring components.

## Wiring

| Component       | ESP8266 Pin |
|------------------|-------------|
| LED              | 2           |
| Buzzer           | 13          |
| OLED VCC         | 3.3V        |
| OLED GND         | GND         |
| OLED SDA         | D2          |
| OLED SCL         | D1          |

## Software Requirements

- **Arduino IDE**: To upload the code to the ESP8266.
- **Adafruit GFX Library**: Core graphics library for the OLED display.
- **Adafruit SSD1306 Library**: Library for interfacing with the SSD1306 OLED display.
- **ESP8266WiFi Library**: For WiFi functionalities.

## Thanks

Special thanks to ❤ **[Spacehuhn](https://github.com/SpacehuhnTech)** for making this project possible!


