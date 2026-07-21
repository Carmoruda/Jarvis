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
