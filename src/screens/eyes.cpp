#include "eyes.h"
#include "hardware/display.h"

Eyes eyes;

void Eyes::Begin(const int fps, const int spacing, const Mood mood) {
    SetFrameRate(fps);
    SetSpacing(spacing);
    SetMood(mood);
    next_blink_ = millis() + 2000;
}

void Eyes::SetFrameRate(const int fps) {
    frame_rate_ms_ = 1000 / fps;
}

void Eyes::SetSpacing(const int spacing) {
    spacing_ = spacing;
    RecalculateBasePositions();
}

void Eyes::SetIdle(const bool active) {
    idle_active_ = active;
}

void Eyes::SetMood(const Mood mood) {
    mood_ = mood;

    switch (mood) {
        case Mood::kDefault:
            left_.target_lower_lid = right_.target_lower_lid = 0;
            left_.target_upper_lid_l = left_.target_upper_lid_r = 0;
            right_.target_upper_lid_l = right_.target_upper_lid_r = 0;
            break;

        case Mood::kHappy:
            left_.target_lower_lid = right_.target_lower_lid = 14;
            left_.target_upper_lid_l = left_.target_upper_lid_r = 0;
            right_.target_upper_lid_l = right_.target_upper_lid_r = 0;
            break;

        case Mood::kTired:
            left_.target_lower_lid = right_.target_lower_lid = 0;
            left_.target_upper_lid_l = left_.target_upper_lid_r = 12;
            right_.target_upper_lid_l = right_.target_upper_lid_r = 12;
            break;

        case Mood::kAngry:
            left_.target_lower_lid = right_.target_lower_lid = 0;
            left_.target_upper_lid_l = 0;
            left_.target_upper_lid_r = left_.base_height - 10;
            right_.target_upper_lid_l = right_.base_height - 10;
            right_.target_upper_lid_r = 0;
            break;
    }
}

void Eyes::RecalculateBasePositions() {
    const int total_width = left_.base_width + spacing_ + right_.base_width;
    const int start_x = (kScreen.width - total_width) / 2;

    left_.x = left_.target_x = start_x + left_.base_width / 2;
    right_.x = right_.target_x = start_x + left_.base_width + spacing_ + right_.base_width / 2;
    left_.y = right_.y = left_.target_y = right_.target_y = kScreen.height / 2;
}

int Eyes::XConstraint() const {
    return kScreen.width - left_.base_width - spacing_ - right_.base_width;
}

int Eyes::YConstraint() const {
    return kScreen.height - max(left_.base_height, right_.base_height);
}

void Eyes::UpdateBlinker() {
    const unsigned long now = millis();

    switch (blink_state_) {
        case BlinkState::kOpen:
            if (now >= next_blink_) blink_state_ = BlinkState::kClosing;
            break;

        case BlinkState::kClosing:
            left_.height -= close_speed_;
            right_.height -= close_speed_;

            if (left_.height <= 2) {
                left_.height = right_.height = 2;
                blink_state_ = BlinkState::kOpening;
            }
            break;

        case BlinkState::kOpening:
            left_.height += open_speed_;
            right_.height += open_speed_;

            if (left_.height >= left_.base_height) {
                left_.height = left_.base_height;
                right_.height = right_.base_height;
                blink_state_ = BlinkState::kOpen;
                next_blink_ = now + random(1000, 3000);
            }
            break;
    }
}

void Eyes::UpdateIdler() {
    const unsigned long now = millis();

    if (!idle_active_ || now < next_move_) return;

    const int dx = random(XConstraint());
    const int dy = random(YConstraint());

    left_.target_x = dx + left_.base_width / 2;
    right_.target_x = dx + left_.base_width + spacing_ + right_.base_width / 2;
    left_.target_y = right_.target_y = dy + left_.base_height / 2;

    next_move_ = now + interval_ms_ + random(idle_variation_ms_);
}

void Eyes::UpdateMoodTransition(Eye &eye) {
    eye.lower_lid += static_cast<int>((eye.target_lower_lid     - eye.lower_lid)     * 0.3);
    eye.upper_lid_l += static_cast<int>((eye.target_upper_lid_l  - eye.upper_lid_l)   * 0.3);
    eye.upper_lid_r += static_cast<int>((eye.target_upper_lid_r  - eye.upper_lid_r)   * 0.3);
}

void Eyes::UpdatePosition(Eye &eye) {
    eye.x += static_cast<int>((eye.target_x - eye.x) * 0.3);
    eye.y += static_cast<int>((eye.target_y - eye.y) * 0.3);
}

void Eyes::DrawEye(const Eye& eye) {
    const int x = eye.x - eye.width / 2;
    const int y = eye.y - eye.height / 2;
    const int r = min(8, eye.height / 2);

    u8g2.drawRBox(x, y, eye.width, eye.height, r);

    if (eye.upper_lid_l > 0 || eye.upper_lid_r > 0) {
        u8g2.setDrawColor(0);
        u8g2.drawTriangle(  static_cast<int16_t>(x),              static_cast<int16_t>(y),
                            static_cast<int16_t>(x + eye.width),  static_cast<int16_t>(y),
                            static_cast<int16_t>(x),              static_cast<int16_t>(y + eye.upper_lid_l));
        u8g2.drawTriangle(  static_cast<int16_t>(x + eye.width),  static_cast<int16_t>(y),
                            static_cast<int16_t>(x),              static_cast<int16_t>(y + eye.upper_lid_l),
                            static_cast<int16_t>(x + eye.width),  static_cast<int16_t>(y + eye.upper_lid_r));
        u8g2.setDrawColor(1);
    }

    if (eye.lower_lid > 0) {
        u8g2.setDrawColor(0);
        u8g2.drawTriangle(  static_cast<int16_t>(x),                  static_cast<int16_t>(y + eye.height),
                            static_cast<int16_t>(x + eye.width),      static_cast<int16_t>(y + eye.height),
                            static_cast<int16_t>(x + eye.width / 2),  static_cast<int16_t>(y + eye.height - eye.lower_lid));
        u8g2.setDrawColor(1);
    }
}

void Eyes::DrawAll() const {
    u8g2.clearBuffer();
    DrawEye(left_);
    DrawEye(right_);
    u8g2.sendBuffer();
}

void Eyes::Update() {
    if (millis() - last_frame_ < frame_rate_ms_) return;
    last_frame_ = millis();

    UpdateBlinker();
    UpdateIdler();
    UpdateMoodTransition(left_);
    UpdateMoodTransition(right_);
    UpdatePosition(left_);
    UpdatePosition(right_);
    DrawAll();
}