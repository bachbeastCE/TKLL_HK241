#include <Arduino.h>
#include <global.h>
#include <lcd.h>
#include <sensor_module.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>


void setup()
{
  Serial.begin(115200);
  Wire.begin();
  //displayLCD()

  setupDatabase();
  setupLCD();
  setupDHT();
  setupBH1750();
  setupMQ135();
  setupRelay();
  setupHomeUpdateData();
}

void loop()
{
  turnOnRelay(relay1);
  displayLCD();
}
