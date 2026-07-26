// Copyright (c) 2026 Carmoruda. MIT License. See LICENSE for details.
// Wi-Fi connection and status controller.
//
// Declares the Wifi controller, which establishes the network connection,
// tracks connection details, and renders the Wi-Fi status screen. Connection
// credentials come from secrets.h, and the configured NTP server is exposed.

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