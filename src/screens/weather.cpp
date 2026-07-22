#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "ui/strings.h"
#include "ui/icons.h"
#include "screens/weather.h"
#include "hardware/display.h"

static HTTPClient http;
static String url = "https://api.openweathermap.org/data/2.5/weather?q=" + String(kOpenWeather.city) + "," +
                        String(kOpenWeather.country_code) + "&units=" + String(kOpenWeather.units) + "&appid=" +
                        String(kOpenWeather.api_key);

static int prev_hour = -1;
static int prev_min = -1;
static bool weather_changed = false;
static String weather_status = txt::kGetWeatherInfo;

static unsigned long last_weather_update = -kOpenWeather.call_interval;

Weather WeatherData = {
    .condition_id = -1,
    .weather = "N/A",
    .description = "N/A",
    .icon = nullptr,
    .temperature = -10000,
    .humidity = -10000,
    .wind_speed = -10000
};

static void DrawWeather() {
    u8g2.clearBuffer();
    u8g2.drawLine(5, 15, 123, 15);
    u8g2.setFont(u8g2_font_helvR08_tf);

    // Weather City
    u8g2.drawUTF8(5, 10, kOpenWeather.city);

    // Clock
    struct tm time_info{};
    getLocalTime(&time_info);

    char hour[3], min[3];   // 2 digits + null terminator
    snprintf(hour, sizeof(hour), "%02d", time_info.tm_hour);
    snprintf(min, sizeof(min), "%02d", time_info.tm_min);

    // Only update the display if the time has changed
    if (time_info.tm_min == prev_min && !weather_changed) {
        return;
    }

    prev_hour = time_info.tm_hour;
    prev_min = time_info.tm_min;

    const String time_str = String(hour) + ":" + String(min);
    DrawRightAligned(time_str.c_str(), 10, 5);

    if (weather_status != txt::kWeatherFetchOkay) {
        DrawHorizontallyCentered(weather_status, 40);
        u8g2.sendBuffer();
        return;
    }

    // Temperature
    u8g2.setFont(u8g2_font_helvB24_tf);

    char temperature_str[12];
    snprintf(temperature_str, sizeof(temperature_str), "%.0f°", WeatherData.temperature);
    u8g2.drawUTF8(5, 47, temperature_str);

    // Condition Icon
    u8g2.drawBitmap(99, 22, icons::kIconWidth / 8, icons::kIconHeight, WeatherData.icon);

    // Condition text
    u8g2.setFont(u8g2_font_helvR08_tf);
    DrawRightAligned(WeatherData.description, 62, 5);

    // Humidity
    char humidity_str[12];
    snprintf(humidity_str, sizeof(humidity_str), "%.0f%%", WeatherData.humidity);
    u8g2.drawXBMP(5, 54, icons::kIconSmallWidth, icons::kIconSmallHeight, icons::kIconHumidity);
    u8g2.drawStr(17, 62, humidity_str);

    // Wind Speed
    // char wind_speed_str[12];
    // snprintf(wind_speed_str, sizeof(wind_speed_str), "%.0fkm", WeatherData.wind_speed);
    // u8g2.drawXBMP(45, 54, icons::kIconSmallWidth, icons::kIconSmallHeight, icons::kIconWindSpeed);
    // u8g2.drawStr(55, 62, wind_speed_str);

    u8g2.sendBuffer();

}

static void FetchWeather() {
    // Display a message while fetching weather info
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvR08_tr);
    DrawHorizontallyCentered(weather_status, 30);
    u8g2.sendBuffer();

    if (!http.begin(url)) {
        weather_status = txt::kApiCallFailed;
        return;
    }

    const int code = http.GET();

    if (code != 200) {
        weather_status = txt::kHTTPError;
        http.end();
        return;
    }

    if (ParseWeatherData(http.getString())) {
        weather_status = txt::kWeatherFetchOkay;
        WeatherData.icon = AssignWeatherIcon();
        weather_changed = true;
    }

    http.end();
}

static int ParseWeatherData (const String& payload) {
    JsonDocument doc;
    const DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        u8g2.clearBuffer();
        DrawHorizontallyCentered(txt::kJSONDeserializationError,30);
        u8g2.sendBuffer();
        return 0;
    }

    WeatherData.condition_id = doc["weather"][0]["id"].as<int>();
    WeatherData.weather = doc["weather"][0]["main"].as<String>();
    WeatherData.description = doc["weather"][0]["description"].as<String>();
    WeatherData.description[0] = toupper(WeatherData.description[0]);
    WeatherData.temperature = doc["main"]["temp"].as<float>();
    WeatherData.humidity =  doc["main"]["humidity"].as<float>();
    WeatherData.wind_speed =  doc["wind"]["speed"].as<float>();

    return 1;
}

static const uint8_t* AssignWeatherIcon() {
    const int id = WeatherData.condition_id;

    if (id >= 200 && id < 300) return icons::kIconThunderstorm;
    if (id >= 300 && id < 400) return icons::kIconDrizzle;
    if (id >= 500 && id < 510) return icons::kIconRain;
    if (id == 511) return icons::kIconSnow;
    if (id >= 600 && id < 700) return icons::kIconSnow;
    if (id >= 700 && id < 800) return icons::kIconAtmosphere;
    if (id == 800) return icons::kIconClear;
    if (id == 801) return icons::kIconFewClouds;
    if (id == 802) return icons::kIconScatteredClouds;
    if (id > 802 && id < 900) return icons::kIconClouds;

    return nullptr;
}

void UpdateWeather() {
    const unsigned long now = millis();

    if (now - last_weather_update >= kOpenWeather.call_interval) {
        last_weather_update = now;
        weather_changed = true;
        FetchWeather();
    }

    DrawWeather();
}
