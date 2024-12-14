#include <Arduino.h>
#include <global.h>

void Task_10(void *parameters) {
    while (1) {
      // FUNTION HERE  
    getKeyInput();
    //fire_dectect();
      // END 
    vTaskDelay(10 / portTICK_PERIOD_MS);  // Run every 10ms
  }
}

void setup()
{
  //BEGIN
  Serial.begin(115200);
  Wire.begin();

  //SETUP
  setupDatabase();
  setupBH1750();
  setupButton();
  setupFAN();
  setupLCD();
  setup_IR_Control();
  setup_fire_dectect();
  setupLCD();
  xTaskCreate(Task_10, "Task10ms", 2048, NULL, 6,NULL);
}

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
  }

  // Kiểm tra và thực hiện nhiệm vụ 5 giây một lần
  if (currentTime - previousTime5s >= interval5s) {
    previousTime5s = currentTime;
    readLight();
    update_light();
    //displayLCD();
  }
// // Kiểm tra và thực hiện nhiệm vụ 2 giây một lần
//   if (currentTime - previousTime2s >= interval2s) {
//     previousTime2s = currentTime;
    
//   }
  // Thực hiện hàm A nếu chưa được thực hiện hoặc đã qua 2 giây kể từ lần cuối thực hiện
  if (!receive_done || (currentTime - previousTime2s >= interval2s)) {
    previousTime2s = currentTime;  // Cập nhật thời gian thực hiện receive
    receive_data_from_database();
    //IR_Control();
    receive_done = true;    // Đánh dấu receive đã được thực hiện
  }

  // Thực hiện hàm B nếu hàm A đã hoàn thành và đã qua 1 giây
  if (receive_done && (currentTime - previousTime2s >= 1000)) {
    sendtime = currentTime;  // Cập nhật thời gian thực hiện hàm B
    send_data_to_database();
    receive_done = false;   // Đặt lại cờ để chuẩn bị cho lần thực hiện tiếp theo
  }
  button_controll_relay();
}


        //IR_Controll();

//     /*
//      * Print current send values
//      */
//     Serial.println();
//     Serial.print(F("Send now: address=0x00, command=0x"));
//     Serial.print(sCommand, HEX);
//     Serial.print(F(", repeats="));
//     Serial.print(sRepeats);
//     Serial.println();

//     Serial.println(F("Send standard NEC with 8 bit address"));
//     Serial.flush();

//     // Receiver output for the first loop must be: Protocol=NEC Address=0x102 Command=0x34 Raw-Data=0xCB340102 (32 bits)
//     //IrSender.sendNEC(0x102, sCommand, sRepeats);
//     IrSender.sendNECRaw(0xF20DFF00, sRepeats);
//     /*
//      * If you want to send a raw HEX value directly like e.g. 0xCB340102 you must use sendNECRaw()
//      */
// //    Serial.println(F("Send 32 bit LSB 0xCB340102 with NECRaw()"));
// //    IrSender.sendNECRaw(0xCB340102, sRepeats);

//     /*
//      * If you want to send an "old" MSB HEX value used by IRremote versions before 3.0 like e.g. 0x40802CD3 you must use sendNECMSB()
//      */
// //    Serial.println(F("Send old 32 bit MSB 0x40802CD3 with sendNECMSB()"));
// //    IrSender.sendNECMSB(0x40802CD3, 32, sRepeats);

//     /*
//      * Increment send values
//      */
//     //sCommand += 0x11;
//     //sRepeats++;
//     // clip repeats at 4
//     //if (sRepeats > 4) {
//     //    sRepeats = 4;
//     //}

//     delay(3000);  // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
//

