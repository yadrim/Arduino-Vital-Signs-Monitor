#ifndef __SENSORBASE_H_INCLUDED__
#define __SENSORBASE_H_INCLUDED__

#include "SensorData.h"
#include "SensorDisplay.h"

#define REPORTING_PERIOD_MS 300

/**
 * Clase base para los sensores, aqui definimos el comportamiento y variables en comun
 */
class SensorBase
{  


protected:
  uint32_t lastDisplayUpdate; // ultima actualizacion de pantalla

public:
  
  SensorBase()
  {
  }
  
  bool initialized; // indica si se inicializo bien
  bool canDisplay; // indica si puede mostrar datos en pantalla
  bool active;
  bool selected;

  SensorType type; // tipo del sensor

  virtual void Setup() {}
  
  virtual void Update() {}
  
  virtual void Display() {}
  
  virtual void DisplayPlace() {}
  
  void Capture() {
	active = !active;
    DisplayPlace();	
  }
  
  void Select() {
	selected = true;
	DisplayPlace();
  }
  
  void Unselect() {
	active = false;
	selected = false;
	DisplayPlace();
  }
  
};

#endif
