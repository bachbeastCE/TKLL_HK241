#include <global.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <IRremote.hpp>

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

int speed = 100;
int freq = 5000;
int resolution = 8;
int channel= 1;

void setupFAN(){
    ledcSetup(channel,freq,8);
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
        IR_Control();
            //RECIVE FAN SPEED DATA
        Firebase.RTDB.getString(&fbdo, "/Fan/mode");
        String fanMode = fbdo.stringData();
        if(fanMode == "Manual"){
            if (Firebase.RTDB.getInt(&fbdo, "/Fan/speed") && fbdo.dataType() == "int") {
                    if (fbdo.intData() != speed) {
                        speed = fbdo.intData();
                        ledcWrite(channel,(speed * 255) /100);
                    }
                }
            }
        else{
                if (homeTemperature < 25.0) {
                    ledcWrite(channel,(0 * 255) /100); // Fan off
                } else if (homeTemperature >= 25.0 && homeTemperature < 27.0) {
                    ledcWrite(channel,(25 * 255) /100); // Fan at 50% speed
                } else if (homeTemperature >= 27.0 && homeTemperature < 29.0) {
                ledcWrite(channel,(50 * 255) /100); // Fan at 50% speed
                } else if (homeTemperature >= 29.0 && homeTemperature < 31.0) {
                    ledcWrite(channel,(75 * 255) /100); // Fan at 50% speed
                } else {
                    ledcWrite(channel,(100 * 255) /100); // Fan at 100% speed
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
    if (flame_state == HIGH){
        digitalWrite(buzzer,LOW);
        Firebase.RTDB.setInt(&fbdo, "/Fire", 0);
    }     
    else{
        digitalWrite(buzzer,HIGH);
        Firebase.RTDB.setInt(&fbdo, "/Fire", 1);
    }
        
}

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

void IR_Control(){
        if (Firebase.RTDB.getInt(&fbdo, "/ir_device/key") && fbdo.dataType() == "int"){
        int key = fbdo.intData();
        if(key != 0){
            Firebase.RTDB.setInt(&fbdo, "/ir_device/key" , 0);
            switch (key)
            {
            case 69:
                IrSender.sendNEC(0x102, KEY17_1, 0);;
                break;
            case 70:
                IrSender.sendNEC(0x102, KEY17_2, 0);
                break;
            case 71:
                IrSender.sendNEC(0x102, KEY17_3, 0);
                break;
            case 68:
                IrSender.sendNEC(0x102, KEY17_4, 0);
                break;
            case 64:
                IrSender.sendNEC(0x102, KEY17_5, 0);
                break;                                        
            case 67:
                IrSender.sendNEC(0x102, KEY17_6, 0);
                break;
            case 7:
                IrSender.sendNEC(0x102, KEY17_7, 0);
                break;
            case 21:
                IrSender.sendNEC(0x102, KEY17_8, 0);
                break;
            case 9:
                IrSender.sendNEC(0x102, KEY17_9, 0);
                break;
            case 22:
                IrSender.sendNEC(0x102, KEY17_STAR, 0);
                break;
            case 25:
                IrSender.sendNEC(0x102, KEY17_0, 0);
                break;
            case 13:
                IrSender.sendNEC(0x102, KEY17_SHARP, 0);
                break;
            case 24:
                IrSender.sendNEC(0x102, KEY17_UP, 0);
                break;
            case 8:
                IrSender.sendNEC(0x102, KEY17_LEFT, 0);
                break;
            case 28:
                IrSender.sendNEC(0x102, KEY17_OK, 0);
                break;
            case 90:
                IrSender.sendNEC(0x102, KEY17_RIGHT, 0);
                break;
            case 82:
                IrSender.sendNEC(0x102, KEY17_DOWN, 0);
                break;                              
            default:
                break;  
            }
        }
    }
}
