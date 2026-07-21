// Copyright (c) 2026 Carmoruda. MIT License. See LICENSE for details.
// Clock synchronization and display screen.
//
// Manages NTP time synchronization, local time retrieval, and rendering
// the clock screen. Time zone and NTP settings come from the configured
// Wi-Fi settings. This file owns clock rendering and update tracking only.

#pragma once

#include <Arduino.h>

extern const char *time_zone;

void SyncTime();
void ClockSetup();
void DrawClock(const String& hour_str, const String& min_str);
void ResetPrevTime();
