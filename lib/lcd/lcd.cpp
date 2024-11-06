#include "lcd.h"

LiquidCrystal_I2C lcd(adress, lcdColumns, lcdRows);
String messageToScroll = "DO AN THIET KE LUAN LY";
int lcd_status = 1;

// TMP VARIABLE
float tmp_temp=0;
float tmp_humid=0;
float tmp_lightlevel=0;
float tmp_airlevel=0;

void setupLCD()
{
    // initialize LCD
    lcd.init();
    // turn on LCD backlight
    lcd.backlight();
}

void displayLCD(){
  switch (lcd_status)
  {
  case INTRO:{
    lcd.setCursor(0, 1);
    lcd.print("Xin chao");
    delay(1000);
    lcd.clear();
    scrollText(0, messageToScroll, 500, lcdColumns);
    scrollText(1, messageToScroll, 500, lcdColumns);
    delay(3000);
    //CHANGE STATUS
    lcd_status = 1;
    break;
  }
  case HOME:{
    //if( tmp_temp != homeTemperature || tmp_humid !=homeHumidity){
      tmp_temp= homeTemperature;
      tmp_humid= homeHumidity;
      tmp_lightlevel= homeLightlevel;
      tmp_airlevel= homeAirlevel;
      lcd.setCursor(0, 0);
      lcd.print("Temperature: "); lcd.print(tmp_temp);lcd.print("oC");
      lcd.setCursor(0, 1);
      lcd.print("Humidity: "); lcd.print(tmp_humid);lcd.print("%");
      lcd.setCursor(0, 2);
      lcd.print("Light: "); lcd.print(tmp_lightlevel);lcd.print(" lux");
      lcd.setCursor(0, 3);
      lcd.print("Air: "); lcd.print(tmp_airlevel);lcd.print(" ppm");
      delay(1000);
      lcd.clear();
    //}
        break;
  }
  default:
    break;
  }

    





}

void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i=0; i < lcdColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}