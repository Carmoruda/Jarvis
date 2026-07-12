#include "clock.h"
#include <Arduino.h>
#include "hardware/display.h"

// Time zone configuration
const char *time_zone = "CET-1CEST,M3.5.0,M10.5.0/3"; // Central European Time (CET) with daylight saving time
static int prevHour = -1;
static int prevMin = -1;

void drawClock(String hourStr, String minStr) {

    // Clear the buffer and set the font
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_freedoomr25_tn);

    String timeStr = hourStr + ":" + minStr;

    // Draw the time in the center of the screen
    u8g2.setCursor((128 - u8g2.getStrWidth(timeStr.c_str())) / 2, 50);
    u8g2.print(hourStr);
    u8g2.print(":");
    u8g2.print(minStr);

    u8g2.sendBuffer();
}

void resetPrevTime() {
    prevHour = -1;
    prevMin = -1;
}

void clockSetup()
{
    // Get the current time
    struct tm time_info;
    getLocalTime(&time_info);

    char hour[3], min[3];   // 2 digits + null terminator
    snprintf(hour, sizeof(hour), "%02d", time_info.tm_hour);
    snprintf(min, sizeof(min), "%02d", time_info.tm_min);

    // Only update the display if the time has changed
    if (time_info.tm_hour != prevHour || time_info.tm_min != prevMin) {
        drawClock(String(hour), String(min));
        prevHour = time_info.tm_hour;
        prevMin = time_info.tm_min;
    }
}
