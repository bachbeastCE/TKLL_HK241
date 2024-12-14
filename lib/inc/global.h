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
#define DATABASE_URL "https://smartroomapp-1cad3-default-rtdb.asia-southeast1.firebasedatabase.app/" 
#define API_KEY "AIzaSyAIMkx2XnNrD4Fy0TWyFNAi_pKKqwHMDUg"


//DEFINE GPIO PIN
#define SCL_Pin 22
#define SDA_Pin 21
#define dht_signal_Pin 23
#define mq135_analog_Pin 35
#define fan_pin 33
#define fire_detect_Pin 15
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

extern uint8_t relayStatus[MAX_RELAY];

// SETUP FUNCTION
void setupButton();
void setupDatabase();
void setupFAN();
void setup_IR_Control();
void IR_Control();

// CONTROL FUCTION
void update_temp_humd_air();
void button_controll_relay();
void update_light();
void receive_data_from_database();
void send_data_to_database();

void setup_fire_dectect();
void fire_dectect();














#endif