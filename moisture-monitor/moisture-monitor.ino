/*
 * Capacitive moisture monitor.
 * 
 * Adrian Bowyer
 * Reprap Ltd.
 * 
 * https://reprapltd.com
 * 
 * Licence: GPL 3
 * 
 * 30 September 2022
 * 
 */

// Compile with Heltec WiFi Kit 32

// ....arduino15/packages/esp32/hardware/esp32/1.0.6/variants/heltec_wifi_kit_32/pins_arduino.h
// https://randomnerdtutorials.com/esp32-send-email-smtp-server-arduino-ide/


// OLED graphics

#include <U8g2lib.h> 
char chBuffer[128];
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 16, 15, 4);

// 

const int aInPin = A14; //GPIO 13, PIN 5 counting from top left, 
float voltage;

void setup()
{
  pinMode(aInPin, INPUT);

  // OLED graphics.
  
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);

  u8g2.clearBuffer();
  sprintf(chBuffer, "%s", "Const. I starting...");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 0, chBuffer);
  u8g2.sendBuffer();
  delay(1000);
}

 
void loop()
{  
  voltage = (float)map(analogRead(aInPin), 0, 3000, 150, 2450);

  // Display it
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_crox5h_tf);  
  sprintf(chBuffer, "%.2f (V)", voltage); 
  u8g2.drawStr(10, 10, chBuffer);
  u8g2.sendBuffer();
  
  delay(1000);
}
