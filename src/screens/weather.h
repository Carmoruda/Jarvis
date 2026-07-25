#pragma once

#include <Arduino.h>

#include "ui/strings.h"
#include "secrets.h"

class Weather {
    public:
        void Update();

    private:
        struct ApiConfig {
            const char* city;
            const char* country_code;
            const char* units;
            const char* api_key;
            const unsigned long call_interval;
        };

        static constexpr ApiConfig kOpenWeatherApi = {
            .city = "Madrid",
            .country_code = "ES",
            .units = "metric",
            .api_key = WEATHER_API_KEY,
            .call_interval = 10UL * 60UL * 1000UL
        };

        void Draw();
        void Fetch();
        bool ParseData(const String& payload);
        const uint8_t* AssignIcon() const;

        // Fetched data
        int condition_id_ = 0;
        String condition_;
        String condition_description_;
        const uint8_t* icon_ = nullptr;
        float temperature_ = 0.0;
        float humidity_ = 0.0;
        float wind_speed_ = 0.0;

        // Fetch state
        unsigned long last_update_ = -kOpenWeatherApi.call_interval;
        String status_ = txt::kGetWeatherInfo;

        // Draw state
        int prev_hour_ = -1;
        int prev_min_ = -1;
        bool changed_ = false;
};

extern Weather weather;