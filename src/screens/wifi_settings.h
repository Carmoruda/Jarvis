// Copyright (c) 2026 Carmoruda. MIT License. See LICENSE for details.
// Wi-Fi connection and status screen.
//
// Manages connecting to Wi-Fi, NTP time sync, and rendering the
// Wi-Fi status screen. Connection credentials come from secrets.h.
// This file owns the Wi-Fi screen only; clock/eye rendering lives elsewhere.
//

#pragma once

#include "secrets.h"

struct WiFiConfig {
    const char* ssid;
    const char* password;
    const char* ntpServer;
};

constexpr WiFiConfig kWifiConfig = {
    .ssid = WIFI_SSID,
    .password = WIFI_PASS,
    .ntpServer = "pool.ntp.org"
};

void ConnectWifi();
void DrawWifiSettings(const String &ssid, const String &ip, const String &status, int rssi);
void ResetWifiStatus();
void WifiStatus();
