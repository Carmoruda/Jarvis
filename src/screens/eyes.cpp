#include "eyes.h"
#include "hardware/display.h"

Mood current_mood = Mood::kHappy;

Idler idler;

BlinkState blink_state = BlinkState::kOpen;
Blinker blinker;

Eye left_eye  = {.x = 40, .y = 35, .width = 36, .height = 2, .base_width = 36, .base_height = 36};
Eye right_eye = {.x = 88, .y = 35, .width = 36, .height = 2, .base_width = 36, .base_height = 36};

int eye_spacing = 10;
unsigned long last_eye_frame = 0;
unsigned long eye_frame_rate_ms = 20; // Default value for 50 fps (1000/50 = 20 millis)

void SetupEyes(const int fps, const int next_blink, const int spacing, Mood mood, Eye& left, Eye& right) {
    SetFrameRate(fps);
    SetEyeSpacing(spacing);
    RecalculateBasePositions(left, right);
    SetMood(mood, left, right);
    blinker.next_blink = millis() + next_blink;
}

static void SetEyeSpacing(const int spacing) {
    eye_spacing = spacing;
}

static void SetFrameRate(const int fps) {
    eye_frame_rate_ms = fps;
}

static void SetMood(Mood mood, Eye& left, Eye& right) {
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

static void RecalculateBasePositions(Eye& left, Eye& right) {
    int total_width = left.base_width + eye_spacing + right.base_width;
    int start_x = (kScreen.width - total_width) / 2;

    left.x = start_x + left.base_width / 2;
    right.x = start_x + left.base_width + eye_spacing + right.base_width / 2;
    left.y = right.y = kScreen.height / 2;
}

void UpdateEyes(Eye& left, Eye& right) {
    if (millis() - last_eye_frame < eye_frame_rate_ms) return;
    last_eye_frame = millis();

    UpdateBlinker(blinker, left, right);
    UpdateIdler(idler, left, right);
    UpdateMoodTransition(left);
    UpdateMoodTransition(right);
    UpdatePosition(left);
    UpdatePosition(right);
    DrawEyes(left, right);
}

static void UpdateMoodTransition(Eye& eye) {
    eye.lower_lid += static_cast<int>((eye.target_lower_lid     - eye.lower_lid)     * 0.3);
    eye.upper_lid_l += static_cast<int>((eye.target_upper_lid_l  - eye.upper_lid_l)   * 0.3);
    eye.upper_lid_r += static_cast<int>((eye.target_upper_lid_r  - eye.upper_lid_r)   * 0.3);

}

static int GetXConstraint(const Eye& left, const Eye& right) {
   return kScreen.width - left.base_width - eye_spacing - right.base_width;
}

static int GetYConstraint(const Eye& left, const Eye& right) {
    return (kScreen.height - max(left.base_height, right.base_height));
}

static void UpdateIdler(Idler& idler_controller, Eye& left, Eye& right) {
    const unsigned long now = millis();

    if (idler_controller.active && now >= idler_controller.next_move) {
        const int dx = random(GetXConstraint(left, right));
        const int dy = random(GetYConstraint(left, right));

        const int left_edge_x = dx;
        const int right_edge_x = dx + left.base_width + eye_spacing;

        left.target_x = left_edge_x + left.base_width / 2;
        right.target_x = right_edge_x + right.base_width / 2;
        left.target_y = right.target_y = dy + left.base_height / 2;

        idler_controller.next_move = now + idler_controller.interval_ms + random(idler_controller.variation_ms);
    }
}

static void UpdatePosition(Eye& eye) {
    eye.x += static_cast<int>((eye.target_x - eye.x) * 0.3);
    eye.y += static_cast<int>((eye.target_y - eye.y) * 0.3);
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

static void DrawEyes(const Eye& left, const Eye& right) {
    u8g2.clearBuffer();
    DrawEye(left);
    DrawEye(right);
    u8g2.sendBuffer();
}
