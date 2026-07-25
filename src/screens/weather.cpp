#include "screens/weather.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "hardware/display.h"
#include "ui/strings.h"
#include "ui/icons.h"

Weather weather;

void Weather::Fetch() {
    const String url = "https://api.openweathermap.org/data/2.5/weather?q="
                 + String(kOpenWeatherApi.city) + "," + String(kOpenWeatherApi.country_code)
                    + "&units=" + String(kOpenWeatherApi.units) + "&appid=" + String(kOpenWeatherApi.api_key);

    HTTPClient http;

    // Display a message while fetching weather info
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvR08_tr);
    DrawHorizontallyCentered(status_, 30);
    u8g2.sendBuffer();

    if (!http.begin(url)) {
        status_ = txt::kApiCallFailed;
        return;
    }

    const int code = http.GET();
    if (code != 200) {
        status_ = txt::kHTTPError;
        http.end();
        return;
    }

    const String payload  = http.getString();
    if (ParseData(payload)) {
        icon_ = AssignIcon();
        status_ = txt::kWeatherFetchOkay;
        changed_ = true;
    }
}

bool Weather::ParseData(const String& payload) {
    JsonDocument doc;
    const DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        status_ = txt::kJSONDeserializationError;
        return false;
    }

    condition_id_               = doc["weather"][0]["id"].as<int>();
    condition_                  = doc["weather"][0]["main"].as<String>();
    condition_description_      = doc["weather"][0]["description"].as<String>();
    condition_description_[0]   = toupper(condition_description_[0]);
    temperature_                = doc["main"]["temp"].as<float>();
    humidity_                   =  doc["main"]["humidity"].as<float>();
    wind_speed_                 =  doc["wind"]["speed"].as<float>();

    return true;
}

const uint8_t* Weather::AssignIcon() const {
    const int id = condition_id_;

    if (id >= 200 && id < 300)  return icons::kIconThunderstorm;
    if (id >= 300 && id < 400)  return icons::kIconDrizzle;
    if (id >= 500 && id < 510)  return icons::kIconRain;
    if (id == 511)              return icons::kIconSnow;
    if (id >= 600 && id < 700)  return icons::kIconSnow;
    if (id >= 700 && id < 800)  return icons::kIconAtmosphere;
    if (id == 800)              return icons::kIconClear;
    if (id == 801)              return icons::kIconFewClouds;
    if (id == 802)              return icons::kIconScatteredClouds;
    if (id > 802 && id < 900)   return icons::kIconClouds;

    return nullptr;
}

void Weather::Draw() {
    struct tm time_info{};
    const bool has_time = getLocalTime(&time_info);

    const bool time_changed = has_time && (time_info.tm_hour != prev_hour_ || time_info.tm_min != prev_min_);
    if (!time_changed && !changed_) return;

    if (has_time) {
        prev_hour_ = time_info.tm_hour;
        prev_min_ = time_info.tm_min;
    }

    changed_ = false;

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvR08_tf);

    // City + time header
    u8g2.drawUTF8(5, 10, kOpenWeatherApi.city);

    if (has_time) {
        char time_str[6];
        snprintf(time_str, sizeof(time_str), "%02d:%02d", time_info.tm_hour, time_info.tm_min);
        DrawRightAligned(time_str, 10, 5);
    }

    u8g2.drawLine(5, 15, 123, 15);

    if (status_ != txt::kWeatherFetchOkay) {
        DrawHorizontallyCentered(status_, 40);
        u8g2.sendBuffer();
        return;
    }

    // Temperature (left)
    char temp_str[12];
    snprintf(temp_str, sizeof(temp_str), "%.0f°", temperature_);
    u8g2.setFont(u8g2_font_helvB24_tf);
    u8g2.drawUTF8(5, 47, temp_str);

    // Weather icon and description (right)
    u8g2.setFont(u8g2_font_helvR08_tf);
    if (icon_ != nullptr) {
        u8g2.drawBitmap(99, 22, icons::kIconWidth / 8, icons::kIconHeight, icons::kIconThunderstorm);
        DrawRightAligned(condition_description_, 62, 5);
    }

    // Humidity (lower left)
    char humidity_str[12];
    snprintf(humidity_str, sizeof(humidity_str), "%.0f%%", humidity_);
    u8g2.drawXBMP(5, 54, icons::kIconSmallWidth, icons::kIconSmallHeight, icons::kIconHumidity);
    u8g2.drawStr(17, 62, humidity_str);

    // Wind Speed
    // char wind_speed_str[12];
    // snprintf(wind_speed_str, sizeof(wind_speed_str), "%.0fkm", wind_speed_);
    // u8g2.drawXBMP(45, 54, icons::kIconSmallWidth, icons::kIconSmallHeight, icons::kIconWindSpeed);
    // u8g2.drawStr(55, 62, wind_speed_str);

    u8g2.setFont(u8g2_font_helvR08_tf);
}

void Weather::Update() {
    const unsigned long now = millis();

    if (now - last_update_ >= kOpenWeatherApi.call_interval) {
        last_update_ = now;
        Fetch();
    }

    Draw();
}

void Weather::ResetView() {
    prev_hour_ = -1;
    prev_min_ = -1;
    changed_ = true;
}
