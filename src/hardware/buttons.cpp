#include "buttons.h"

Button up_button(4);
Button down_button(5);

Button::Button(const uint8_t pin, const unsigned long debounce_ms): pin_(pin), debounce_ms_(debounce_ms) {}

void Button::Setup() const {
    pinMode(pin_, INPUT_PULLUP);
}

bool Button::Read() {
    const int reading = digitalRead(pin_);

    // Check if the button state has changed
    if (reading != last_state_) {
        last_change_time_ = millis();
        last_state_ = reading;
    }

    // If the button hasn't been stable for the debounce time, ignore it
    if ((millis() - last_change_time_) <= debounce_ms_) return false;

    // If the button isn't stable, read it
    if (reading != stable_state_) {
        stable_state_ = reading;
        return (reading == LOW); // Button pressed
    }

    return false;
}
