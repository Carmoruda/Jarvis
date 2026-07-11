#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <WiFi.h>
#include "secrets.h"

struct Screen {
    public:
        uint8_t sda_pin;
        uint8_t scl_pin;
};

struct Button {
    public:
        uint8_t pin;
        bool state;
};

struct WiFiConfig {
    public:
        const char *ssid;
        const char *password;
        char *ntpServer;
};

constexpr Screen SCREEN = {21, 22};
constexpr WiFiConfig WIFI_CONFIG = {WIFI_SSID, WIFI_PASS, "pool.ntp.org"};

const char *time_zone = "CET-1CEST,M3.5.0,M10.5.0/3"; // Central European Time (CET) with daylight saving time

// LED Screen config (Rotation, Reset pin)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void connectWiFi()
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr((128 - u8g2.getStrWidth("Connecting to WiFi...")) / 2, 30, "Connecting to WiFi...");
    u8g2.sendBuffer();

    WiFi.begin(WIFI_CONFIG.ssid, WIFI_CONFIG.password);

    uint8_t retry = 0;

    while (WiFi.status() != WL_CONNECTED && retry < 100) {
        delay(300);
        retry++;
    }

    if (WiFi.status() == WL_CONNECTED) {

        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr((128 - u8g2.getStrWidth("WiFi connected!")) / 2, 30, "WiFi connected!");
        u8g2.sendBuffer();

        delay(500);

    } else {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr((128 - u8g2.getStrWidth("WiFi FAILED!")) / 2, 30, "WiFi FAILED!");
        u8g2.sendBuffer();

        delay(1500);
    }
}

void syncTime()
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr((128 - u8g2.getStrWidth("Syncing time...")) / 2, 30, "Syncing time...");
    u8g2.sendBuffer();

    configTzTime(time_zone, WIFI_CONFIG.ntpServer);

    delay(1000);
}

void setup() {
    Serial.begin(115200);

    Wire.begin(SCREEN.sda_pin, SCREEN.scl_pin);
    Wire.setClock(400000);

    u8g2.begin();

    connectWiFi();
    syncTime();

    delay(100);
}

void loop() {
    time_t now = time(nullptr);
    struct tm t;
    getLocalTime(&t);

    u8g2.clearBuffer();

    u8g2.setFont(u8g2_font_6x10_tr);

    char timeString[sizeof("HH:MM:SS")];
    // snprintf -> destination buffer, size of the buffer, format string, values to format
    snprintf(timeString, sizeof(timeString),
        "%02d:%02d:%02d",
        t.tm_hour,
        t.tm_min,
        t.tm_sec
    );
    u8g2.drawStr(0, 10, timeString);

    u8g2.setFont(u8g2_font_ncenB14_tr);
    const char *txt = "Hola!";
    u8g2.drawStr((128 - u8g2.getStrWidth(txt)) / 2, 35, txt);

    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(0, 60, "jarvis v0.1");

    u8g2.sendBuffer();
}
