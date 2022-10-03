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

#include "secret.h"

// OLED graphics

#include <U8g2lib.h> 
char chBuffer[128];
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, 16, 15, 4);

// 

const int aInPin = A14; //GPIO 13, PIN 5 counting from top left, 
float percentage;
float average = 50.0; // Bayesean...
float moving = 5.0;
float dry = 2485.0;
float wet = 970.0;
float threshold = 40.0;
char warn = ' ';

unsigned long tim, lastTim, interval;

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
  sprintf(chBuffer, "%s", "Water % starting...");
  u8g2.drawStr(64 - (u8g2.getStrWidth(chBuffer) / 2), 0, chBuffer);
  u8g2.sendBuffer();
  delay(1000);
  lastTim = millis();
  interval = 1000*5; //*24*3600; // One day in ms.
}

void SendEmail()
{

}

 
void loop()
{  
  percentage = (float)analogRead(aInPin);
  percentage = 100.0*(dry - percentage)/(dry - wet);
  average = (moving*average + percentage)/(moving + 1.0);

  // Display it
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_crox5h_tf);  
  sprintf(chBuffer, "%c%d%% water", warn, (int)(average + 0.5)); 
  u8g2.drawStr(10, 10, chBuffer);
  u8g2.sendBuffer();

  if(average < threshold)
  {
      warn = '*';
  } else
  {
      warn = ' ';
  }

  tim = millis();

  if (tim - lastTim >= interval) 
  {
    lastTim = tim;
    if(warn != ' ')
      SendEmail();
  }
  
  delay(1000);
}
