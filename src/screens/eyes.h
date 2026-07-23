#pragma once

enum class BlinkState {
    kOpen,
    kClosing,
    kOpening
};

struct Eye {
    public:
        int x, y;
        int width, height;
        int base_width, base_height;
};

struct Blinker {
    public:
        BlinkState state = BlinkState::kOpen;
        unsigned long next_blink = 0;
        int close_speed = 3;
        int open_speed = 2;
};

extern BlinkState blink_state;
extern Blinker blinker;

extern Eye left_eye;
extern Eye right_eye;
extern int eye_spacing;

extern unsigned long last_eye_frame;
extern unsigned long eye_frame_rate_ms;

void SetupEyes(int fps, int next_blink, int spacing);
void SetFrameRate(int fps);
void SetEyeSpacing(int spacing);
void RecalculateEyePosition(Eye& left, Eye& right);
void UpdateEyes(Eye& left, Eye& right);
static void UpdateBlinker(Blinker& b, Eye& left, Eye& right);
static void DrawEye(const Eye& eye);
void DrawEyes(const Eye& left, const Eye& right);