#include <WiFi.h>
#include <strings.h>
#include "secrets.h"
#include "hardware/display.h"
#include "screens/wifi_settings.h"

WiFiConfig WIFI_CONFIG = {WIFI_SSID, WIFI_PASS, "pool.ntp.org"};

void drawWiFiSettings() {
    String ssid = WiFi.SSID();
    String ip = WiFi.localIP().toString();
    String status = (WiFi.status() == WL_CONNECTED) ? "Connected" : "Disconnected";
    int rssi = WiFi.RSSI();

    u8g2.clearBuffer();

    u8g2.drawFrame(0, 0, 128, 16);
    u8g2.drawFrame(0, 17, 128, 47);

    // Screen title
    u8g2.setFont(u8g2_font_tiny5_tr);
    u8g2.drawStr((128 - u8g2.getStrWidth(txt::WIFI_SETTINGS_TITLE)) / 2, 10, txt::WIFI_SETTINGS_TITLE);

    // WiFi settings
    u8g2.setFont(u8g2_font_profont11_tr);
    u8g2.drawStr(10, 40, ("Status: " + status).c_str());
    u8g2.drawStr(10, 30, ("SSID: " + ssid).c_str());
    u8g2.drawStr(10, 50, ("IP: " + ip).c_str());
    u8g2.drawStr(10, 60, ("RSSI: " + String(rssi) + " dBm").c_str());

    u8g2.sendBuffer();
}
