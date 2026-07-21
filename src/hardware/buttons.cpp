#include "buttons.h"

Button up_button = {.pin = 32, .last_state = HIGH, .stable_state = HIGH, .last_change_time = 0, .debounce_time = 50};
Button down_button = {.pin = 27, .last_state = HIGH, .stable_state = HIGH, .last_change_time = 0, .debounce_time = 50};

void ButtonsSetup() {
    pinMode(up_button.pin, INPUT_PULLUP);
    pinMode(down_button.pin, INPUT_PULLUP);
}

bool ReadButton(Button &button) {
    const int reading = digitalRead(button.pin);

    // Check if the button state has changed
    if (reading != button.last_state) {
        button.last_change_time = millis();
        button.last_state = reading;
    }

    // If the button hasn't been stable for the debounce time, ignore it
    if ((millis() - button.last_change_time) <= button.debounce_time) return false;

    if (reading != button.stable_state) {
        button.stable_state = reading;
        return (reading == LOW); // Button pressed
    }

    return false; // Button not pressed
}
