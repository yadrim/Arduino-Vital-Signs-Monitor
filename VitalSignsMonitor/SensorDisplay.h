#ifndef __SENSORDISPLAY_H_INCLUDED__
#define __SENSORDISPLAY_H_INCLUDED__

#include <Adafruit_GFX.h>    // Include core graphics library
#include <Adafruit_ILI9341.h> // Include Adafruit_ILI9341 library to drive the display
#include <Wire.h>
#include <virtuabotixRTC.h> 

virtuabotixRTC myRTC(2, 3, 4);////

/**
     DECLARE PIN FOR DISPLAY
*/
#define TFT_DC 9
#define TFT_RST 8  // You can also connect this to the Arduino reset in which case, set this #define pin to -1!
#define TFT_CS 10
// The rest of the pins are pre-selected as the default hardware SPI for Arduino Uno (SCK = 13 and SDA = 11)

// Create display;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void ClearScreen() {
  tft.fillScreen(0x0000); // Fill screen with black
}

void SetupDisplay() {
  tft.begin();
 //myRTC.setDS1302Time(0, 0, 1, 6, 9, 2, 2019); ////
  
  ClearScreen();
}

void ShowTemperature(bool selected, float value) {
  int background;

  if (selected)
    background = 0xF912;
  else
    background = 0x0000;

  tft.fillRect(10, 32, 220 , 80, background);
  tft.drawRoundRect(10, 32, 220, 80, 5, 0x0138);

  tft.setCursor(97, 40);
  tft.setTextColor(0xF3C3);
  tft.setTextSize(2);
  tft.println("TEMP");

  tft.setCursor(15, 68);
  tft.setTextColor(0x07E0);
  tft.setTextSize(1);

  tft.print((char)167);  tft.println("C");

  tft.setCursor(99, 80);
  tft.setTextColor(0xFFFF, background);
  tft.setTextSize(3);
  tft.print(value, 0);
}

void ShowHeartRate(bool selected, float heartRate, float SpO2) {
  int background;

  if (selected)
    background = 0xF912;
  else
    background = 0x0000;

  tft.fillRect(10, 214, 105 , 80, background);
  tft.fillRect(125, 214, 105 , 80, background);

  tft.drawRoundRect(10, 214, 105, 80, 5, 0x0138);
  tft.drawRoundRect(125, 214, 105, 80, 5, 0x0138);

  tft.setCursor(51, 218);
  tft.setTextColor(0xF800);
  tft.setTextSize(2);
  tft.println("PR");

  tft.setCursor(93, 250);
  tft.setTextColor(0x07E0);
  tft.setTextSize(0);
  tft.println("bpm");

  //SPO2
  tft.setCursor(155, 218);
  tft.setTextColor(0x48FE);
  tft.setTextSize(2);
  tft.println("SPO2");

  tft.setCursor(218, 250);
  tft.setTextColor(0x07E0);
  tft.setTextSize(0);
  tft.println("%");

  tft.setCursor(50, 250);
  tft.setTextColor(0xFFE0, 0x0000);
  tft.setTextSize(3);
  tft.print(heartRate, 0);

  tft.setCursor(164, 250);
  tft.setTextColor(0xFFE0, 0x0000);
  tft.setTextSize(3);
  tft.print(SpO2, 0);
}

void ShowPressure(bool selected, float value) {
  int background;

  if (selected)
    background = 0xF912;
  else
    background = 0x0000;

  tft.fillRect(10, 123, 220, 80, background);
  tft.drawRoundRect(10, 123, 220, 80, 5, 0x0138);//C2

  //NIBP
  tft.setCursor(95, 130);
  tft.setTextColor(0x1C5E);
  tft.setTextSize(2);
  tft.println("NIBP");

  tft.setCursor(15, 159);
  tft.setTextColor(0x07E0);
  tft.setTextSize(1);
  tft.println("mmHg");

  /*
      tft.setCursor(99, 80);
      tft.setTextColor(0xFFFF, background);
      tft.setTextSize(3);
      tft.print(value, 0); */
}

 void ClockDate() ////  
 {  
  tft.setCursor(45,0);  
  tft.setTextColor(0xFFFF,0x0000);
  tft.setTextSize(0);  // 
  tft.print(myRTC.dayofmonth);  // 
  tft.print("/");
  tft.print(myRTC.month);
  tft.print("/");
  tft.print(myRTC.year);  
  
  tft.setCursor(190,0);
  tft.setTextColor(0xFFFF,0x0000);
  tft.setTextSize(0);  // 
  tft.print(myRTC.hours);
  tft.print(":");
  tft.print(myRTC.minutes);
  tft.print(":");
  tft.setCursor(225,0);
  tft.println(myRTC.seconds);

 Serial.print("Current Date / Time: ");
 Serial.print(myRTC.dayofmonth); //You can switch between day and month if you're using American system
 Serial.print("/");
 Serial.print(myRTC.month);
 Serial.print("/");
 Serial.print(myRTC.year);
 Serial.print(" ");
 Serial.print(myRTC.hours);
 Serial.print(":");
 Serial.print(myRTC.minutes);
 Serial.print(":");
 Serial.println(myRTC.seconds);  
 }

 
void DisplayCaptureScreen() {
  ClearScreen();
  
  tft.setCursor(12, 0);  // Set position (x,y)
  tft.setTextColor(0xFFFF);  // Set color of text. First is the color of text and after is color of background
  tft.setTextSize(0);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
  tft.println("DATE:");  // Print a text or value
  tft.setCursor(160,0);
  tft.println("HRS:");

  tft.setCursor(17, 10);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(1);
  tft.println("UNIVERSIDAD NACIONAL DE INGENIERIA");

  tft.setCursor(46, 20);
  tft.setTextColor(0x07FF);
  tft.setTextSize(1);
  tft.println("PROTOTYPE OF VITAL SIGNS");

  tft.setCursor(10, 310);
  tft.setTextColor(0xFFE0);
  tft.setTextSize(1);
  tft.println("Patient:");

  ShowTemperature(true, 0);
  ShowPressure(false, 0);
  ShowHeartRate(false, 0, 0);
}

void SelectPatient(Patient patient) 
{
  int posY = 40 + (20 * patient.position);
  
  tft.fillRect(0, posY, 240 , 20, 0xF912);
  
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.setCursor(5, posY);
  tft.println(patient.name);
}

void UnselectPatient(Patient patient)
{
  int posY = 40 + (20 * patient.position);
  
  tft.fillRect(0, posY, 240 , 20, 0x0000);
  
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.setCursor(5, posY);
  tft.println(patient.name);
}

#endif
