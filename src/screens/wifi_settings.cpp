#include <WiFi.h>
#include <strings.h>
#include "hardware/display.h"
#include "screens/wifi_settings.h"

// WiFi status variables to track changes
static String prev_ssid = "N/A";
static String prev_ip = "N/A";
static String prev_status = "N/A";
static int prev_rssi = -1;

void ConnectWifi()
{
    // Display a message while connecting to WiFi
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont11_tr);
    u8g2.drawStr((128 - u8g2.getStrWidth(txt::kWifiConnecting)) / 2, 30, txt::kWifiConnecting);
    u8g2.sendBuffer();

    // Connect to WiFi
    WiFi.begin(kWifiConfig.ssid, kWifiConfig.password);

    uint8_t retry = 0;

    // Wait for connection with a timeout of 30 seconds (100 * 300ms)
    while (WiFiClass::status() != WL_CONNECTED && retry < 100) {
        delay(300);
        retry++;
    }

    // Display connection result
    if (WiFiClass::status() == WL_CONNECTED) {

        u8g2.clearBuffer();
        u8g2.drawStr((128 - u8g2.getStrWidth(txt::kWifiConnected)) / 2, 30, txt::kWifiConnected);
        u8g2.sendBuffer();

        delay(500);

    } else {
        u8g2.clearBuffer();
        u8g2.drawStr((128 - u8g2.getStrWidth(txt::kWifiFailed)) / 2, 30, txt::kWifiFailed);
        u8g2.sendBuffer();

        delay(1500);
    }
}

void DrawWifiSettings(const String &ssid, const String &ip, const String &status, const int rssi) {
    u8g2.clearBuffer();

    u8g2.drawLine(5, 15, 123, 15);

    // Screen title
    u8g2.setFont(u8g2_font_profont11_tr);
    u8g2.drawStr((128 - u8g2.getStrWidth(txt::kWifiSettingsTitle)) / 2, 10, txt::kWifiSettingsTitle);

    // WiFi settings
    u8g2.drawStr(5, 30, ("SSID: " + ssid).c_str());
    u8g2.drawStr(5, 40, ("Status: " + status).c_str());
    u8g2.drawStr(5, 50, ("IP: " + ip).c_str());
    u8g2.drawStr(5, 60, ("RSSI: " + String(rssi) + " dBm").c_str());

    u8g2.sendBuffer();
}

void ResetWifiStatus() {
    prev_ssid = "N/A";
    prev_ip = "N/A";
    prev_status = "N/A";
    prev_rssi = -1;
}

void WifiStatus()
{
    // Get WiFi status
    const String ssid = WiFi.SSID();
    const String ip = WiFi.localIP().toString();
    const String status = (WiFiClass::status() == WL_CONNECTED) ? "Connected" : "Disconnected";
    const int rssi = WiFi.RSSI();

    // Only update the display if the time has changed
    if (ssid != prev_ssid || ip != prev_ip || status != prev_status || rssi != prev_rssi) {
        DrawWifiSettings(ssid, ip, status, rssi);
        prev_ssid = ssid;
        prev_ip = ip;
        prev_status = status;
        prev_rssi = rssi;
    }
}
