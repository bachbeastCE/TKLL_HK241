#include <Arduino.h>
#include <global.h>
#include <lcd.h>
#include <sensor_module.h>

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  setupLCD();
  setupDHT();
  setupBH1750();
  setupMQ135();
  setupHomeUpdateData();
  //displayLCD();
}

<<<<<<< Updated upstream
void loop()
{
  displayLCD();
=======
//uint8_t sCommand = 0x0D;
//uint8_t sRepeats = 0;

unsigned long previousTime20s = 0;
unsigned long previousTime5s = 0;
unsigned long previousTime2s = 0;
unsigned long sendtime = 0;
const unsigned long interval20s = 20000;
const unsigned long interval5s = 5000;
const unsigned long interval2s = 2000;


bool receive_done = false;


void loop() {
  unsigned long currentTime = millis();

  // Kiểm tra và thực hiện nhiệm vụ 20 giây một lần
  if (currentTime - previousTime20s >= interval20s) {
    previousTime20s = currentTime;
    readAir();
    readTempHump();
    update_temp_humd_air();
    speedControllerFan(temperature);
  }

  // Kiểm tra và thực hiện nhiệm vụ 5 giây một lần
  if (currentTime - previousTime5s >= interval5s) {
    previousTime5s = currentTime;
    readLight();
    update_light();
  }

  if (currentTime - previousTime2s >= interval2s) {
    previousTime2s = currentTime;
    
  }
  // Thực hiện hàm A nếu chưa được thực hiện hoặc đã qua 2 giây kể từ lần cuối thực hiện
  if (!receive_done || (currentTime - previousTime2s >= interval2s)) {
    previousTime2s = currentTime;  // Cập nhật thời gian thực hiện receive
    receive_data_from_database();
    IR_Control();
    receive_done = true;    // Đánh dấu receive đã được thực hiện
  }

  // Thực hiện hàm B nếu hàm A đã hoàn thành và đã qua 1 giây
  if (receive_done && (currentTime - previousTime2s >= 1000)) {
    sendtime = currentTime;  // Cập nhật thời gian thực hiện hàm B
    send_data_to_database();
    receive_done = false;   // Đặt lại cờ để chuẩn bị cho lần thực hiện tiếp theo
  }
  button_controll_relay();
>>>>>>> Stashed changes
}
