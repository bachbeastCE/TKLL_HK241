#include <global.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>


//READ SENSOR VARIABLE 
float homeTemperature = 0;
float homeHumidity = 0;
int homeLightlevel = 0;
int homeAirlevel = 0;
int count=0;
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


void air_humd_temp_update(){
    homeTemperature = getTemperature();
    homeHumidity = getHumidity() ;
    homeAirlevel = getAirlevel();
    
}

void light_update(){
    homeLightlevel = getLightlevel();
}

void air_humd_temp_db(){
    Firebase.RTDB.setFloat(&fbdo, "Home/homeTemperature", homeTemperature);
    Firebase.RTDB.setFloat(&fbdo, "Home/homeHumidity", homeHumidity);
    Firebase.RTDB.setInt(&fbdo, "Home/homeAirlevel", homeAirlevel);
}


void light_db(){
    Firebase.RTDB.setInt(&fbdo, "Home/homeLightlevel", homeLightlevel);
}

void receive_db(){

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
}

void send_db(){
        for (uint8_t i = 0; i < MAX_RELAY; i++) {
            if (relays[i].changeflag == 1) {
                relays[i].changeflag = 0;
                char tmp[32];
                snprintf(tmp, sizeof(tmp), "/Relay/Relay%d", i + 1);
                Firebase.RTDB.setInt(&fbdo, tmp, relays[i].status);
            }   
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
}

void setupButton()
{
    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    pinMode(button3, INPUT_PULLUP);
    pinMode(button4, INPUT_PULLUP);
}



void button_relay(){
    for (uint8_t i = 0; i < MAX_RELAY; i++) {
            if (isButtonPressed(i)) {
                relays[i].toolgeRelay();  // Toggle relay state
        }
    }
}



void setupFireDetect(){
      pinMode(fire_detect, INPUT);
      pinMode(buzzer,OUTPUT);
}

void FireDetect(){
    int flame_state = digitalRead(fire_detect);
    Firebase.RTDB.setInt(&fbdo, "/Fire", !flame_state);
    if (flame_state == HIGH)
        digitalWrite(buzzer,LOW);
    else
        digitalWrite(buzzer,HIGH);
}