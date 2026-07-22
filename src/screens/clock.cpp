#include "clock.h"
#include "ui/strings.h"
#include "screens/wifi_settings.h"
#include "hardware/display.h"

// Time zone configuration
const char *time_zone = "CET-1CEST,M3.5.0,M10.5.0/3"; // Central European Time (CET) with daylight saving time
static int prev_hour = -1;
static int prev_min = -1;

static void DrawClock(const String& hour_str, const String& min_str) {

    // Clear the buffer and set the font
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvB24_tf);

    const String time_str = hour_str + ":" + min_str;

    // Draw the time in the center of the screen
    DrawHorizontallyCentered(time_str, (64 + u8g2.getMaxCharHeight()) / 2);

    u8g2.sendBuffer();
}

void SyncTime()
{
    // Display a message while syncing time
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvR08_tr);
    DrawHorizontallyCentered(txt::kSyncingTime, 30);
    u8g2.sendBuffer();

    // Configure time using NTP
    configTzTime(time_zone, kWifiConfig.ntpServer);

    delay(1000);
}

void ResetPrevTime() {
    prev_hour = -1;
    prev_min = -1;
}

void ClockSetup()
{
    // Get the current time
    struct tm time_info{};
    getLocalTime(&time_info);

    char hour[3], min[3];   // 2 digits + null terminator
    snprintf(hour, sizeof(hour), "%02d", time_info.tm_hour);
    snprintf(min, sizeof(min), "%02d", time_info.tm_min);

    // Only update the display if the time has changed
    if (time_info.tm_hour != prev_hour || time_info.tm_min != prev_min) {
        DrawClock(String(hour), String(min));
        prev_hour = time_info.tm_hour;
        prev_min = time_info.tm_min;
    }
}
