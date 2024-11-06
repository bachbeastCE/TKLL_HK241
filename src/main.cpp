#include <Arduino.h>
#include <global.h>
#include <lcd.h>
#include <sensor_module.h>

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  setupLCD();
  setupDHT();
  setupBH1750();
  setupMQ135();
  setupHomeUpdateData();
  //displayLCD();
}

void loop()
{
  displayLCD();
}
