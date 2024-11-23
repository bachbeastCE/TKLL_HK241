#ifndef global_h
#define global_h

//LIBRARY INCLUDE
#include <Arduino.h>
#include <sensor_module.h>
#include <button.h>
#include <lcd.h>

#define WIFI_SSID "Duy Bach"
#define WIFI_PASSWORD "28082004"
#define DATABASE_URL "https://test-9c8d6-default-rtdb.asia-southeast1.firebasedatabase.app/" 
#define API_KEY "AIzaSyDq552RtmjLM8ok1NhIKufMbw3_c73sppk"
//DEFINE GPIO PIN
#define SCL_Pin 22
#define SDA_Pin 21
#define dht_signal_Pin 23
#define mq135_analog_Pin 35

#define ON 1
#define OFF 0

#define MAX_RELAY 4
#define relay1 25
#define relay2 26
#define relay3 27
#define relay4 14

#define MAX_BUTTON 4
#define button1 17
#define button2 5
#define button3 18
#define button4 19


//VARIABLE


extern float homeTemperature;
extern float homeHumidity;
extern float homeLightlevel;
extern float homeAirlevel;
extern uint8_t relayStatus[MAX_RELAY];


// RETURN FUNCTION
float getHomeTemperature();
float getHomeHumidity();
float getHomeLightlevel();
float getHomeAirlevel();


void toggleRelay(int relay);



// SETUP FUNCTION
void setupRelay();
void setupButton();
void setupWifi();
void homeUpdateDataTask(void *parameters);
void setupHomeUpdateData();
void databaseTask(void *parameters);
void setupDatabase();
void readButtonTask(void *parameters);
void setupReadButton();
void updateRelayTask(void *parameters);
void setupuUpdateRelay();
#endif