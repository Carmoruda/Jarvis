#pragma once

#include <Arduino.h>

extern const char *time_zone;

void SyncTime();
void ClockSetup();
void DrawClock(const String& hour_str, const String& min_str);
void ResetPrevTime();
