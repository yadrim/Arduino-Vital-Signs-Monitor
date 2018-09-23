#ifndef __SENSORDISPLAY_H_INCLUDED__
#define __SENSORDISPLAY_H_INCLUDED__

#include <Adafruit_GFX.h>    // Include core graphics library
#include <Adafruit_ILI9341.h> // Include Adafruit_ILI9341 library to drive the display
#include <Wire.h>


/**
 *   DECLARE PIN FOR DISPLAY
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
  ClearScreen();
}

// Muestra la pantalla principal
void ShowMainScreen() {
  tft.setCursor(12, 0);  // Set position (x,y)
  tft.setTextColor(0xFFFF);  // Set color of text. First is the color of text and after is color of background
  tft.setTextSize(0);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
  tft.println("DATE:");  // Print a text or value
  tft.setCursor(195,0);
  tft.println("HRS:");

  tft.setCursor(17,10);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(1);
  tft.println("UNIVERSIDAD NACIONAL DE INGENIERIA");

  tft.setCursor(46,20);
  tft.setTextColor(0x07FF);
  tft.setTextSize(1);
  tft.println("PROTOTYPE OF VITAL SIGNS");

  tft.setCursor(10, 310);
  tft.setTextColor(0xFFE0);
  tft.setTextSize(1);
  tft.println("Patient:");

//Draw lines: 
  
  tft.drawLine(0, 33, 240, 33, 0x00BA);  // Draw line (x0,y0,x1,y1,color)
  tft.drawLine(0, 123, 240, 123, 0x00BA);  // Draw line (x0,y0,x1,y1,color)
  tft.drawLine(0, 214, 240, 214, 0x00BA);
  tft.drawLine(120, 214, 120, 302, 0x00BA);
  tft.drawLine(0, 302, 240, 302, 0x00BA);
  
 /* tft.drawLine(0, 0, 240, 0,0x00BA);
  tft.drawLine(0, 319, 240, 319,0x00BA);
  tft.drawLine(0, 0, 0, 320,0x00BA);
  tft.drawLine(239, 0, 239, 320,0x00BA);
  
 
   /*tft.drawRoundRect(10, 32, 220, 80, 5, 0xF3C3);//C1
  tft.drawRoundRect(10, 123, 220, 80, 5, 0x1C5E);//C2
  tft.drawRoundRect(10, 214, 105, 80, 5, 0xF800);
  tft.drawRoundRect(125, 214, 105, 80, 5, 0x48FE);*/ 


 /////////////////////////Etiquetas///////////////////////////////////
  //Temp 
  tft.setCursor(97, 40);
  tft.setTextColor(0xF3C3);
  tft.setTextSize(2);
  tft.println("TEMP");
  
  tft.setCursor(15, 68);
  tft.setTextColor(0x07E0);
  tft.setTextSize(1);
  
  tft.print((char)167);  tft.println("C");
  
  //NIBP
  tft.setCursor(95, 130);
  tft.setTextColor(0x1C5E);
  tft.setTextSize(2);
  tft.println("NIBP");
  
  tft.setCursor(15, 159);
  tft.setTextColor(0x07E0);
  tft.setTextSize(1);
  tft.println("mmHg");

 //PR
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
}


#endif
