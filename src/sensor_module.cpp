#include "sensor_module.h"

DHT dht(DHT_PIN, DHT_VERSION);
BH1750FVI lightMeter(BH1750_DEFAULT_I2CADDR, BH1750_ONE_TIME_HIGH_RES_MODE, BH1750_SENSITIVITY_DEFAULT, BH1750_ACCURACY_DEFAULT);
MQ135 airMeter = MQ135(mq135_analog_Pin);

float temperature = -1;
float humidity = -1;
float lightlevel = -1;
float airlevel = -1;

void setupDHT(){
    dht.begin();
    xTaskCreate( 
        DHTTask, // Task function 
        "DHT Task", // Name of the task 
        2000, // Stack size 
        NULL, // Task input parameter 
        10, // Priority of the task 
        NULL // Task handle 
    );
}

void DHTTask(void *parameters){ 
    while (1) { 
        humidity = dht.readHumidity(); 
        temperature = dht.readTemperature(); 
        if (isnan(humidity) || isnan(temperature)) 
        { 
            Serial.println("Failed to read from DHT sensor!"); 
        } 
        else { 
            Serial.print("Humidity: "); 
            Serial.print(humidity); 
            Serial.print(" %\t"); 
            Serial.print("Temperature: "); 
            Serial.print(temperature); 
            Serial.println(" °C"); } 
            vTaskDelay(10000 / portTICK_PERIOD_MS); 
        } 
}

float getTemperature(){
    return temperature;
}

float getHumidity(){
    return humidity;
}

void setupBH1750() {
    Serial.println();
    while (lightMeter.begin() != true) {
        Serial.println(F("ROHM BH1750FVI is not present"));  //(F()) saves string to flash & keeps dynamic memory free
        delay(5000);
    }

    Serial.println(F("ROHM BH1750FVI is present"));
    /* Change BH1750 settings on the fly */
    lightMeter.setCalibration(1.06);  // Call before "readLightLevel()"
    lightMeter.setSensitivity(1.00);  // Call before "readLightLevel()"
    lightMeter.setResolution(BH1750_CONTINUOUS_HIGH_RES_MODE);  // Continuous measurement

    xTaskCreate(
        BH1750Task,  // Task function
        "BH1750 Task",  // Name of the task
        2000,  // Stack size
        NULL,  // Task input parameter
        5,  // Priority of the task
        NULL  // Task handle
    );
}

void BH1750Task(void *pvParameters) {
    while (1) {
        lightlevel = lightMeter.readLightLevel();  // Start measurement -> wait for result -> read result -> return result or 4294967295 if communication error occurs
        Serial.println();
        Serial.print(F("Light level.........: "));
        if (lightlevel != BH1750_ERROR) {  // BH1750_ERROR=4294967295
            Serial.print(lightlevel, 2);
            Serial.println(F(" lux"));
        } else {
            Serial.println(F("error"));
        }

        // Serial.print(F("Power for 555nm wave: "));
        // if (lightlevel != BH1750_ERROR) {  // BH1750_ERROR=4294967295
        //     Serial.print((lightlevel / 683), 2);
        //     Serial.println(F(" Watt/m^2"));
        // } else {
        //     Serial.println(F("error"));
        // }
        // Serial.print(F("Resolution value....: 0x"));
        // Serial.println(lightMeter.getResolution(), HEX);  // Print the current resolution

        // Serial.print(F("Calibration value...: "));
        // Serial.println(lightMeter.getCalibration());  // Print the current calibration

        // Serial.print(F("Sensitivity value...: "));
        // Serial.println(lightMeter.getSensitivity());  // Print the current sensitivity

        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Delay for 200 ms
    }
}

float getLightlevel(){
    return lightlevel;
}

void setupMQ135(){
    pinMode(35,INPUT);
    xTaskCreate( 
        MQ135Task, // Task function 
        "MQ135 Task", // Name of the task 
        2000, // Stack size 
        NULL, // Task input parameter 
        10, // Priority of the task 
        NULL // Task handle 
    );
}

void MQ135Task(void *pvParameters){
    while(1){
        airlevel = (((analogRead(35)* 0.004887)*100)*2);//airMeter.getPPM();
        Serial.print("Air level: ");
        Serial.print(airlevel);
        Serial.println(" ppm");
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

float getAirlevel(){
    return airlevel;
}