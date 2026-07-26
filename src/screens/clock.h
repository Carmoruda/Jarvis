// Copyright (c) 2026 Carmoruda. MIT License. See LICENSE for details.
// Clock synchronization and display controller.
//
// Declares the Clock controller, which synchronizes local time through NTP,
// retrieves the current date and time, and renders the clock screen when the
// displayed minute changes. The time zone is configured locally, while the
// NTP server is provided by the Wi-Fi controller.

#pragma once

#include <Arduino.h>

class Clock {
    public:
        void Sync();
        void Update();
        void Reset();
    private:
        // Central European Time (CET) with daylight saving time
        static constexpr const char* kTimeZone = "CET-1CEST,M3.5.0,M10.5.0/3";

        static void Draw(const String& hour_str, const String& min_str, const String& date);

        // Tracking
        int prev_hour_ = -1;
        int prev_min_ = -1;
};

extern Clock time_clock;