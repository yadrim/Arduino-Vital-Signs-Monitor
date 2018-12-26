#include "HeartRateSensor.h"
#include "TemperatureSensor.h"

#include <Adafruit_GFX.h>    // Include core graphics library
#include <Adafruit_ILI9341.h> // Include Adafruit_ILI9341 library to drive the display

#define bselect 4
#define benter 5
#define bmenu 6
#define bmemory 7

enum ActionTypeEnum {
  NONE = 0,
  ENTER = 1,
  MENU = 2,
  SELECT = 3,
  MEMORY = 4
};

enum DisplayEnum {
  CAPTURE = 1,
  PATIENTS = 2,
  DATALIST = 3
};

int selectedSensor;

HeartRateSensor heartRate;
TemperatureSensor temperature;
SensorBase currentSensor;

int action;
int menu;
bool resetMenu;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  SetupDisplay();
  
  heartRate.Setup();
  temperature.Setup();

  //si el sensor no logro iniciar correctamente, no hacer nada
  if(!heartRate.initialized)
    while(1);

  if(!temperature.initialized)
    while(1);


  //set capture screen as default
  menu = CAPTURE;
  selectedSensor = TEMPERATURE;
  
  //dibuja la pantalla principal
  DisplayCaptureScreen();
  DisplaySelectedSensor(selectedSensor);
}

void loop() {
  action = NONE;
  
  if (digitalRead(bselect)== HIGH)
    action = SELECT;
  if (digitalRead(benter)== HIGH)
    action = ENTER;
    /*
  if (digitalRead(bmenu)== HIGH)
    action = MENU;
  if (digitalRead(bmemory)== HIGH)
    action = MEMORY;

    */
  
  if(action != NONE)
    UpdateDisplay();

    /*

  switch(menu){
    case CAPTURE:
      UpdateCaptureSensor();
      break;
  }

  */
}

void UpdateDisplay() {
  if(action == MENU)
  {
    if(menu == DATALIST)
      menu = CAPTURE;
    else
      menu++;
    
    resetMenu = true;
  }
  
  switch(menu)
  {
    case CAPTURE:
      UpdateCapture();
      break;
      
    case PATIENTS:
      UpdatePatients();
      break;
      
    case DATALIST:
      break;
  }

  resetMenu = false;
}

// Capture functions

void UpdateCapture() {
  if(resetMenu)
    DisplayCaptureScreen();
  
  switch(action)
  {
    case SELECT:      
      //temperature.active = false;
      
      if(selectedSensor == SPO)
         selectedSensor = TEMPERATURE;
      else
         selectedSensor++;

      DisplaySelectedSensor(selectedSensor);
      break;
      
    case ENTER:
      if(selectedSensor == TEMPERATURE)
        temperature.active = true;
      break;
    
    case MEMORY:
      break;
  }

  /*
  switch(selectedSensor)
  {
    case TEMPERATURE:
      currentSensor = temperature;

    case HEARTRATE:
      currentSensor = heartRate;
  }
  */
}

void UpdateCaptureSensor(){
  switch(selectedSensor)
  {
    case TEMPERATURE:
      if(temperature.active)
      {
        temperature.Update();
        if(temperature.canDisplay)
          temperature.Display();
      }
  }

  /*
  if(currentSensor.active) {
    Serial.print("updating sensor");
    currentSensor.Update();
    
    if(currentSensor.canDisplay){
      currentSensor.Display();
    }
  } 
  */ 
}

// Patients functions

void UpdatePatients() {
  if(resetMenu)
    DisplayPatientsScreen();
  
  switch(action)
  {
    case SELECT:
      break;
      
    case ENTER:
      break;
  }
}

void DisplayPatientsScreen() {
  // draw code here
}
