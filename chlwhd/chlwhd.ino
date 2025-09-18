#include <DS1302.h>
//#include <Wire.h>
#include <LiquidCrystal_I2C.h>

DS1302 rtc(2,3,4);

LiquidCrystal_I2C lcd(0x27,16,2);

void setup(){
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  
  //rtc.setDOW(MONDAY);
  //rtc.setTime(17,03,30);
  //rtc.setDate(12,13,2022);
  }

  void loop(){
  Serial.print(rtc.getDOWStr());
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  Serial.println(rtc.getTimeStr());

  lcd.clear();
  data();
  printTime();
  delay(1000);
  }

  void data(){
    lcd.setCursor(0,0);
    lcd.print(rtc.getDOWStr());
    lcd.setCursor(3,1);
    lcd.print(rtc.getDateStr());
    }

    void printTime(){
      lcd.setCursor(8,0);
      lcd.print(rtc.getTimeStr());
     // delay(1000);
      }
