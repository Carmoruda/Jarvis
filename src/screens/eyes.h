#pragma once

enum class BlinkState {
    kOpen,
    kClosing,
    kOpening
};

enum class Mood {
    kDefault,
    kHappy,
    kTired,
    kAngry
};

struct Eye {
    int x, y;
    int target_x, target_y;

    int width, height;
    int base_width, base_height;

    int lower_lid;
    int upper_lid_l, upper_lid_r;

    int target_lower_lid;
    int target_upper_lid_l, target_upper_lid_r;
};

struct Idler {
    bool active = true;
    unsigned long next_move = 0;
    int interval_ms = 3000;
    int variation_ms = 2000;
};

struct Blinker {
    BlinkState state = BlinkState::kOpen;
    unsigned long next_blink = 0;
    int close_speed = 3;
    int open_speed = 2;
};

extern BlinkState blink_state;
extern Blinker blinker;
extern Idler idler;

extern Eye left_eye;
extern Eye right_eye;
extern int eye_spacing;
extern Mood current_mood;

extern unsigned long last_eye_frame;
extern unsigned long eye_frame_rate_ms;

void SetupEyes(int fps, int next_blink, int spacing, Mood mood, Eye& left, Eye& right);
void UpdateEyes(Eye& left, Eye& right);
static int GetXConstraint(const Eye& left, const Eye& right);
static int GetYConstraint(const Eye& left, const Eye& right);
static void SetFrameRate(int fps);
static void SetEyeSpacing(int spacing);
static void SetMood(Mood mood, Eye& left, Eye& right);
static void RecalculateBasePositions(Eye& left, Eye& right);
static void UpdateMoodTransition(Eye& eye);
static void UpdatePosition(Eye& eye);
static void UpdateBlinker(Blinker& b, Eye& left, Eye& right);
static void UpdateIdler(Idler& idler_controller, Eye& left, Eye& right);
static void DrawEye(const Eye& eye);
static void DrawEyes(const Eye& left, const Eye& right);