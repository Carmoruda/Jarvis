#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "strings.h"
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
    .icon = "N/A",
    .temperature = -10000,
    .humidity = -10000,
    .pressure = -10000
};

static void DrawWeather() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvR08_tr);

    // Weather City
    u8g2.drawStr(5, 10, kOpenWeather.city);

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

    u8g2.drawStr(95, 10, time_str.c_str());
    u8g2.drawLine(5, 15, 123, 15);

    if (weather_status != txt::kWeatherFetchOkay) {
        u8g2.drawStr((128 - u8g2.getStrWidth(weather_status.c_str())) / 2, 40, weather_status.c_str());
        u8g2.sendBuffer();
        return;
    }

    // Temperature
    u8g2.setFont(u8g2_font_helvB24_tf);

    char temperature_str[12];
    snprintf(temperature_str, sizeof(temperature_str), "%.0fº", WeatherData.temperature);
    u8g2.drawUTF8(5, 50, temperature_str);

    u8g2.sendBuffer();
}

static void FetchWeather() {
    // Display a message while fetching weather info
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvR08_tr);
    u8g2.drawStr((128 - u8g2.getStrWidth(weather_status.c_str())) / 2, 30, weather_status.c_str());
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
        AssignWeatherIcon();
        weather_changed = true;
    }

    http.end();
}

static int ParseWeatherData (const String& payload) {
    JsonDocument doc;
    const DeserializationError error = deserializeJson(doc, payload);

    if (error) {
        u8g2.clearBuffer();
        u8g2.drawStr((128 - u8g2.getStrWidth(txt::kJSONDeserializationError)) / 2, 30, txt::kJSONDeserializationError);
        u8g2.sendBuffer();

        Serial.print("JSON deserialization error: ");
        Serial.println(error.c_str());

        return 0;
    }

    WeatherData.condition_id = doc["weather"][0]["id"].as<int>();
    WeatherData.weather = doc["weather"][0]["main"].as<String>();
    WeatherData.temperature = doc["main"]["temp"].as<float>();
    WeatherData.humidity =  doc["main"]["humidity"].as<float>();
    WeatherData.pressure =  doc["main"]["pressure"].as<float>();

    Serial.println(WeatherData.condition_id);
    Serial.println(WeatherData.weather);
    Serial.println(WeatherData.temperature);
    Serial.println(WeatherData.humidity);
    Serial.println(WeatherData.pressure);

    return 1;
}

static void AssignWeatherIcon() {
    const int id = WeatherData.condition_id;

    if (id >= 200 && id < 300) Serial.println("Thunderstorm");
    else if (id >= 300 && id < 400) Serial.println("Drizzle");
    else if (id >= 500 && id < 600) Serial.println("Rain");
    else if (id >= 600 && id < 700) Serial.println("Snow");
    else if (id == 701) Serial.println("Mist");
    else if (id == 711) Serial.println("Smoke");
    else if (id == 721) Serial.println("Haze");
    else if (id == 731 || id == 761) Serial.println("Dust");
    else if (id == 741) Serial.println("Fog");
    else if (id == 751) Serial.println("Sand");
    else if (id == 762) Serial.println("Ash");
    else if (id == 771) Serial.println("Squall");
    else if (id == 781) Serial.println("Tornado");
    else if (id == 800) Serial.println("Clear");
    else if (id > 800 && id < 900) Serial.println("Clouds");
    else Serial.println("Error");
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
