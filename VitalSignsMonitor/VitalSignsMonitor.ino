#include "HeartRateSensor.h"
#include "TemperatureSensor.h"

#include <Adafruit_GFX.h>    // Include core graphics library
#include <Adafruit_ILI9341.h> // Include Adafruit_ILI9341 library to drive the display

HeartRateSensor heartRate;
TemperatureSensor temperature;

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

  //dibuja la pantalla principal
  ShowMainScreen();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  heartRate.Update();
  temperature.Update();

  if(heartRate.canDisplay) {
    heartRate.Display();
  }

  if(temperature.canDisplay){
    temperature.Display();
  }
}
