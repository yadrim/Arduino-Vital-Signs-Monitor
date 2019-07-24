#ifndef __PRESSURESENSOR_H_INCLUDED__
#define __PRESSURESENSOR_H_INCLUDED__

#include "SensorDisplay.h"
#include "SensorData.h"
#include "SensorBase.h"

class PressureSensor : public SensorBase
{

  double pressure;
  
public:

  PressureSensor() {
    type = PRESSURE; // establecemos el tipo de nuestro sensor, en este caso TEMPERATURE
  }

  void Setup(){   
    //initialized = sensor.begin(); // se inicializa el sensor
  }

  void Update() {

    canDisplay = (millis() - lastDisplayUpdate) > REPORTING_PERIOD_MS; // verificamos si ya paso el intervalo de actualizacion 
  }

  void Display() {    
    ShowPressure(selected, 0);
    lastDisplayUpdate = millis(); // establecemos la ultima actualizacion de pantalla
  }

  double GetValue()
  {
    return pressure;
  }
  
};


#endif
