#include "display.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void DrawRightAligned(const String& text, const int y, const int right_margin) {
    const int x = u8g2.getDisplayWidth() - right_margin - u8g2.getStrWidth(text.c_str());
    u8g2.drawStr(x, y, text.c_str());
}

void DrawHorizontallyCentered (const String& text, const int y) {
    const int text_width = u8g2.getStrWidth(text.c_str());
    const int x = (u8g2.getDisplayWidth() - text_width) / 2;

    u8g2.drawStr(x, y, text.c_str());
}


void DrawVerticallyCentered(const String& text, const int x) {
    const int ascent = u8g2.getAscent();
    const int descent = u8g2.getDescent();

    const int y =
        (u8g2.getDisplayHeight() + ascent + descent) / 2;

    u8g2.drawStr(x, y, text.c_str());
}