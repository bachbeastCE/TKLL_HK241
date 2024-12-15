#ifndef global_h
#define global_h




//LIBRARY INCLUDE
#include <Arduino.h>
#include <sensor_module.h>
#include <button.h>
#include <lcd.h>
#include <string.h>
#include <relay.h>

#define WIFI_SSID "Duy Bach"
#define WIFI_PASSWORD "28082004"
#define DATABASE_URL "https://smartroomapp-1cad3-default-rtdb.asia-southeast1.firebasedatabase.app/" 
#define API_KEY "AIzaSyAIMkx2XnNrD4Fy0TWyFNAi_pKKqwHMDUg"
//DEFINE GPIO PIN
#define SCL_Pin 22
#define SDA_Pin 21
#define dht_signal_Pin 23
#define mq135_analog_Pin 35
#define fan_pin 33
#define fire_detect 15
#define buzzer 32


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
extern int homeLightlevel;
extern int homeAirlevel;
extern uint8_t relayStatus[MAX_RELAY];
extern int count;

// RETURN FUNCTION
float getHomeTemperature();
float getHomeHumidity();
float getHomeLightlevel();
float getHomeAirlevel();


// SETUP FUNCTION
void setupButton();
void setupWifi();
void setupFireDetect();
void FireDetect();
void air_humd_temp_update();
void light_update();
void setupDatabase();
void button_relay();
void receive_db();
void send_db();
void light_db();
void air_humd_temp_db();
void controll_fan();



#endif