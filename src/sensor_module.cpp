#include "sensor_module.h"

DHT dht(DHT_PIN, DHT_VERSION);
BH1750FVI lightMeter(BH1750_DEFAULT_I2CADDR, BH1750_ONE_TIME_HIGH_RES_MODE, BH1750_SENSITIVITY_DEFAULT, BH1750_ACCURACY_DEFAULT);
MQ135 airMeter = MQ135(mq135_analog_Pin);

float temperature = -1;
float humidity = -1;
float lightlevel = -1;
float airlevel = -1;

void readTempHump(){
    humidity = dht.readHumidity(); 
    temperature = dht.readTemperature(); 
    if (isnan(humidity) || isnan(temperature)) 
    { 
        Serial.println(F("Failed to read from DHT sensor!"));  
    } 
    else { 
        Serial.print(F("Humidity: ")); 
        Serial.print(humidity); 
        Serial.println(F("%")); 
        Serial.print(F("Temperature: ")); 
        Serial.print(temperature); 
        Serial.println(F(" °C")); } 
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
}
void readLight(){
    lightlevel = lightMeter.readLightLevel();  // Start measurement -> wait for result -> read result -> return result or 4294967295 if communication error occurs
    Serial.println();
    Serial.print(F("Light level.........: "));
    if (lightlevel != BH1750_ERROR) {  // BH1750_ERROR=4294967295
        Serial.print(lightlevel, 2);
        Serial.println(F(" lux"));
    } else {
        Serial.println(F("error"));
    }
}
float getLightlevel(){
    return lightlevel;
}

void readAir(){
    airlevel = (((analogRead(35)* 0.004887)*100)*2);//airMeter.getPPM();
    Serial.print(F("Air level: "));
    Serial.print(airlevel);
    Serial.println(F(" ppm"));
}
float getAirlevel(){
    return airlevel;
}