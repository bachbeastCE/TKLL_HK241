#include <Arduino.h>
#include <global.h>
#include <sensor_module.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
#include <lcd.h>

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  setupDatabase();
  setupLCD();
  setupDHT();
  setupBH1750();
  setupMQ135();
  setupButton();
  setupReadButton();
  setupHomeUpdateData();
  //setupUpdateRelay();
  setupglobalTask();
  setupFAN();
  setup_IR_Controll();
}

//uint8_t sCommand = 0x0D;
//uint8_t sRepeats = 0;

void loop() {
        IR_Controll();

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
 }
