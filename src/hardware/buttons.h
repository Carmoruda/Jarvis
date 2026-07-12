#pragma once
#include <Arduino.h>

struct Button {
    public:
        uint8_t pin;
        int lastState;
        int stableState;
        unsigned long lastChangeTime;
        const unsigned long debounceTime;
};

extern Button upButton;
extern Button downButton;

extern int screen;
extern int lastScreen;

bool readButton(Button &button);
void buttonsSetup();
