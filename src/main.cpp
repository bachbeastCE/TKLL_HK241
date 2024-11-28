#include <Arduino.h>
#include <global.h>
#include <sensor_module.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <lcd.h>

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  setupDatabase();
  setupLCD();
  setupDHT();
  setupBH1750();
  setupMQ135();
  setupRelay();
  setupButton();
  setupReadButton();
  setupHomeUpdateData();
  setupuUpdateRelay();
}

void loop()
{
      
}
