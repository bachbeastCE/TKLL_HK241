#ifndef global_h
#define global_h

//LIBRARY INCLUDE
#include <sensor_module.h>

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

//FUNCTION
void homeUpdateDataTask(void *parameters);
void setupHomeUpdateData();

#endif