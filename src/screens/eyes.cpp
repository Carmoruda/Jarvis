#include "eyes.h"
#include "hardware/display.h"

Mood current_mood = Mood::kHappy;

BlinkState blink_state = BlinkState::kOpen;
Blinker blinker;

Eye left_eye  = {.x = 40, .y = 35, .width = 36, .height = 2, .base_width = 36, .base_height = 36};
Eye right_eye = {.x = 88, .y = 35, .width = 36, .height = 2, .base_width = 36, .base_height = 36};

int eye_spacing = 18;
unsigned long last_eye_frame = 0;
unsigned long eye_frame_rate_ms = 20; // Default value for 50 fps (1000/50 = 20 millis)

void SetupEyes(const int fps, const int next_blink, const int spacing, Mood mood, Eye& left, Eye& right) {
    SetFrameRate(fps);
    SetEyeSpacing(spacing);
    SetMood(mood, left, right);
    blinker.next_blink = millis() + next_blink;
}

void SetEyeSpacing(const int spacing) {
    eye_spacing = spacing;
}

void SetFrameRate(const int fps) {
    eye_frame_rate_ms = fps;
}

void SetMood(Mood mood, Eye& left, Eye& right) {
    current_mood = mood;

    switch (mood) {
        case Mood::kDefault:
            left.target_lower_lid = right.target_lower_lid = 0;
            left.target_upper_lid_l = left.target_upper_lid_r = 0;
            right.target_upper_lid_l = right.target_upper_lid_r = 0;
            break;

        case Mood::kHappy:
            left.target_lower_lid = right.target_lower_lid = 14;
            left.target_upper_lid_l = left.target_upper_lid_r = 0;
            right.target_upper_lid_l = right.target_upper_lid_r = 0;
            break;

        case Mood::kTired:
            left.target_lower_lid = right.target_lower_lid = 0;
            left.target_upper_lid_l = left.target_upper_lid_r = 12;
            right.target_upper_lid_l = right.target_upper_lid_r = 12;
            break;

        case Mood::kAngry:
            left.target_lower_lid = right.target_lower_lid = 0;
            left.target_upper_lid_l = 0;
            left.target_upper_lid_r = left.base_height - 10;
            right.target_upper_lid_l = right.base_height - 10;
            right.target_upper_lid_r = 0;
            break;

    }
}

void RecalculateEyePosition(Eye& left, Eye& right) {
    const int half_gap = eye_spacing / 2;
    constexpr int half_width = kScreen.width / 2;

    left.x = half_width - half_gap - left.base_width / 2;
    right.x = half_width + half_gap + right.base_width / 2;

}

void UpdateEyes(Eye& left, Eye& right) {
    if (millis() - last_eye_frame < eye_frame_rate_ms) return;
    last_eye_frame = millis();

    UpdateBlinker(blinker, left, right);
    UpdateMoodTransition(left);
    UpdateMoodTransition(right);
    DrawEyes(left, right);
}

static void UpdateMoodTransition(Eye& eye) {
    eye.lower_lid     += static_cast<int>((eye.target_lower_lid     - eye.lower_lid)     * 0.3);
    eye.upper_lid_l    += static_cast<int>((eye.target_upper_lid_l  - eye.upper_lid_l)   * 0.3);
    eye.upper_lid_r    += static_cast<int>((eye.target_upper_lid_r  - eye.upper_lid_r)   * 0.3);

}

static void UpdateBlinker(Blinker& b, Eye& left, Eye& right) {
    const unsigned long now = millis();

    switch (b.state) {
        case BlinkState::kOpen:
            if (now >= b.next_blink) b.state = BlinkState::kClosing;
            break;

        case BlinkState::kClosing:
            left.height -= b.close_speed;
            right.height -= b.close_speed;

            if (left.height <= 2) {
                left.height = right.height = 2;
                b.state = BlinkState::kOpening;
            }
            break;

        case BlinkState::kOpening:
            left.height += b.open_speed;
            right.height += b.open_speed;

            if (left.height >= left.base_height) {
                left.height = left.base_height;
                right.height = right.base_height;
                b.state = BlinkState::kOpen;
                b.next_blink = now + random(1000, 3000);
            }
            break;
    }
}

static void DrawEye(const Eye& eye) {
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

void DrawEyes(const Eye& left, const Eye& right) {
    u8g2.clearBuffer();
    DrawEye(left);
    DrawEye(right);
    u8g2.sendBuffer();
}
