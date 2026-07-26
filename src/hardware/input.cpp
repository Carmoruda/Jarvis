#include "input.h"

Input touch(5, HIGH, false);
// Input up_button(4, LOW, true);
// Input down_button(5, LOW, true);

Input::Input(const uint8_t pin, const int active_level, const bool use_pullup, const unsigned long debounce_ms):
    pin_(pin),
    active_level_(active_level),
    use_pullup_(use_pullup),
    debounce_ms_(debounce_ms),
    last_state_(!active_level),
    stable_state_(!active_level) {}

void Input::Setup() const {
    pinMode(pin_, use_pullup_ ? INPUT_PULLUP : INPUT);
}

bool Input::Pressed() {
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
        return (reading == active_level_);
    }

    return false;
}
