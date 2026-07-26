// Copyright (c) 2026 Carmoruda. MIT License. See LICENSE for details.
// Debounced button input handling.
//
// Declares the Button controller, which configures an active-low GPIO input
// with its internal pull-up resistor and detects button presses after applying
// software debouncing. Also declares the shared navigation buttons.

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