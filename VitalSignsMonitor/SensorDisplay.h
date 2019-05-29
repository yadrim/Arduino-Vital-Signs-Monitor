#ifndef __SENSORDISPLAY_H_INCLUDED__
#define __SENSORDISPLAY_H_INCLUDED__

#include <Adafruit_GFX.h>    // Include core graphics library
#include <Adafruit_ILI9341.h> // Include Adafruit_ILI9341 library to drive the display
#include <Wire.h>
#include <virtuabotixRTC.h> 
#include "SensorData.h"


virtuabotixRTC RTC(2, 3, 4); //(clk, DAT, RST)

/**
     DECLARE PIN FOR DISPLAY
*/
#define TFT_DC 9
#define TFT_RST 8  // You can also connect this to the Arduino reset in which case, set this #define pin to -1!
#define TFT_CS 53
// The rest of the pins are pre-selected as the default hardware SPI for Arduino Uno (SCK = 13 and SDA = 11)


// Create display;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

char Date[15];/////
char HRS[15];


void ClearScreen() {
  tft.fillScreen(0x0000); // Fill screen with black
}


void SetupDisplay() {
  tft.begin();
 //myRTC.setDS1302Time(50, 30, 02, 0, 24, 4, 2019); // 24/04/2019   02:30:50  
  ClearScreen();
}

void ShowTemperature(bool selected, float value) {
  int background;

  if (selected)
    background = 0xF912;
  else
    background = 0x0000;

  tft.fillRoundRect(10, 32, 220 , 80, 5, background);
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

  tft.fillRoundRect(10, 214, 105 , 80, 5, background);
  tft.fillRoundRect(125, 214, 105 , 80, 5, background);

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
  tft.print("SPO2");

  tft.setCursor(218, 250);
  tft.setTextColor(0x07E0);
  tft.setTextSize(0);
  tft.println("%");
  
  tft.setCursor(50, 250);
  tft.setTextColor(0xFFE0, background);
  tft.setTextSize(3);
  tft.print(heartRate, 0);

  tft.setCursor(164, 250);
  tft.setTextColor(0xFFFF, background);
  tft.setTextSize(3);
  tft.print(SpO2, 0);
}

void ShowPressure(bool selected, float value) {
  int background;

  if (selected)
    background = 0xF912;
  else
    background = 0x0000;

  tft.fillRoundRect(10, 123, 220, 80, 5, background);
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

  tft.setCursor(12, 0);  // Set position (x,y)
  tft.setTextColor(0xFFFF);  // Set color of text. First is the color of text and after is color of background
  tft.setTextSize(0);  // Set text size. Goes from 0 (the smallest) to 20 (very big)
  tft.println("DATE:");  // Print a text or value
  tft.setCursor(160,0);
  tft.println("HRS:");
  
  tft.setTextColor(0xFFFF,0x0000); 
  tft.setCursor(45,0);  
  tft.setTextSize(0); 

  sprintf(Date,"%02d/%02d/%04d\0", RTC.dayofmonth, RTC.month, RTC.year);
  tft.print(Date);
   
  tft.setTextColor(0xFFFF,0x0000);
  tft.setCursor(186,0);
  tft.setTextSize(0);  // 

  sprintf(HRS,"%02d:%02d:%02d\0", RTC.hours, RTC.minutes, RTC.seconds);
  tft.print(HRS);   

 }

 
void DisplayCaptureScreen() {
  ClearScreen();
  ClockDate();
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

  /*tft.fillRoundRect(98, 300, 50, 20, 5, 0x0138);
  tft.setCursor(125, 308);
  tft.setTextColor(0xFFE0,0x0138);
  tft.setTextSize(1);
  tft.println("BL");*/
 
  //tft.drawRoundRect(98, 300, 20, 20, 5, 0x0138);

  ShowTemperature(true, 0);
  ShowPressure(false, 0);
  ShowHeartRate(false, 0, 0);
}

void  SelectPatient(Patient patient) 
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

