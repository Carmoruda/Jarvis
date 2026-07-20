#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "secrets.h"
#include "strings.h"
#include "screens/clock.h"
#include "screens/wifi_settings.h"
#include "hardware/display.h"
#include "hardware/buttons.h"

struct Screen {
    public:
        uint8_t width;
        uint8_t height;
        uint8_t sda_pin;
        uint8_t scl_pin;
};

enum ScreenStates {
    CLOCK,
    EYES,
    WIFI,
    NUM_SCREENS
};

constexpr Screen SCREEN = {128, 64, 21, 22};

// Screen
int screen = ScreenStates::CLOCK;
int lastScreen = -1;

void connectWiFi()
{
    // Display a message while connecting to WiFi
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont11_tr);
    u8g2.drawStr((128 - u8g2.getStrWidth(txt::WIFI_CONNECTING)) / 2, 30, txt::WIFI_CONNECTING);
    u8g2.sendBuffer();

    // Connect to WiFi
    WiFi.begin(WIFI_CONFIG.ssid, WIFI_CONFIG.password);

    uint8_t retry = 0;

    // Wait for connection with a timeout of 30 seconds (100 * 300ms)
    while (WiFi.status() != WL_CONNECTED && retry < 100) {
        delay(300);
        retry++;
    }

    // Display connection result
    if (WiFi.status() == WL_CONNECTED) {

        u8g2.clearBuffer();
        u8g2.drawStr((128 - u8g2.getStrWidth(txt::WIFI_CONNECTED)) / 2, 30, txt::WIFI_CONNECTED);
        u8g2.sendBuffer();

        delay(500);

    } else {
        u8g2.clearBuffer();
        u8g2.drawStr((128 - u8g2.getStrWidth(txt::WIFI_FAILED)) / 2, 30, txt::WIFI_FAILED);
        u8g2.sendBuffer();

        delay(1500);
    }
}

void syncTime()
{
    // Display a message while syncing time
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont11_tr);
    u8g2.drawStr((128 - u8g2.getStrWidth(txt::SYNCING_TIME)) / 2, 30, txt::SYNCING_TIME);
    u8g2.sendBuffer();

    // Configure time using NTP
    configTzTime(time_zone, WIFI_CONFIG.ntpServer);

    delay(1000);
}

void setup() {
    Serial.begin(115200);

    Wire.begin(SCREEN.sda_pin, SCREEN.scl_pin);
    Wire.setClock(400000);

    // Initialize the display
    u8g2.begin();

    connectWiFi();
    syncTime();

    buttonsSetup();

    delay(100);
}

void loop() {
    // Reconnect to WiFi if disconnected
    if (WiFi.status() != WL_CONNECTED) WiFi.reconnect();

    if (readButton(upButton) && screen < ScreenStates::NUM_SCREENS - 1) {
        screen++;
    }
    if (readButton(downButton) && screen > 0) {
        screen--;
    }

    if (screen != lastScreen) {
        u8g2.clearBuffer();
        u8g2.sendBuffer();
        lastScreen = screen;
        Serial.print("Screen changed to: ");
        Serial.println(screen);
        if (screen == ScreenStates::CLOCK) {
            resetPrevTime();
        }
    }

    switch (screen) {
        case ScreenStates::CLOCK:
            clockSetup();
            break;
        case ScreenStates::EYES:
            u8g2.clearBuffer();
            u8g2.drawFrame(0, 0, 128, 16);
            u8g2.drawFrame(0, 17, 128, 47);
            u8g2.sendBuffer();
            break;
        case ScreenStates::WIFI:
            drawWiFiSettings();
            break;
    }

    u8g2.sendBuffer();
}
