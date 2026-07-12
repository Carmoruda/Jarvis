#include <WiFi.h>
#include "hardware/display.h"

void drawWiFiSettings() {
    String ssid = WiFi.SSID();
    String ip = WiFi.localIP().toString();

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_bitcasual_tr);
    u8g2.drawStr(10, 20, "WiFi Settings");
    u8g2.drawStr(10, 40, ("SSID: " + ssid).c_str());
    u8g2.drawStr(10, 60, ("IP: " + ip).c_str());
    u8g2.sendBuffer();
}
