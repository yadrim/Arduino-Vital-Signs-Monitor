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

HeartRateSensor heartRate;
TemperatureSensor temperature;

SensorBase *currentSensor;
int selectedSensor;

int action;
int menu;
bool resetMenu;
uint32_t lastActionTime;

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
  currentSensor= &temperature;
  lastActionTime = 0;
  
  //dibuja la pantalla principal
  DisplayCaptureScreen();
}

void loop() {
  action = NONE;
  
  if (digitalRead(bselect)== HIGH)
    action = SELECT;
  if (digitalRead(benter)== HIGH)
    action = ENTER;
  if (digitalRead(bmenu)== HIGH)
    action = MENU;
  if (digitalRead(bmemory)== HIGH)
    action = MEMORY;
  
  if(action != NONE)
    UpdateDisplay();

  switch(menu){
    case CAPTURE:
      UpdateCaptureSensor();
      break;
  }
}

void UpdateDisplay() {
  if ((millis() - lastActionTime) < 1000)
    return;

  Serial.println();
  Serial.print("Action:");
  Serial.print(action);
  
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
  lastActionTime = millis();
}

// Capture functions

void UpdateCapture() {
  if(resetMenu)
    DisplayCaptureScreen();
  
  switch(action)
  {
    case SELECT:      
      Serial.print("Current Sensor:");
      Serial.print(currentSensor->type);
      
	    currentSensor->Unselect();
	  
      if(selectedSensor == SPO)
         selectedSensor = TEMPERATURE;
      else
         selectedSensor++;
	 
  	  switch(selectedSensor)
  	  {
    		case TEMPERATURE:
    		  currentSensor = &temperature;
          break;
    
    		default:
    		  currentSensor = &heartRate;
          break;
  	  }
    
  	  currentSensor->Select();

      Serial.print("Change to Sensor:");
      Serial.print(currentSensor->type);

      Serial.print("Selected Sensor:");
      Serial.print(selectedSensor);
      break;
      
    case ENTER:
      currentSensor->Capture();
      Serial.print("Sensor Active:");
      Serial.print(currentSensor->active);
      break;
    
    case MEMORY:
      break;
  }
}

void UpdateCaptureSensor(){
  if(currentSensor->active) {
    currentSensor->Update();
    
    if(currentSensor->canDisplay){
      currentSensor->Display();
    }
  } 
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
