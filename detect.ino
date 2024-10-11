#include <Wire.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_SSD1306.h> // SSD1306 OLED display library
#include <ESP8266WiFi.h>

#define LED 2                /* LED pin (2=built-in LED) */
#define BUZZER 13             /* Buzzer pin */
#define LED_INVERT true      /* Invert HIGH/LOW for LED */
#define SERIAL_BAUD 115200   /* Baudrate for serial communication */
#define CH_TIME 140          /* Scan time (in ms) per channel */
#define PKT_RATE 5           /* Min. packets before it gets recognized as an attack */
#define PKT_TIME 1           /* Min. interval (CH_TIME*CH_RANGE) before it gets recognized as an attack */

// Channels to scan on (US=1-11, EU=1-13, JAP=1-14)
const short channels[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 /*,14*/ };

// Initialize the OLED display
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Use -1 for reset pin

// ===== Runtime variables ===== //
int ch_index { 0 };               // Current index of channel array
int packet_rate { 0 };            // Deauth packet counter (resets with each update)
int attack_counter { 0 };         // Attack counter
int total_attacks { 0 };          // Total number of attacks
unsigned long update_time { 0 };  // Last update time
unsigned long ch_time { 0 };      // Last channel hop time
bool attack_detected { false };   // Attack detection flag
bool sniffing_enabled { true };   // Sniffing state flag

// ===== Sniffer function ===== //
void sniffer(uint8_t *buf, uint16_t len) {
  if (!buf || len < 28) return; // Drop packets without MAC header

  byte pkt_type = buf[12]; // second half of frame control field

  // If captured packet is a deauthentication or disassociation frame
  if (pkt_type == 0xA0 || pkt_type == 0xC0) {
    ++packet_rate;
  }
}

// ===== Attack detection functions ===== //
void attack_started() {
  digitalWrite(LED, !LED_INVERT); // turn LED on
  attack_detected = true;

  total_attacks++; // Increment total attack count

  // Start buzzer sound
 for (int i = 0; i < 5; i++) {
    digitalWrite(BUZZER, HIGH); // Turn buzzer on
    delay(100);                 // Wait for 100 ms (short beep)
    digitalWrite(BUZZER, LOW);  // Turn buzzer off
    delay(100);                 // Wait for 100 ms
}

// Add a longer beep at the end
digitalWrite(BUZZER, HIGH); // Turn buzzer on
delay(300);                 // Wait for 300 ms (long beep)
digitalWrite(BUZZER, LOW);  // Turn buzzer off


  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Deauth detector");
  display.setCursor(0, 30);
  display.println("ATTACK DETECTED");
  display.setCursor(0, 40);
  display.println();
  display.setCursor(0, 50);
  display.print("Total Attacks: ");
  display.println(total_attacks); // Display total attacks
  display.display();

  Serial.println("ATTACK DETECTED");
}

void attack_stopped() {
  digitalWrite(LED, LED_INVERT); // turn LED off
  attack_detected = false;        // Reset attack detection flag

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Deauth detector");
  display.setCursor(0, 30);
  display.println("ATTACK STOPPED");
  display.setCursor(0, 40);
  display.println();
  display.setCursor(0, 50);
  display.print("Total Attacks: ");
  display.println(total_attacks); // Display total attacks
  display.display();

  Serial.println("ATTACK STOPPED");

  // Re-initialize WiFi module
  WiFi.disconnect();
  wifi_set_opmode(STATION_MODE);
  wifi_set_channel(channels[0]);
  wifi_promiscuous_enable(true);

  sniffing_enabled = true; // Enable sniffing
}

void no_attack_detected() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Deauth detector");
  display.setCursor(0, 30);
  display.println("NO ATTACKS DETECTED");
  display.setCursor(0, 40);
  display.print("Total Attacks: ");
  display.println(total_attacks); // Display total attacks
  display.display();
}

// ===== Setup ===== //
void setup() {
  Serial.begin(SERIAL_BAUD); // Start serial communication

  pinMode(LED, OUTPUT); // Enable LED pin
  pinMode(BUZZER, OUTPUT); // Enable buzzer pin
  digitalWrite(LED, LED_INVERT);

  // Initialize the display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for the display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Display boot message
  display.setCursor(0, 30);
  display.println("Starting Deauther");
  display.display();

  delay(7000); // Wait for 7 seconds

  no_attack_detected(); // Display initial message

  WiFi.disconnect();                   // Disconnect from any saved or active WiFi connections
  wifi_set_opmode(STATION_MODE);       // Set device to client/station mode
  wifi_set_promiscuous_rx_cb(sniffer); // Set sniffer function
  wifi_set_channel(channels[0]);        // Set channel
  wifi_promiscuous_enable(true);       // Enable sniffer

  Serial.println("Started \\o/");
}

// ===== Loop ===== //
void loop() {
  unsigned long current_time = millis(); // Get current time (in ms)

  // Re-enable promiscuous mode in each iteration if sniffing is enabled
  if (sniffing_enabled) {
    wifi_promiscuous_enable(true);
  }

  // Update each second (or scan-time-per-channel * channel-range)
  if (current_time - update_time >= (sizeof(channels) * CH_TIME)) {
    update_time = current_time; // Update time variable

    // When detected deauth packets exceed the minimum allowed number
    if (packet_rate >= PKT_RATE) {
      ++attack_counter; // Increment attack counter
    } else {
      if (attack_counter >= PKT_TIME) {
        attack_stopped();
      }
      attack_counter = 0; // Reset attack counter
    }

    // When attack exceeds minimum allowed time
    if (attack_counter == PKT_TIME && !attack_detected) {
      attack_started();
    }

    Serial.print("Packets/s: ");
    Serial.println(packet_rate);

    packet_rate = 0; // Reset packet rate
  }

  // Channel hopping
  if (sizeof(channels) > 1 && current_time - ch_time >= CH_TIME) {
    ch_time = current_time; // Update time variable

    // Get next channel
    ch_index = (ch_index + 1) % (sizeof(channels) / sizeof(channels[0]));
    short ch = channels[ch_index];

    // Set channel
    wifi_set_channel(ch);
  }
}
