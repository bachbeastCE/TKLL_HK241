#include <global.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

float homeTemperature = 0;
float homeHumidity = 0;
float homeLightlevel = 0;
float homeAirlevel = 0;
bool changeRelayStatus[MAX_RELAY] = {true,true,true,true};
uint8_t relayStatus[MAX_RELAY]= {0,0,0,0};

bool buttonState1 = 0;
bool buttonState2 = 0;
bool buttonState3 = 0;
bool buttonState4 = 0;


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
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setupHomeUpdateData(){ 
    xTaskCreate( 
        homeUpdateDataTask, // Task function 
        "Home update", // Name of the task 
        1500, // Stack size 
        NULL, // Task input parameter 
        2, // Priority of the task 
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
        vTaskDelay(10000 / portTICK_PERIOD_MS);
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

void setupRelay()
{
    pinMode(relay1, OUTPUT);
    digitalWrite(relay1, HIGH);
    pinMode(relay2, OUTPUT);
    digitalWrite(relay2, HIGH);
    pinMode(relay3, OUTPUT);
    digitalWrite(relay3, HIGH);
    pinMode(relay4, OUTPUT);
    digitalWrite(relay4, HIGH); 
}

void toggleRelay(int relay)
{
    switch (relay)
    {
    case relay1:{
        changeRelayStatus[0] = true ;
        if (relayStatus[0] == 1){
            relayStatus[0] = 0;
            digitalWrite(relay1, HIGH);        
        }
        else{
            relayStatus[0] = 1;
            digitalWrite(relay1, LOW);    
        }
        
        break;
        }
    case relay2:{
        changeRelayStatus[1] = true ;
        if (relayStatus[1] == 1 ){
            relayStatus[1] = 0;
            digitalWrite(relay2, HIGH); 
            
        }
        else{
            relayStatus[1] = 1;
            digitalWrite(relay2, LOW);     
        }
        break;
        }
    case relay3:{
        changeRelayStatus[2] = true ;
        if (relayStatus[2] == 1){
            relayStatus[2] = 0;
            digitalWrite(relay3, HIGH); 
            
        }
        else{
            digitalWrite(relay3, LOW); 
            relayStatus[2] = 1;
        }
         
        break;
    }
    case relay4:{
        changeRelayStatus[3] = true ;
        if (relayStatus[3] == 1){
            relayStatus[3] = 0;
            digitalWrite(relay4, HIGH); 
            
        }
        else{
             relayStatus[3] = 1;
            digitalWrite(relay4, LOW); 
        }
        break;
    }  
    }   
}



void setupButton()
{
    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    pinMode(button3, INPUT_PULLUP);
    pinMode(button4, INPUT_PULLUP);
}

void readButtonTask(void *parameters){ 
    while (1) {
        getKeyInput();
        vTaskDelay(10 / portTICK_PERIOD_MS); // Delay for 10ms
    }
}

void setupReadButton(){ 
    xTaskCreate( 
        readButtonTask, // Task function 
        "Read Button", // Name of the task 
        2000, // Stack size 
        NULL, // Task input parameter 
        1, // Priority of the task 
        NULL // Task handle 
    );
}

void button_change_relay()
{
   for(int i =0;i< MAX_BUTTON;i++){
      if(button_flag[i]){
            (homeDevice[i].getStatus()=="OFF")?(homeDevice[i].turnON()):(homeDevice[i].turnOFF());
            char tmp[50]= "Device/";
            strcat(tmp,homeDevice[i].getName().c_str());
            Firebase.RTDB.setString(&fbdo,tmp,homeDevice[i].getStatus() );
      }
   }
};

void database_change_relay()
{
    for(int i =0;i< MAX_RELAY;i++){
        char tmp[50]= "/Device/";
        strcat(tmp,homeDevice[i].getName().c_str());
        if(Firebase.RTDB.getString(&fbdo, tmp) && fbdo.dataType() == "String"){
            (fbdo.stringData()== "OFF")?(homeDevice[i].turnOFF()):(homeDevice[i].turnON());
        }
   }
};

void updateRelay(void *parameters){ 
    while (1) {
        database_change_relay();
        button_change_relay();
        // if(changeRelayStatus[0] && Firebase.RTDB.setString(&fbdo, "Relay/Relay1", relayStatus[0]))changeRelayStatus[0]= false;
        // if(changeRelayStatus[1] && Firebase.RTDB.setString(&fbdo, "Relay/Relay2", relayStatus[1]))changeRelayStatus[1]= false;  
        // if(changeRelayStatus[2] && Firebase.RTDB.setString(&fbdo, "Relay/Relay3", relayStatus[2]))changeRelayStatus[2]= false;
        // if(changeRelayStatus[3] && Firebase.RTDB.setString(&fbdo, "Relay/Relay4", relayStatus[3]))changeRelayStatus[3]= false;
        vTaskDelay(10 / portTICK_PERIOD_MS); 
    }
}


void updateRelayTask(void *parameters){ 
    while (1) {
        button_change_relay();
        // if(changeRelayStatus[0] && Firebase.RTDB.setString(&fbdo, "Relay/Relay1", relayStatus[0]))changeRelayStatus[0]= false;
        // if(changeRelayStatus[1] && Firebase.RTDB.setString(&fbdo, "Relay/Relay2", relayStatus[1]))changeRelayStatus[1]= false;  
        // if(changeRelayStatus[2] && Firebase.RTDB.setString(&fbdo, "Relay/Relay3", relayStatus[2]))changeRelayStatus[2]= false;
        // if(changeRelayStatus[3] && Firebase.RTDB.setString(&fbdo, "Relay/Relay4", relayStatus[3]))changeRelayStatus[3]= false;
        vTaskDelay(10 / portTICK_PERIOD_MS); 
    }
}

void setupuUpdateRelay(){ 
    xTaskCreate( 
        updateRelayTask, // Task function 
        "Update Relay", // Name of the task 
        4096, // Stack size 
        NULL, // Task input parameter 
        4, // Priority of the task 
        NULL // Task handle 
    );
}



