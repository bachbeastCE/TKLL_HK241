#ifndef lcd_h
#define lcd_h

//LIBRARY INCLUDE
#include <global.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//DEFINE LCD_STATUS
#define INTRO 0
#define HOME 1

//VARIABLE
const int lcdColumns = 20;
const int lcdRows = 4;
const u_int8_t adress = 0x27;

extern int lcd_status;

//FUNCTION
void setupLCD();
void displayLCD();
void scrollText(int row, String message, int delayTime, int lcdColumns);

#endif