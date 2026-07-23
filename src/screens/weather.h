#pragma once

#include "secrets.h"

struct Weather {
    int condition_id;
    String weather;
    String description;
    const uint8_t* icon;
    float temperature;
    float humidity;
    float wind_speed;
};

struct WeatherApiConfig {
    const char* city;
    const char* country_code;
    const char* units;
    const char* api_key;
    const unsigned long call_interval;
};

extern Weather WeatherData;

constexpr WeatherApiConfig kOpenWeather = {
    .city = "Madrid",
    .country_code = "ES",
    .units = "metric",
    .api_key = WEATHER_API_KEY,
    .call_interval = 10UL * 60UL * 1000UL
};

static void DrawWeather();
static void FetchWeather();
static int ParseWeatherData (const String& payload);
static const uint8_t* AssignWeatherIcon();
void UpdateWeather();
