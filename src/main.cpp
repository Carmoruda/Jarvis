#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include "screens/eyes.h"
#include "screens/clock.h"
#include "screens/weather.h"
#include "screens/wifi.h"
#include "hardware/display.h"
#include "hardware/input.h"

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
static int screen = kEyes;
static int last_screen = -1;

void setup() {
    Serial.begin(115200);

    Wire.begin(kScreen.sda_pin, kScreen.scl_pin);
    Wire.setClock(400000);

    // Initialize the display
    u8g2.begin();

    wifi.Connect();
    time_clock.Sync();
    eyes.Begin(50, 10, Mood::kDefault);
    touch.Setup();

    delay(100);
}

void loop() {
    // Reconnect to WiFi if disconnected
    if (WiFi.status() != WL_CONNECTED) WiFi.reconnect();

    if (touch.Pressed()) {
        screen = (screen + 1) % kNumScreens;
    }

    if (screen != last_screen) {
        u8g2.clearBuffer();
        u8g2.sendBuffer();
        last_screen = screen;
        Serial.print("Screen changed to: ");
        Serial.println(screen);

        if (screen == kClock) {
            time_clock.Reset();
        } else if (screen == kWifi) {
            wifi.Reset();
        } else if (screen == kWeather) {
            weather.Reset();
        }

    }

    switch (screen) {
        case kEyes:
            eyes.Update();
            break;
        case kClock:
            time_clock.Update();
            break;
        case kWeather:
            weather.Update();
            break;
        case kWifi:
            wifi.Update();
            break;
        default: ;
    }
}
