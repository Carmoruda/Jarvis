#pragma once

struct WiFiConfig {
    public:
        const char *ssid;
        const char *password;
        const char *ntpServer;
};

extern WiFiConfig WIFI_CONFIG;

void drawWiFiSettings();
