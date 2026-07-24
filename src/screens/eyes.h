// Copyright (c) 2026 Carmoruda. MIT License. See LICENSE for details.
// Animated robot eyes for the eyes screen.
//
// Owns eye geometry, blinking, idle movement and mood expressions.
// Drawing happens through the shared u8g2 instance. Screen-level
// navigation lives in main.cpp, not here.

#pragma once

enum class Mood {
    kDefault,
    kHappy,
    kTired,
    kAngry
};

class Eyes {
    public:
        void Begin(int fps, int spacing, Mood mood);
        void Update();

        void SetMood(Mood mood);
        void SetSpacing(int spacing);
        void SetFrameRate(int fps);
        void SetIdle(bool active);

    private:
        enum class BlinkState {
            kOpen,
            kClosing,
            kOpening
        };

        struct Eye {
            int x = 0, y = 0;
            int target_x= 0, target_y = 0;

            int width = 36, height = 36;
            int base_width = 36, base_height = 36;

            int lower_lid = 0, upper_lid_l = 0, upper_lid_r = 0;
            int target_lower_lid = 0, target_upper_lid_l = 0, target_upper_lid_r = 0;
        };

        Eye left_, right_;

        // Blink animation
        BlinkState blink_state_ = BlinkState::kOpen;
        unsigned long next_blink_ = 0;
        int close_speed_ = 3, open_speed_ = 2;

        // Idle animation
        bool idle_active_ = true;
        unsigned long next_move_ = 0;
        int interval_ms_ = 3000, idle_variation_ms_ = 2000;

        // Eye layout and timing
        int spacing_ = 10;
        unsigned long last_frame_ = 0, frame_rate_ms_ = 20;
        Mood mood_ = Mood::kDefault;

        void RecalculateBasePositions();
        int XConstraint() const;
        int YConstraint() const;

        void UpdateBlinker();
        void UpdateIdler();
        static void UpdateMoodTransition(Eye& eye);
        static void UpdatePosition(Eye& eye);

        static void DrawEye(const Eye& eye);
        void DrawAll() const;
};

extern Eyes eyes;