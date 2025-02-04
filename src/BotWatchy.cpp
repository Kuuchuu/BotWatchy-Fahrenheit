#include "BotWatchy.h"

const int posHeart0X = 10;
const int posHeart0Y = 10;
const int posHeart1X = 40;
const int posHeart1Y = 10;
const int posHeart2X = 69;
const int posHeart2Y = 10;
const int posTemperatureX = 138;
const int posTemperatureY = 68;
const int posTriforceX = 8;
const int posTriforceY = 163;
const int posWeatherBaseX = 40;
const int posWeatherBaseY = 151;
const int posWeather0X = 55;
const int posWeather0Y = 158;
const int posWeather1X = 103;
const int posWeather1Y = 158;
const int posWeather2X = 151;
const int posWeather2Y = 158;
const int posWifiX = 138;
const int posWifiY = 10;
const int posStepsX = 10;
const int posStepsY = 38;

const float VOLTAGE_MIN = 3.2;
const float VOLTAGE_MAX = 4.1;
const float VOLTAGE_RANGE = 0.9;

RTC_DATA_ATTR int weatherIntervalCounterOneCall = WEATHER_UPDATE_INTERVAL;
RTC_DATA_ATTR weatherDataOneCall currentWeatherOneCall;

BotWatchy::BotWatchy()
{
  // Serial.begin(115200);
}

void BotWatchy::drawWatchFace()
{
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);

  display.drawBitmap(0, 0, epd_bitmap_Border, 200, 200, GxEPD_BLACK);

  display.drawBitmap(posTriforceX, posTriforceY, epd_bitmap_triforce, 33, 28, GxEPD_BLACK);

  drawTime();
  drawDate();
  drawWeather();
  drawSteps();
  drawBattery();
  drawWifi();
}

void BotWatchy::drawTime()
{
  display.setFont(&Calamity_Bold18pt7b);
  display.setCursor(12, 140);
  if (currentTime.Hour < 10)
  {
    display.print("0");
    display.print(currentTime.Hour);
    display.print(":");
    if (currentTime.Minute < 10)
      display.print("0");
    display.println(currentTime.Minute);
    display.setFont(&Calamity_Bold8pt7b);
    display.setCursor(122, 140);
    display.print("AM");
  }
  else if (currentTime.Hour < 13)
  {
    display.print(currentTime.Hour);
    display.print(":");
    if (currentTime.Minute < 10)
      display.print("0");
    display.println(currentTime.Minute);
    display.setFont(&Calamity_Bold8pt7b);
    display.setCursor(122, 140);
    display.print("AM");
  }
  else if (currentTime.Hour < 22)
  {
    display.print("0");
    display.print(currentTime.Hour - 12);
    display.print(":");
    if (currentTime.Minute < 10)
      display.print("0");
    display.println(currentTime.Minute);
    display.setFont(&Calamity_Bold8pt7b);
    display.setCursor(122, 140);
    display.print("PM");
  }
  else
  {
    display.print(currentTime.Hour - 12);
    display.print(":");
    if (currentTime.Minute < 10)
      display.print("0");
    display.println(currentTime.Minute);
    display.setFont(&Calamity_Bold8pt7b);
    display.setCursor(122, 140);
    display.print("PM");
  }
}

void BotWatchy::drawDate()
{
  display.setFont(&Calamity_Bold8pt7b);

  String dayOfWeek = dayStr(currentTime.Wday);
  String month = monthStr(currentTime.Month);

  display.setCursor(12, 78);
  display.println(dayOfWeek);

  display.setCursor(12, 97);
  display.print(month);
  display.print(" ");

  display.print(currentTime.Day);
  if (currentTime.Day == 1)
    display.print("st");
  else if (currentTime.Day == 2)
    display.print("nd");
  else if (currentTime.Day == 3)
    display.print("rd");
  else
    display.print("th");
}

void BotWatchy::drawSteps(){
    uint32_t stepCount = sensor.getCounter();
    display.drawBitmap(posStepsX, posStepsY, epd_bitmap_steps, 19, 19, GxEPD_BLACK);
    display.setFont(&Calamity_Bold8pt7b);
    display.setCursor(posStepsX + 20, posStepsY + 14);
    display.println(stepCount);
}

