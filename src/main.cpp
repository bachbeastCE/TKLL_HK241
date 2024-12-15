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

void database(void *parameters) { 
    while (1) {
         // RECEIVE FROM DATABASE
        receive_db();
        //vTaskDelay(1000 / portTICK_PERIOD_MS);
        // SEND TO DATABASE
        send_db();
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

}

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  setupLCD();
  setupButton();
  setupDatabase();

  xTaskCreate( air_humd_temp,"air_humd_temp update", 8192, NULL, 10,NULL);
  delay(100);
  xTaskCreate( light,"light update", 8192, NULL, 9,NULL);
  delay(100);
  xTaskCreate(readButtonTask, "Read Button", 1024, NULL, 3, NULL);
  xTaskCreate(globalTask,"Global Task", 1024, NULL, 1, NULL);
  //xTaskCreate(database,"Database",8192, NULL,2, NULL );
}

void loop()
{
       
}


