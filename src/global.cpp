#include <global.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>


//READ SENSOR VARIABLE 
float homeTemperature = 0;
float homeHumidity = 0;
float homeLightlevel = 0;
float homeAirlevel = 0;
int count=15;
//RELAY VARIABLE
Relay relays[MAX_RELAY]={Relay(relay1),Relay(relay2),
                            Relay(relay3),Relay(relay4)};
uint8_t relayStatusFlag[MAX_RELAY]= {0,0,0,0};
uint8_t relayStatus[MAX_RELAY]= {0,0,0,0};

//PWM VARIABLE
const int freq = 5000;
const int resolution = 8;
const int fanChanel= 0;

void setupFAN(){
    ledcSetup(fanChanel,freq,resolution);
    ledcAttachPin(fan_pin,fanChanel);
}





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
        homeLightlevel = getLightlevel() ;
        homeHumidity = getHumidity() ;
        homeTemperature = getTemperature();
        homeAirlevel = getAirlevel() ;
        //END UPDATE DATA
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void setupHomeUpdateData(){ 
    xTaskCreate( 
        homeUpdateDataTask, // Task function 
        "Home update", // Name of the task 
        2048, // Stack size 
        NULL, // Task input parameter 
        2, // Priority of the task 
        NULL // Task handle 
    );
}

void databaseTask(void *parameters) { 
    while (1) {
        // UPDATE DATA INTO DATABASE HERE
        if (Firebase.ready() && signupOK) {
            if (Firebase.RTDB.setInt(&fbdo, "Home/homeTemperature", homeTemperature)&& count==15) {
                //Serial.println("Temperature updated");
            }
            
            if (Firebase.RTDB.setFloat(&fbdo, "Home/homeHumidity", homeHumidity) && count==15) {
                //Serial.println("Humidity updated");
            }
            if (Firebase.RTDB.setFloat(&fbdo, "Home/homeLightlevel", homeLightlevel)) {
                //Serial.println("Light level updated");
            }
            if (Firebase.RTDB.setFloat(&fbdo, "Home/homeAirlevel", homeAirlevel)&& count==15) {
                //Serial.println("Air level updated");
            }
        }

         // RECEIVE FROM DATABASE
        for (int i = 0; i < MAX_RELAY; i++) {
            char tmp[32];
            snprintf(tmp, sizeof(tmp), "/Relay/Relay%d", i + 1);
            if (Firebase.RTDB.getInt(&fbdo, tmp) && fbdo.dataType() == "int" && relays[i].changeflag != 1) {
                int tmp1 = fbdo.intData();
                if (tmp1 != relays[i].status) {
                    relays[i].status = tmp1;
                    (tmp1 == 0) ? (relays[i].turnRelayOFF()) : (relays[i].turnRelayON());
                }
            }
        }  

        // SEND TO DATABASE
        for (uint8_t i = 0; i < MAX_RELAY; i++) {
            if (relays[i].changeflag == 1) {
                relays[i].changeflag = 0;
                char tmp[32];
                snprintf(tmp, sizeof(tmp), "/Relay/Relay%d", i + 1);
                Firebase.RTDB.setInt(&fbdo, tmp, relays[i].status);
            }   
        }
        UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL); 
        Serial.print("Stack high water mark: "); 
        Serial.println(uxHighWaterMark);
        (count == 15)?(count=0):(count++);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
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
        8192, // Stack size (increase from 2000 to 3000)
        NULL, // Task input parameter 
        2, // Priority of the task 
        NULL // Task handle 
    );
}

void setupButton()
{
    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    pinMode(button3, INPUT_PULLUP);
    pinMode(button4, INPUT_PULLUP);
}
void readButtonTask(void *parameters) { 
    while (1) {
        getKeyInput();  // Read and debounce button inputs
        vTaskDelay(10 / portTICK_PERIOD_MS); // Run every 10ms
    }
}

void setupReadButton() { 
    xTaskCreate(
        readButtonTask, 
        "Read Button", 
        1024, 
        NULL, 
        3,  // Higher priority for button inputs
        NULL
    );
}

void updateRelayTask(void *parameters) { 
    while (1) {
       
    }
}

void setupUpdateRelay() { 
    xTaskCreate(
        updateRelayTask, 
        "Update Relay", 
        1024,  // Increased stack size for Firebase operations
        NULL, 
        2,  // Lower priority to avoid blocking other tasks
        NULL
    );
}

void globalTask(void *parameters) {
    while (1) {
        for (uint8_t i = 0; i < MAX_RELAY; i++) {
            if (isButtonPressed(i)) {
                relays[i].toolgeRelay();  // Toggle relay state
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);  // Run every 100ms
    }
}

void setupglobalTask() {
    xTaskCreate(
        globalTask, 
        "Global Task", 
        1024, 
        NULL, 
        1,  // Lowest priority
        NULL
    );
}

