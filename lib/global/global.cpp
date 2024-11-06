#include <global.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>


float homeTemperature = 0;
float homeHumidity = 0;
float homeLightlevel = 0;
float homeAirlevel = 0;

//Define Firebase Data object
FirebaseData fbdo; 
FirebaseConfig config; 
FirebaseAuth auth; 
bool signupOK = false;

////////// RETURN VARIABLE 
float getHomeTemperature(){
    return homeTemperature;
}
float getHomeHumidity(){
    return homeHumidity;
}
float getHomeLightlevel(){
    return homeLightlevel;
}
float getHomeAirlevel(){
    return homeAirlevel;
}

///////// FUNCTION
void setupWifi(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
}

void homeUpdateDataTask(void *parameters){ 
    while (1) {
        //UPDATE DATA HERE
        if(homeTemperature != getTemperature()){homeTemperature = getTemperature();}
        if(homeHumidity !=getHumidity() ){homeHumidity = getHumidity() ;}
        if(homeLightlevel !=getLightlevel() ){homeLightlevel = getLightlevel() ;}
        if(homeAirlevel !=getAirlevel() ){homeAirlevel = getAirlevel() ;}
        //END UPDATE DATA
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 200ms
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

void databaseTask(void *parameters) { 
    while (1) {
        // UPDATE DATA INTO DATABASE HERE
        if (Firebase.ready() && signupOK) {
            if (Firebase.RTDB.setInt(&fbdo, "Home/homeTemperature", homeTemperature)) {
                Serial.println("Temperature updated");
            }
            if (Firebase.RTDB.setFloat(&fbdo, "Home/homeHumidity", homeHumidity)) {
                Serial.println("Humidity updated");
            }
            if (Firebase.RTDB.setFloat(&fbdo, "Home/homeLightlevel", homeLightlevel)) {
                Serial.println("Light level updated");
            }
            if (Firebase.RTDB.setFloat(&fbdo, "Home/homeAirlevel", homeAirlevel)) {
                Serial.println("Air level updated");
            }
        }
    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL); 
    Serial.print("Stack high water mark: "); 
    Serial.println(uxHighWaterMark);
        vTaskDelay(10000 / portTICK_PERIOD_MS); // Delay for 5000ms
    }
}

void setupDatabase() { 
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

    xTaskCreate(
        databaseTask, // Task function 
        "Database", // Name of the task 
        7500, // Stack size (increase from 2000 to 3000)
        NULL, // Task input parameter 
        2, // Priority of the task 
        NULL // Task handle 
    );
}




