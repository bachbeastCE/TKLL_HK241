#ifndef global_h
#define global_h




//LIBRARY INCLUDE
#include <Arduino.h>
#include <sensor_module.h>
#include <button.h>
#include <lcd.h>
#include <relay.h>




#define WIFI_SSID "Duy Bach"
#define WIFI_PASSWORD "28082004"
#define DATABASE_URL "https://test-9c8d6-default-rtdb.asia-southeast1.firebasedatabase.app/" 
#define API_KEY "AIzaSyDq552RtmjLM8ok1NhIKufMbw3_c73sppk"

//DEFINE GPIO PIN
#define SCL_Pin 22
#define SDA_Pin 21
#define dht_signal_Pin 23
#define mq135_analog_Pin 35
#define fan_pin 33
#define fire_detect_Pin 32
#define IR_SEND_PIN 4  // Ví dụ: GPIO 4

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


// KEY-CODE TABLE FOR 17-KEY IR REMOTE CONTROLLER:
#define KEY17_1     69  //  1
#define KEY17_2     70  //  2
#define KEY17_3     71  //  3
#define KEY17_4     68  //  4
#define KEY17_5     64  //  5
#define KEY17_6     67  //  6
#define KEY17_7     7   //  7
#define KEY17_8     21  //  8
#define KEY17_9     9   //  9
#define KEY17_STAR  22  //  *
#define KEY17_0     25  //  0
#define KEY17_SHARP 13  //  #
#define KEY17_UP    24  //  UP
#define KEY17_LEFT  8   //  LEFT
#define KEY17_OK    28  //  OK
#define KEY17_RIGHT 90  //  RIGHT
#define KEY17_DOWN  82  //  DOWN



//VARIABLE
extern int speed;
extern int freq;
extern int resolution ;
extern int channel;


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


// SETUP FUNCTION

void setupButton();
void setupWifi();
void homeUpdateDataTask(void *parameters);
void setupHomeUpdateData();
void databaseTask(void *parameters);
void setupDatabase();
void readButtonTask(void *parameters);
void setupReadButton();
void updateRelayTask(void *parameters);
void setupUpdateRelay();
void setupFAN();
void setup_IR_Controll();
void IR_Controll();


void globalTask(void *parameters);
void setupglobalTask();

















#endif