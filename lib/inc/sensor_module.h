#ifndef sensor_module_h
#define sensor_module_h

//INCLUDE LIBARRY
#include <Arduino.h>
#include <global.h>
#include <DHT.h> 
#include <BH1750FVI.h>
#include <MQ135.h>
#include <MQUnifiedsensor.h>
#include <Wire.h>

#define DHT_PIN dht_signal_Pin
#define DHT_VERSION DHT11


//INCLUDE DATA VARIABLE
extern float temp;
extern float humidity;
extern float lightlevel;
extern float airlevel;

// IMPLEMENT FOR TEMPERATURE & HUMIDITY SENSOR
void readTempHump();
float getTemperature();
float getHumidity();

// IMPLEMENT FOR LIGHT SENSOR
void setupBH1750();
void readLight();
float getLightlevel();

// IMPLEMENT FOR AIR SENSOR
void readAir();
float getAirlevel();

#endif