// Copyright (c) 2026 Carmoruda. MIT License. See LICENSE for details.
// OLED display configuration.
//
// Declares the shared U8g2 display instance used to render the application
// screens. The display uses an SSD1306 controller, a 128x64 resolution,
// full-buffer rendering, and the hardware I2C interface.

#pragma once

#include <Arduino.h>

class Button {
    public:
        explicit Button(uint8_t pin, unsigned long debounce_ms = 50);

        void Setup() const;
        bool Read();

    private:
        uint8_t pin_;
        unsigned long debounce_ms_;
        int last_state_ = HIGH;
        int stable_state_ = HIGH;
        unsigned long last_change_time_ = 0;
};

extern Button up_button;
extern Button down_button;