#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <WiFi.h>
#include "secrets.h"
#include <ctime>

struct Screen {
    public:
        uint8_t sda_pin;
        uint8_t scl_pin;
};

struct WiFiConfig {
    public:
        const char *ssid;
        const char *password;
};

constexpr Screen SCREEN = {21, 22};
constexpr WiFiConfig WIFI_CONFIG = {WIFI_SSID, WIFI_PASS};

// LED Screen config (Rotation, Reset pin)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
    Serial.begin(115200);
    delay(1000);

    Wire.begin(SCREEN.sda_pin, SCREEN.scl_pin);
    Wire.setClock(400000);

    u8g2.begin();

    delay(500);
    WiFi.begin(WIFI_CONFIG.ssid, WIFI_CONFIG.password);
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.print("Connecting to WiFi...");
        delay(1000);
    } else {
        Serial.println("Connected to WiFi");
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 10, "Connected to WiFi");
        u8g2.sendBuffer();
        delay(5000);
    }
}
