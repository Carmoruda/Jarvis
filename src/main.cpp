#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "screens/eyes.h"
#include "screens/clock.h"
#include "screens/weather.h"
#include "screens/wifi_settings.h"
#include "hardware/display.h"
#include "hardware/buttons.h"

namespace {
    enum ScreenStates {
        kEyes,
        kClock,
        kWeather,
        kWifi,
        kNumScreens
    };
}

// Screen
int screen = ScreenStates::kEyes;
int last_screen = -1;

void setup() {
    Serial.begin(115200);

    Wire.begin(kScreen.sda_pin, kScreen.scl_pin);
    Wire.setClock(400000);

    // Initialize the display
    u8g2.begin();

    ConnectWifi();
    SyncTime();
    SetupEyes(20, 2000, 12, Mood::kDefault, left_eye, right_eye);
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
            UpdateEyes(left_eye, right_eye);
            break;
        case ScreenStates::kClock:
            UpdateClock();
            break;
        case ScreenStates::kWeather:
            UpdateWeather();
            break;
        case ScreenStates::kWifi:
            UpdateWifiStatus();
            break;
        default: ;
    }

    u8g2.sendBuffer();
}
