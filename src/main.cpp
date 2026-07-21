#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "screens/clock.h"
#include "screens/wifi_settings.h"
#include "screens/weather.h"
#include "hardware/display.h"
#include "hardware/buttons.h"

namespace {
    struct Screen {
    public:
        uint8_t width;
        uint8_t height;
        uint8_t sda_pin;
        uint8_t scl_pin;
    };
}

namespace {
    enum ScreenStates {
        kEyes,
        kClock,
        kWeather,
        kWifi,
        kNumScreens
    };
}

constexpr Screen kScreen = {.width = 128, .height = 64, .sda_pin = 21, .scl_pin = 22};

// Screen
int screen = ScreenStates::kClock;
int last_screen = -1;

void setup() {
    Serial.begin(115200);

    Wire.begin(kScreen.sda_pin, kScreen.scl_pin);
    Wire.setClock(400000);

    // Initialize the display
    u8g2.begin();

    ConnectWifi();
    SyncTime();

    ButtonsSetup();

    delay(100);
}

void loop() {
    // Reconnect to WiFi if disconnected
    if (WiFiClass::status() != WL_CONNECTED) WiFi.reconnect();

    if (ReadButton(up_button) && screen < ScreenStates::kNumScreens - 1) {
        screen++;
    }
    if (ReadButton(down_button) && screen > 0) {
        screen--;
    }

    if (screen != last_screen) {
        u8g2.clearBuffer();
        u8g2.sendBuffer();
        last_screen = screen;
        Serial.print("Screen changed to: ");
        Serial.println(screen);

        if (screen == ScreenStates::kClock) {
            ResetPrevTime();
        } else if (screen == ScreenStates::kWifi) {
            ResetWifiStatus();
        }

    }

    switch (screen) {
        case ScreenStates::kEyes:
            u8g2.clearBuffer();
            u8g2.drawFrame(0, 0, 128, 16);
            u8g2.drawFrame(0, 17, 128, 47);
            u8g2.sendBuffer();
            break;
        case ScreenStates::kClock:
            ClockSetup();
            break;
        case ScreenStates::kWeather:
            UpdateWeather();
            break;
        case ScreenStates::kWifi:
            WifiStatus();
            break;
        default: ;
    }

    u8g2.sendBuffer();
}
