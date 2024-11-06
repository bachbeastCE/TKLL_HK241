#include <global.h>

float homeTemperature = 0;
float homeHumidity = 0;
float homeLightlevel = 0;
float homeAirlevel = 0;

void homeUpdateDataTask(void *parameters){ 
    while (1) {
        //UPDATE DATA HERE
        if(homeTemperature != getTemperature()){homeTemperature = getTemperature();}
        if(homeHumidity !=getHumidity() ){homeHumidity = getHumidity() ;}
        if(homeLightlevel !=getLightlevel() ){homeLightlevel = getLightlevel() ;}
        if(homeAirlevel !=getAirlevel() ){homeAirlevel = getAirlevel() ;}
        //END UPDATE DATA
        vTaskDelay(200 / portTICK_PERIOD_MS); // Delay for 200ms
    }
}

void setupHomeUpdateData(){ 
    xTaskCreate( 
        homeUpdateDataTask, // Task function 
        "Home update", // Name of the task 
        1500, // Stack size 
        NULL, // Task input parameter 
        1, // Priority of the task 
        NULL // Task handle 
    );
}