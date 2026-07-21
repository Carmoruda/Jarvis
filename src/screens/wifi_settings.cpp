#include <WiFi.h>
#include <strings.h>
#include "secrets.h"
#include "hardware/display.h"
#include "screens/wifi_settings.h"

WiFiConfig WIFI_CONFIG = {WIFI_SSID, WIFI_PASS, "pool.ntp.org"};

// WiFi status variables to track changes
static String prevSsid = "N/A";
static String prevIp = "N/A";
static String prevStatus = "N/A";
static int prevRssi = -1;

void connectWiFi()
{
    // Display a message while connecting to WiFi
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont11_tr);
    u8g2.drawStr((128 - u8g2.getStrWidth(txt::WIFI_CONNECTING)) / 2, 30, txt::WIFI_CONNECTING);
    u8g2.sendBuffer();

    // Connect to WiFi
    WiFi.begin(WIFI_CONFIG.ssid, WIFI_CONFIG.password);

    uint8_t retry = 0;

    // Wait for connection with a timeout of 30 seconds (100 * 300ms)
    while (WiFi.status() != WL_CONNECTED && retry < 100) {
        delay(300);
        retry++;
    }

    // Display connection result
    if (WiFi.status() == WL_CONNECTED) {

        u8g2.clearBuffer();
        u8g2.drawStr((128 - u8g2.getStrWidth(txt::WIFI_CONNECTED)) / 2, 30, txt::WIFI_CONNECTED);
        u8g2.sendBuffer();

        delay(500);

    } else {
        u8g2.clearBuffer();
        u8g2.drawStr((128 - u8g2.getStrWidth(txt::WIFI_FAILED)) / 2, 30, txt::WIFI_FAILED);
        u8g2.sendBuffer();

        delay(1500);
    }
}

void drawWiFiSettings(String ssid, String ip, String status, int rssi) {
    u8g2.clearBuffer();

    u8g2.drawLine(5, 15, 123, 15);

    // Screen title
    u8g2.setFont(u8g2_font_profont11_tr);
    u8g2.drawStr((128 - u8g2.getStrWidth(txt::WIFI_SETTINGS_TITLE)) / 2, 10, txt::WIFI_SETTINGS_TITLE);

    // WiFi settings
    u8g2.setFont(u8g2_font_profont11_tr);
    u8g2.drawStr(5, 40, ("Status: " + status).c_str());
    u8g2.drawStr(5, 30, ("SSID: " + ssid).c_str());
    u8g2.drawStr(5, 50, ("IP: " + ip).c_str());
    u8g2.drawStr(5, 60, ("RSSI: " + String(rssi) + " dBm").c_str());

    u8g2.sendBuffer();
}

void resetWiFiStatus() {
    prevSsid = "N/A";
    prevIp = "N/A";
    prevStatus = "N/A";
    prevRssi = -1;
}

void WiFiStatus()
{
    // Get WiFi status
    String ssid = WiFi.SSID();
    String ip = WiFi.localIP().toString();
    String status = (WiFi.status() == WL_CONNECTED) ? "Connected" : "Disconnected";
    int rssi = WiFi.RSSI();

    // Only update the display if the time has changed
    if (ssid != prevSsid || ip != prevIp || status != prevStatus || rssi != prevRssi) {
        drawWiFiSettings(ssid, ip, status, rssi);
        prevSsid = ssid;
        prevIp = ip;
        prevStatus = status;
        prevRssi = rssi;
    }
}
