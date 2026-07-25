#include "screens/wifi.h"

#include <WiFi.h>

#include "ui/strings.h"
#include "hardware/display.h"

Wifi wifi;

void Wifi::Connect() {
    // Message while connecting to WiFi
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvR08_tr);
    DrawHorizontallyCentered(txt::kWifiConnecting, 30);
    u8g2.sendBuffer();

    WiFi.begin(kConfig.ssid, kConfig.password);

    // Wait for connection with a timeout of 30 seconds (100 * 300ms)
    uint8_t retry = 0;
    while (WiFiClass::status() != WL_CONNECTED && retry < 100) {
        delay(300);
        retry++;
    }

    if (WiFiClass::status() == WL_CONNECTED) {
        u8g2.clearBuffer();
        DrawHorizontallyCentered(txt::kWifiConnected, 30);
        u8g2.sendBuffer();

        prev_ssid_ = WiFi.SSID();
        prev_status_ = (WiFiClass::status() == WL_CONNECTED) ? "Connected" : "Disconnected";

        delay(500);
    } else {
        u8g2.clearBuffer();
        DrawHorizontallyCentered(txt::kWifiFailed, 30);
        u8g2.sendBuffer();

        delay(1500);
    }

}

void Wifi::Draw(const String &ssid, const String &ip, const String &status, int rssi) {
    u8g2.clearBuffer();

    u8g2.drawLine(5, 15, 123, 15);

    // Screen title
    u8g2.setFont(u8g2_font_helvR08_tr);
    DrawHorizontallyCentered(txt::kWifiSettingsTitle, 10);

    // WiFi settings
    u8g2.drawStr(5, 30, ("SSID: " + ssid).c_str());
    u8g2.drawStr(5, 40, ("Status: " + status).c_str());
    u8g2.drawStr(5, 50, ("IP: " + ip).c_str());
    u8g2.drawStr(5, 60, ("RSSI: " + String(rssi) + " dBm").c_str());

    u8g2.sendBuffer();
}

void Wifi::Reset() {
    prev_ssid_ = "N/A";
    prev_ip_ = "N/A";
    prev_status_ = "N/A";
    prev_rssi_ = -1;
}

void Wifi::Update() {
    // Get WiFi status
    const String ssid = WiFi.SSID();
    const String ip = WiFi.localIP().toString();
    const String status = (WiFiClass::status() == WL_CONNECTED) ? "Connected" : "Disconnected";
    const int rssi = WiFi.RSSI();

    // Only update the display if the time has changed
    if (ssid != prev_ssid_ || ip != prev_ip_ || status != prev_status_ || rssi != prev_rssi_) {
        Draw(ssid, ip, status, rssi);
        prev_ssid_ = ssid;
        prev_ip_ = ip;
        prev_status_ = status;
        prev_rssi_ = rssi;
    }
}

const char* Wifi::GetNtpServer() const {
    return kConfig.ntpServer;
}
