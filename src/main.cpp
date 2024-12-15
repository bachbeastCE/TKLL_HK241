#include <Arduino.h>
#include <global.h>
#include <sensor_module.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <lcd.h>

void globalTask(void *parameters) {
    while (1) {
        button_relay();
        vTaskDelay(100 / portTICK_PERIOD_MS);  // Run every 100ms
    } 
}

void readButtonTask(void *parameters) { 
    while (1) {
        getKeyInput();  // Read and debounce button inputs
        vTaskDelay(10 / portTICK_PERIOD_MS); // Run every 10ms
    }
}

void air_humd_temp(void *parameters){ 
    while (1) {
        air_humd_temp_update();
        vTaskDelay(20000 / portTICK_PERIOD_MS);
    }
}

void light(void *parameters){ 
    while (1) {
      light_update();
      vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void lcd(void *parameters){ 
    while (1) {
      displayLCD();
      vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

// void database(void *parameters) { 
//     while (1) {
//         if(count==18)air_humd_temp_db();
//         if(count%5==0)light_db();
//         if(count %2 == 1)receive_db();
//         else send_db(); // SEND TO DATABASE// RECEIVE FROM DATABASE
//         count++;
//         if(count>=20);count=0;
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }

// }

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  setupLCD();
  setupButton();
  setupDatabase();
    setupFireDetect();

  xTaskCreate( air_humd_temp,"air_humd_temp update",4096, NULL, 1,NULL);
  delay(100);
  xTaskCreate( light,"light update", 2048, NULL, 2,NULL);
  delay(100);
  xTaskCreate(readButtonTask, "Read Button", 1024, NULL, 5, NULL);
  delay(100);
  xTaskCreate(globalTask,"Global Task", 1024, NULL, 10, NULL);
xTaskCreate(lcd,"Global Task", 2048, NULL, 4, NULL);
}

void loop()
{
    if(count==0)air_humd_temp_db();
    if(count%5==0)light_db();
    receive_db();
    controll_fan();
    send_db(); // SEND TO DATABASE// RECEIVE FROM DATABASE
    count++;
    if(count>=20);count=0;
    FireDetect();
    delay(1000);
}


