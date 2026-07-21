#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "secrets.h"
#include "strings.h"
#include "screens/clock.h"
#include "screens/wifi_settings.h"
#include "screens/weather.h"
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
    EYES,
    CLOCK,
    WEATHER,
    WIFI,
    NUM_SCREENS
};

constexpr Screen SCREEN = {128, 64, 21, 22};

// Screen
int screen = ScreenStates::CLOCK;
int lastScreen = -1;

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
        } else if (screen == ScreenStates::WIFI) {
            resetWiFiStatus();
        }

    }

    switch (screen) {
        // case ScreenStates::EYES:
        //     u8g2.clearBuffer();
        //     u8g2.drawFrame(0, 0, 128, 16);
        //     u8g2.drawFrame(0, 17, 128, 47);
        //     u8g2.sendBuffer();
        //     break;
        case ScreenStates::CLOCK:
            clockSetup();
            break;
        // case ScreenStates::WEATHER:
        //     weather();
        //     break;
        case ScreenStates::WIFI:
            WiFiStatus();
            break;
    }

    u8g2.sendBuffer();
}
