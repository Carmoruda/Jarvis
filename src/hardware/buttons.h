// Copyright (c) 2026 Carmoruda. MIT License. See LICENSE for details.
// OLED display configuration.
//
// Declares the shared U8g2 display instance used to render the application
// screens. The display uses an SSD1306 controller, a 128x64 resolution,
// full-buffer rendering, and the hardware I2C interface.

#pragma once

#include <Arduino.h>

struct Button {
    public:
        uint8_t pin;
        int last_state;
        int stable_state;
        unsigned long last_change_time;
        const unsigned long debounce_time;
};

extern Button up_button;
extern Button down_button;

extern int screen;
extern int last_screen;

bool ReadButton(Button &button);
void ButtonsSetup();
