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

## Installation

1. Clone the repository or download the code.
2. Open the Arduino IDE and load the provided code.
3. Install the required libraries via the Library Manager.
4. Connect your ESP8266 to your computer.
5. Select the correct board and port in the Arduino IDE.
6. Upload the code to your ESP8266.

## Code Explanation

- **Setup**: Initializes serial communication, pins, and the OLED display. It sets the device to station mode and enables promiscuous mode for packet sniffing.
  
- **Loop**: Continuously checks for deauth packets. If detected, it updates the display, triggers the buzzer, and maintains channel hopping for effective scanning.

- **Attack Detection**: When the number of deauth packets exceeds a defined threshold, the device indicates an attack, increments the attack counter, and updates the display accordingly.

## Customization

- **Adjustable Parameters**: You can modify the `CH_TIME`, `PKT_RATE`, and `PKT_TIME` constants to customize the sensitivity and responsiveness of the detection.
- **Channel Range**: Modify the `channels[]` array to include more channels as needed, based on your region's WiFi regulations.

## Notes

- Ensure you are authorized to monitor the networks in your vicinity.
- This project is intended for educational purposes and should be used responsibly.

