#include "clock.h"

#include "ui/strings.h"
#include "screens/wifi.h"
#include "hardware/display.h"

Clock time_clock;

void Clock::Sync() {
    // Display a message while syncing time
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvR08_tr);
    DrawHorizontallyCentered(txt::kSyncingTime, 30);
    u8g2.sendBuffer();

    // Configure time using NTP
    configTzTime(kTimeZone, wifi.GetNtpServer());

    // Reset hour
    prev_hour_ = -1;
    prev_min_ = -1;

    delay(1000);
}

void Clock::Draw(const String &hour_str, const String &min_str, const String &date) {
    u8g2.clearBuffer();
    u8g2.drawLine(5, 15, 123, 15);

    // Date
    u8g2.setFont(u8g2_font_helvR08_tr);
    DrawHorizontallyCentered(date, 10);

    // Time
    u8g2.setFont(u8g2_font_helvR24_tf);
    const String time_str = hour_str + ":" + min_str;
    DrawHorizontallyCentered(time_str, ((70 + u8g2.getMaxCharHeight())) / 2);

    u8g2.sendBuffer();
}

void Clock::Reset() {
    prev_hour_ = -1;
    prev_min_ = -1;
}

void Clock::Update() {
    // Get the current time
    struct tm time_info{};
    if (!getLocalTime(&time_info)) return;

    // Only redraw when the minute changed
    if (time_info.tm_hour == prev_hour_ && time_info.tm_min == prev_min_) return;
    prev_hour_ = time_info.tm_hour;
    prev_min_ = time_info.tm_min;

    char hour[3], min[3];
    snprintf(hour, sizeof(hour), "%02d", time_info.tm_hour);
    snprintf(min, sizeof(min), "%02d", time_info.tm_min);

    const String date = String(txt::kWeekdayNames[time_info.tm_wday]) + ", "
                            + txt::kMonthNames[time_info.tm_mon]
                            + " " + String(time_info.tm_mday);

    Draw(String(hour), String(min), date);

}