void BotWatchy::drawBattery()
{
  float VBAT = getBatteryVoltage();

  // 12 battery states
  int batState = int(((VBAT - VOLTAGE_MIN) / VOLTAGE_RANGE) * 12);
  if (batState > 12)
    batState = 12;
  if (batState < 0)
    batState = 0;

  // display.setCursor(10, 50);
  // display.print(batState);

  if (batState == 12)
  {
    display.drawBitmap(posHeart0X, posHeart0Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart1X, posHeart1Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart2X, posHeart2Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
  }
  else if (batState == 11)
  {
    display.drawBitmap(posHeart0X, posHeart0Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart1X, posHeart1Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart2X, posHeart2Y, epd_bitmap_heart_threequarters, 27, 22, GxEPD_BLACK);
  }
  else if (batState == 10)
  {
    display.drawBitmap(posHeart0X, posHeart0Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart1X, posHeart1Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart2X, posHeart2Y, epd_bitmap_heart_half, 27, 22, GxEPD_BLACK);
  }
  else if (batState == 9)
  {
    display.drawBitmap(posHeart0X, posHeart0Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart1X, posHeart1Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart2X, posHeart2Y, epd_bitmap_heart_quarter, 27, 22, GxEPD_BLACK);
  }
  else if (batState == 8)
  {
    display.drawBitmap(posHeart0X, posHeart0Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart1X, posHeart1Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart2X, posHeart2Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
  }
  else if (batState == 7)
  {
    display.drawBitmap(posHeart0X, posHeart0Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart1X, posHeart1Y, epd_bitmap_heart_threequarters, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart2X, posHeart2Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
  }
  else if (batState == 6)
  {
    display.drawBitmap(posHeart0X, posHeart0Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart1X, posHeart1Y, epd_bitmap_heart_half, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart2X, posHeart2Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
  }
  else if (batState == 5)
  {
    display.drawBitmap(posHeart0X, posHeart0Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart1X, posHeart1Y, epd_bitmap_heart_quarter, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart2X, posHeart2Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
  }
  else if (batState == 4)
  {
    display.drawBitmap(posHeart0X, posHeart0Y, epd_bitmap_heart_full, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart1X, posHeart1Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart2X, posHeart2Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
  }
  else if (batState == 3)
  {
    display.drawBitmap(posHeart0X, posHeart0Y, epd_bitmap_heart_threequarters, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart1X, posHeart1Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart2X, posHeart2Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
  }
  else if (batState == 2)
  {
    display.drawBitmap(posHeart0X, posHeart0Y, epd_bitmap_heart_half, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart1X, posHeart1Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart2X, posHeart2Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
  }
  else if (batState == 1)
  {
    display.drawBitmap(posHeart0X, posHeart0Y, epd_bitmap_heart_quarter, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart1X, posHeart1Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart2X, posHeart2Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
  }
  else if (batState == 0)
  {
    display.drawBitmap(posHeart0X, posHeart0Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart1X, posHeart1Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
    display.drawBitmap(posHeart2X, posHeart2Y, epd_bitmap_heart_empty, 27, 22, GxEPD_BLACK);
  }
}

void BotWatchy::drawWeather()
{
  weatherDataOneCall currentWeatherOneCall = getWeatherData();

  display.drawBitmap(posWeatherBaseX, posWeatherBaseY, epd_bitmap_weather_base, 150, 40, GxEPD_BLACK);
  display.drawBitmap(posTemperatureX, posTemperatureY, epd_bitmap_temperature_base, 50, 50, GxEPD_BLACK);

  drawWeatherIcon(0, currentWeatherOneCall.weatherConditionCode0);
  drawWeatherIcon(1, currentWeatherOneCall.weatherConditionCode1);
  drawWeatherIcon(2, currentWeatherOneCall.weatherConditionCode2);

  // temperature
  int temperature = currentWeatherOneCall.temperature;

  int l = 16;
  int minTemp = -12;
  int maxTemp = 38;

  int scalingForMap = 10000;
  float threeQuarterPi = 4.7123;
  int scaledThreeQuarterPi = threeQuarterPi * scalingForMap;

  int scaledAngle = map(temperature, minTemp, maxTemp, 0, scaledThreeQuarterPi);
  float angle = scaledAngle / float(scalingForMap);
  if (angle > threeQuarterPi)
    angle = threeQuarterPi;
  if (angle < 0)
    angle = 0;
  angle += 2.3561;

  int startX = posTemperatureX + 25;
  int startY = posTemperatureY + 25;
  int endX = int(cos(angle) * l) + startX;
  int endY = int(sin(angle) * l) + startY;

  // draw line three times so no matter the pointer orientation,
  // the pointer is about two pixels wide
  display.drawLine(startX, startY, endX, endY, GxEPD_WHITE);
  display.drawLine(startX + 1, startY, endX + 1, endY, GxEPD_WHITE);
  display.drawLine(startX, startY + 1, endX, endY + 1, GxEPD_WHITE);
}

void BotWatchy::drawWeatherIcon(int8_t iconPosX, int16_t iconWeatherConditionCode)
{
  int iconPosPxX = 0;
  if (iconPosX == 0)
    iconPosPxX = posWeather0X;
  if (iconPosX == 1)
    iconPosPxX = posWeather1X;
  if (iconPosX == 2)
    iconPosPxX = posWeather2X;

  //https://openweathermap.org/weather-conditions
  if (iconWeatherConditionCode > 801) //Cloudy
    display.drawBitmap(iconPosPxX, posWeather0Y, epd_bitmap_weather_clouds, 27, 27, GxEPD_WHITE);
  else if (iconWeatherConditionCode == 801) //Few Clouds
    display.drawBitmap(iconPosPxX, posWeather0Y, epd_bitmap_weather_partlycloudy, 27, 27, GxEPD_WHITE);
  else if (iconWeatherConditionCode == 800) //Clear
    display.drawBitmap(iconPosPxX, posWeather0Y, epd_bitmap_weather_sun, 27, 27, GxEPD_WHITE);
  else if (iconWeatherConditionCode >= 700) //Atmosphere
    display.drawBitmap(iconPosPxX, posWeather0Y, epd_bitmap_weather_clouds, 27, 27, GxEPD_WHITE);
  else if (iconWeatherConditionCode >= 600) //Snow
    display.drawBitmap(iconPosPxX, posWeather0Y, epd_bitmap_weather_snow, 27, 27, GxEPD_WHITE);
  else if (iconWeatherConditionCode >= 500) //Rain
    display.drawBitmap(iconPosPxX, posWeather0Y, epd_bitmap_weather_rain, 27, 27, GxEPD_WHITE);
  else if (iconWeatherConditionCode >= 300) //Drizzle
    display.drawBitmap(iconPosPxX, posWeather0Y, epd_bitmap_weather_rain, 27, 27, GxEPD_WHITE);
  else if (iconWeatherConditionCode >= 200) //Thunderstorm
    display.drawBitmap(iconPosPxX, posWeather0Y, epd_bitmap_weather_flash, 27, 27, GxEPD_WHITE);
  else
    return;
}

void BotWatchy::drawWifi()
{
  display.drawBitmap(posWifiX, posWifiY, WIFI_CONFIGURED ? epd_bitmap_wifi_on : epd_bitmap_wifi_off, 50, 50, GxEPD_BLACK);
}

weatherDataOneCall BotWatchy::getWeatherData()
{
  if (weatherIntervalCounterOneCall >= WEATHER_UPDATE_INTERVAL)
  { //only update if WEATHER_UPDATE_INTERVAL has elapsed i.e. 30 minutes
    if (connectWiFi())
    { //Use Weather API for live data if WiFi is connected
      HTTPClient http;
      http.setConnectTimeout(3000); //3 second max timeout
      String weatherQueryURL = String("https://api.openweathermap.org/data/2.5/onecall?lat=") + String(LAT) + String("&lon=") + String(LON) + String("&exclude=minutely,hourly,alerts&units=metric&appid=") + String(OWN_API_KEY);
      http.begin(weatherQueryURL.c_str());
      int httpResponseCode = http.GET();
      if (httpResponseCode == 200)
      {
        String payload = http.getString();
        JSONVar responseObject = JSON.parse(payload);
        currentWeatherOneCall.invalid = false;
        currentWeatherOneCall.temperature = int(responseObject["current"]["temp"]);
        currentWeatherOneCall.weatherConditionCode0 = int(responseObject["current"]["weather"][0]["id"]);
        currentWeatherOneCall.weatherConditionCode1 = int(responseObject["daily"][1]["weather"][0]["id"]);
        currentWeatherOneCall.weatherConditionCode2 = int(responseObject["daily"][2]["weather"][0]["id"]);
      }
      else
      {
        currentWeatherOneCall.invalid = true;
      }
      http.end();
      //turn off radios
      WiFi.mode(WIFI_OFF);
      btStop();
    }
    else
    { //No WiFi, use RTC Temperature
      currentWeatherOneCall.invalid = true;
    }
    weatherIntervalCounterOneCall = 0;
  }
  else
  {
    weatherIntervalCounterOneCall++;
  }
  return currentWeatherOneCall;
}
