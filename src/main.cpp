#include <Arduino.h>
#include <global.h>
//#include <lcd.h>
//#include <sensor_module.h>
//#include <Firebase_ESP_Client.h>
//#include <WiFi.h>


void setup()
{
  Serial.begin(115200);
  Wire.begin();
  //displayLCD()

  //setupDatabase();
  //setupLCD();
  //setupDHT();
  //setupBH1750();
  //setupMQ135();
  setupRelay();
  setupButton();
  setupReadButton();
  //setupHomeUpdateData();
}

void loop()
{
  if(isButtonPressed(0)){
    toggleRelay(relay1);
  }
    
  if(isButtonPressed(1)){
    toggleRelay(relay2);
  }

  if(isButtonPressed(2)){
    toggleRelay(relay3);
  }
  
  if(isButtonPressed(3)){
    toggleRelay(relay4);
  }
  // if(1){
  //   toggleRelay(relay1);
  //   toggleRelay(relay2);
  //   toggleRelay(relay3);
  //   toggleRelay(relay4);
  //   delay(2000);
  // }
  //displayLCD();
  // toggleRelay(relay1);
  //   toggleRelay(relay2);
  //     toggleRelay(relay3);
  //       toggleRelay(relay4);
  // Serial.println("HEAR");
}
