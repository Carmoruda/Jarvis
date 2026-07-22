// Copyright (c) 2026 Carmoruda. MIT License. See LICENSE for details.
// OLED display configuration and drawing utilities.
//
// Declares the shared U8g2 display instance used to render the application
// screens. Also provides helpers for drawing horizontally or vertically
// aligned text. The display uses an SSD1306 controller, a 128x64 resolution,
// full-buffer rendering, and the hardware I2C interface.

#pragma once

#include <U8g2lib.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

void DrawRightAligned(const String& text, int y, int right_margin = 5);
void DrawHorizontallyCentered(const String& text, int y);
void DrawVerticallyCentered(const String& text, int x);