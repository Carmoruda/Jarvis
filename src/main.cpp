#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "screens/eyes.h"
#include "screens/clock.h"
#include "screens/weather.h"
#include "screens/wifi.h"
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

    wifi.Connect();
    time_clock.Sync();
    eyes.Begin(50, 10, Mood::kDefault);
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
            time_clock.Reset();
        } else if (screen == ScreenStates::kWifi) {
            wifi.Reset();
        } else if (screen == ScreenStates::kWeather) {
            weather.ResetView();
        }

    }

    switch (screen) {
        case ScreenStates::kEyes:
            eyes.Update();
            break;
        case ScreenStates::kClock:
            time_clock.Update();
            break;
        case ScreenStates::kWeather:
            weather.Update();
            break;
        case ScreenStates::kWifi:
            wifi.Update();
            break;
        default: ;
    }

    u8g2.sendBuffer();
}
