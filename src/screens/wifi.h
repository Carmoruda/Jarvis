// Copyright (c) 2026 Carmoruda. MIT License. See LICENSE for details.
// Wi-Fi connection and status screen.
//
// Manages connecting to Wi-Fi, NTP time sync, and rendering the
// Wi-Fi status screen. Connection credentials come from secrets.h.
// This file owns the Wi-Fi screen only; clock/eye rendering lives elsewhere.

#pragma once

#include <Arduino.h>
#include "secrets.h"

class Wifi {
    public:
        void Connect();
        void Update();
        void Reset();

        const char *GetNtpServer() const;
    private:
        struct Config {
            const char* ssid;
            const char* password;
            const char* ntpServer;
        };

        static constexpr Config kConfig = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .ntpServer = "pool.ntp.org"
        };

        static void Draw(const String &ssid, const String &ip, const String &status, int rssi);

        // Tracking
        String prev_ssid_ = "N/A";
        String prev_ip_ = "N/A";
        String prev_status_ = "N/A";
        int prev_rssi_ = -1;
};

extern Wifi wifi;