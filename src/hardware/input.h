// Copyright (c) 2026 Carmoruda. MIT License. See LICENSE for details.
// Debounced button input handling.
//
// Declares the Input controller, which configures a digital GPIO with an
// optional internal pull-up resistor, supports configurable active levels,
// and applies software debouncing to report new activation events. Also
// declares the shared application inputs.

#pragma once

#include <Arduino.h>

class Input {
    public:
        Input(uint8_t pin, int active_level, bool use_pullup, unsigned long debounce_ms = 50);

        void Setup() const;
        bool Pressed();

    private:
        uint8_t pin_;
        int active_level_;
        bool use_pullup_;
        unsigned long debounce_ms_;
        int last_state_ = HIGH;
        int stable_state_ = HIGH;
        unsigned long last_change_time_ = 0;

};

extern Input touch;
// extern Input up_button;
// extern Input down_button;