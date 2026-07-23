#include "eyes.h"
#include "hardware/display.h"

BlinkState blink_state = BlinkState::kOpen;
Blinker blinker;

Eye left_eye = {.x = 40, .y = 35, .width = 36, .height = 2, .base_width = 36, .base_height = 36};
Eye right_eye = {.x = 88, .y = 35, .width = 36, .height = 2, .base_width = 36, .base_height = 36};
int eye_spacing = 18;

unsigned long last_eye_frame = 0;
unsigned long eye_frame_rate_ms = 20; // Default value for 50 fps (1000/50 = 20 millis)

void SetupEyes(const int fps, const int next_blink, const int spacing) {
    SetFrameRate(fps);
    SetEyeSpacing(spacing);
    blinker.next_blink = millis() + next_blink;
}

void SetEyeSpacing(const int spacing) {
    eye_spacing = spacing;
}

void SetFrameRate(const int fps) {
    eye_frame_rate_ms = fps;
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
    DrawEyes(left, right);
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
}

void DrawEyes(const Eye& left, const Eye& right) {
    u8g2.clearBuffer();
    DrawEye(left);
    DrawEye(right);
    u8g2.sendBuffer();
}