void DisplayPatientDataScreen(){
   ClearScreen();  //Limpiar pantalla

    tft.setCursor(60, 10);
    tft.setTextColor(0x01CE);  
    tft.setTextSize(2);
    tft.println("Register: ");


    tft.drawLine(0, 50, 240, 50, 0x00AA); 
    tft.drawLine(0, 104, 240, 104, 0x00AA); 
    tft.drawLine(0, 158, 240, 158, 0x00AA); 
    tft.drawLine(0, 212, 240, 212, 0x00AA); 
    tft.drawLine(0, 266, 240, 266, 0x00AA); 
//////////////////////////////////////////
    tft.drawLine(0, 65, 240, 65, 0x00AA);
    tft.setCursor(10, 55);
    tft.setTextColor(0x00AA);  
    tft.setTextSize(1);
    tft.print("=> ");
    tft.setCursor(180, 55);
  /*  tft.setTextColor(0x00AA);  
    tft.setTextSize(1);
    tft.print("24:32:01");*/

    tft.setCursor(10,75);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("TEMP:");
    tft.setCursor(45,75);

    tft.setCursor(10,85);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("NIBP:");
    
    tft.setCursor(120,75);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("PR:");
    tft.setCursor(160,75);
 
    tft.setCursor(120,85);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("SPO2:");

///////////////////////////////////////
   
  //  tft.drawLine(0, 104, 240, 104, 0x00AA); 
tft.drawLine(0, 119, 240, 119, 0x00AA);    
    
    tft.setCursor(10, 109);
    tft.setTextColor(0x00AA);  
    tft.setTextSize(1);
    tft.print("=> ");
    //tft.setCursor(180, 109);


    tft.setCursor(10,129);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("TEMP:");

    tft.setCursor(10,139);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("NIBP:");
    
    tft.setCursor(120,129);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("PR:");

    tft.setCursor(120,139);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("SPO2:");
////////////////////////////
   // tft.drawLine(0, 158, 240, 158, 0x00AA); 
    tft.drawLine(0, 173, 240, 173, 0x00AA);
    
    tft.setCursor(10, 163);
    tft.setTextColor(0x00AA);  
    tft.setTextSize(1);
    tft.print("=> ");
   
    tft.setCursor(10,183);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("TEMP:");

    tft.setCursor(10,193);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("NIBP:");
    
    tft.setCursor(120,183);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("PR:");

    tft.setCursor(120,193);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("SPO2:");

 /////////////////////////////////////
    //    tft.drawLine(0, 266, 240, 266, 0x00AA);
    tft.drawLine(0, 227, 240, 227, 0x00AA);
    
    tft.setCursor(10, 217);
    tft.setTextColor(0x00AA);  
    tft.setTextSize(1);
    tft.print("=> ");


    tft.setCursor(10,237);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("TEMP:");

    tft.setCursor(10,247);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("NIBP:");
    
    tft.setCursor(120,237);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("PR:");

    tft.setCursor(120,247);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("SPO2:");
    /////////////////////////////////////
  
   // tft.drawLine(0, 266, 240, 266, 0x00AA);
       tft.drawLine(0, 281, 240, 281, 0x00AA);
    
    tft.setCursor(10, 271);
    tft.setTextColor(0x00AA);  
    tft.setTextSize(1);
    tft.print("=> ");

    tft.setCursor(10,291);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("TEMP:");

    tft.setCursor(10,301);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("NIBP:");
    
    tft.setCursor(120,291);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("PR:");

    tft.setCursor(120,301);
    tft.setTextColor(0xFFFF);
    tft.setTextSize(1);
    tft.print("SPO2:");
 }
   

    /*  
        tft.setCursor(10,35);
        tft.setTextColor(0x00AA);
        tft.setTextSize(1);
        tft.print(storage.currentPatient.name);
do {
  
    tft.setTextColor(0x00AA);
    tft.setTextSize(1);
    tft.setCursor(25, 55);
    tft.print(storage.currentData.date.day);
    tft.print("/");
    tft.print(storage.currentData.date.month);
    tft.print("/");
    tft.print(storage.currentData.date.month);
    tft.setCursor(180, 55); // h
    

    tft.setCursor(25, 109);
    tft.print(storage.currentData.date.day);
    tft.print("/");
    tft.print(storage.currentData.date.month);
    tft.print("/");
    tft.print(storage.currentData.date.month);
    tft.setCursor(180, 109);

    tft.setCursor(25, 163);
    tft.print(storage.currentData.date.day);
    tft.print("/");
    tft.print(storage.currentData.date.month);
    tft.print("/");
    tft.print(storage.currentData.date.month);
    tft.setCursor(180, 163);

     tft.setCursor(20, 217);
    tft.print(storage.currentData.date.day);
    tft.print("/");
    tft.print(storage.currentData.date.month);
    tft.print("/");
    tft.print(storage.currentData.date.month);
    tft.setCursor(180, 217);

    tft.setCursor(25, 271);
    tft.print(storage.currentData.date.day);
    tft.print("/");
    tft.print(storage.currentData.date.month);
    tft.print("/");
    tft.print(storage.currentData.date.month);
    tft.setCursor(180, 271);

        tft.setTextColor(0xFFE0);  
        tft.setTextSize(1);
        tft.setCursor(45, 75);
            Serial.print(storage.currentData.data1);
        tft.print(storage.currentData.data1);
        tft.setCursor(60,75);
        tft.print("c");
                
        tft.setCursor(45, 85);
        tft.print(storage.currentData.data2);
        tft.setCursor(60,85);
        tft.print("mmHg");
        
        tft.setCursor(155, 75);
        tft.print(storage.currentData.data3);
        tft.setCursor(175,75);
        tft.print("bpm");
        
        tft.setCursor(155, 85);
        tft.print(storage.currentData.data4);
        tft.setCursor(175,85);
        tft.print("%");

        tft.setTextColor(0xFFE0);  
        tft.setTextSize(1);
        tft.setCursor(45, 129);
        tft.print(storage.currentData.data1);
        tft.setCursor(60,129);
        tft.print("c");        
        tft.setCursor(45, 139);
        tft.print(storage.currentData.data2);
        tft.setCursor(60, 139);
        tft.print("mmHg");
        tft.setCursor(155, 129);
        tft.print(storage.currentData.data3);
        tft.setCursor(175, 129);
        tft.print("bpm");
        tft.setCursor(155, 139);
        tft.print(storage.currentData.data4);
        tft.setCursor(175,139);
        tft.print("%");

        tft.setTextColor(0xFFE0);  
        tft.setTextSize(1);
        tft.setCursor(45, 183);
        tft.print(storage.currentData.data1);
        tft.setCursor(60,183);
        tft.print("c");        
        tft.setCursor(45, 193);
        tft.print(storage.currentData.data2);
        tft.setCursor(60, 193);
        tft.print("mmHg");
        tft.setCursor(155, 183);
        tft.print(storage.currentData.data3);
        tft.setCursor(175, 183);
        tft.print("bpm");
        tft.setCursor(155, 193);
        tft.print(storage.currentData.data4);
        tft.setCursor(175,193);
        tft.print("%");

        tft.setCursor(45, 237);
        tft.print(storage.currentData.data1);
        tft.setCursor(60,237);
        tft.print("c");        
        tft.setCursor(45, 247);
        tft.print(storage.currentData.data2);
        tft.setCursor(60, 247);
        tft.print("mmHg");
        tft.setCursor(155, 237);
        tft.print(storage.currentData.data3);
        tft.setCursor(175, 237);
        tft.print("bpm");
        tft.setCursor(155, 247);
        tft.print(storage.currentData.data4);
        tft.setCursor(175,247);
        tft.print("%");

        tft.setCursor(45, 291);
        tft.print(storage.currentData.data1);
        tft.setCursor(60,291);
        tft.print("c");        
        tft.setCursor(45, 301);
        tft.print(storage.currentData.data2);
        tft.setCursor(60, 301);
        tft.print("mmHg");
        tft.setCursor(155, 291);
        tft.print(storage.currentData.data3);
        tft.setCursor(175, 291);
        tft.print("bpm");
        tft.setCursor(155, 301);
        tft.print(storage.currentData.data4);
        tft.setCursor(175,301);
        tft.print("%");
*/
void  DisplayConnectionScreen()
   {
     ClearScreen();
     tft.setCursor(70,10);
     tft.setTextColor(0x01CE);
     tft.setTextSize(2);
     tft.println("Settings");

    tft.setTextColor(0xFFFF);
    tft.setTextSize(2);    
    tft.setCursor(10, 40);
    tft.println("Bluetooth");

    tft.setTextColor(0xFFFF);
    tft.setTextSize(2);    
    tft.setCursor(190, 40);
    tft.println("OFF");
    
    /*tft.setTextColor(0xFFFF);
    tft.setTextSize(2);    
    tft.setCursor(190, 40);
    tft.println("ON");*/
   }
    
   
    



#endif
