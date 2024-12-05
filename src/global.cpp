#include <global.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <IRremote.hpp>

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
int speed = 100;
int freq = 5000;
int resolution = 8;
int channel= 0;
void setupFAN(){
    ledcSetup(channel,freq,resolution);
    ledcAttachPin(fan_pin,channel);
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



////////////////////////////////////////////////////////
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






////////////////////////////////////////////////////////


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

        if (Firebase.RTDB.getInt(&fbdo, "/Fan/speed") && fbdo.dataType() == "int") {
                int tmp1 = fbdo.intData();
                if (tmp1 != speed) {
                    speed = tmp1;
                    ledcWrite(channel,(speed * 255) /100);
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


////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////







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


////////////////////////////////////////////////////////


void setup_IR_Controll(){
    while (!Serial)
        ; // Wait for Serial to become available. Is optimized away for some cores.

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
    Serial.print(F("Send IR signals at pin "));
    Serial.println(IR_SEND_PIN);

    /*
     * The IR library setup. That's all!
     */
    IrSender.begin(); // Start with IR_SEND_PIN -which is defined in PinDefinitionsAndMore.h- as send pin and enable feedback LED at default feedback LED pin
    disableLEDFeedback(); // Disable feedback LED at default feedback LED pin
}


void IR_Controll(){
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key0") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_0, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key0" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key1") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_1, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key1" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key2") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_2, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key2" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key3") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_3, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key3" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key4") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_4, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key4" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key5") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_5, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key5" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key6") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_6, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key6" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key7") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_7, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key7" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key8") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_8, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key8" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key9") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_9, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key9" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key_star") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_STAR, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key_star" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key_sharp") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_SHARP, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key_sharp" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key_up") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_UP, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key_up" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key_down") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_DOWN, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key_down" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key_left") && fbdo.dataType()  == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_LEFT, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key_left" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key_right") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_RIGHT, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key_right" , 0);}
    if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key_ok") && fbdo.dataType() == "int" && fbdo.intData() == 1){ IrSender.sendNEC(0x102, KEY17_OK, 0);Firebase.RTDB.setInt(&fbdo, "/ir_device/key_ok" , 0);}
}



////////////////////////////////////////////////////////

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
        4096, 
        NULL, 
        1,  // Lowest priority
        NULL
    );
}

