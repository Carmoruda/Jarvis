#include "buttons.h"

Button upButton = {32, HIGH, HIGH, 0, 50}; // UP button (Next Screen)
Button downButton = {27, HIGH, HIGH, 0, 50}; // DOWN button (Previous Screen)

void buttonsSetup() {
    pinMode(upButton.pin, INPUT_PULLUP);
    pinMode(downButton.pin, INPUT_PULLUP);
}

bool readButton(Button &button) {
    int reading = digitalRead(button.pin);

    // Check if the button state has changed
    if (reading != button.lastState) {
        button.lastChangeTime = millis();
        button.lastState = reading;
    }

    // If the button hasn't been stable for the debounce time, ignore it
    if ((millis() - button.lastChangeTime) <= button.debounceTime) return false;

    if (reading != button.stableState) {
        button.stableState = reading;
        return (reading == LOW); // Button pressed
    }

    return false; // Button not pressed
}
