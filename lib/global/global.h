#ifndef global_h
#define global_h

//LIBRARY INCLUDE
#include <Arduino.h>
#include <sensor_module.h>

#define WIFI_SSID "Duy Bach"
#define WIFI_PASSWORD "28082004"
#define DATABASE_URL "https://test-9c8d6-default-rtdb.asia-southeast1.firebasedatabase.app/" 
#define API_KEY "AIzaSyDq552RtmjLM8ok1NhIKufMbw3_c73sppk"
//DEFINE GPIO PIN
#define SCL_Pin 22
#define SDA_Pin 21
#define dht_signal_Pin 23
#define mq135_analog_Pin 35
//VARIABLE

extern float homeTemperature;
extern float homeHumidity;
extern float homeLightlevel;
extern float homeAirlevel;

// RETURN FUNCTION
float getHomeTemperature();
float getHomeHumidity();
float getHomeLightlevel();
float getHomeAirlevel();

// SETUP FUNCTION
void setupWifi();
void homeUpdateDataTask(void *parameters);
void setupHomeUpdateData();
void databaseTask(void *parameters);
void setupDatabase();

#endif