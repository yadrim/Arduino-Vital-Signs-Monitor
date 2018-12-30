#ifndef __SENSORBASE_H_INCLUDED__
#define __SENSORBASE_H_INCLUDED__

#include "SensorData.h"
#include "SensorDisplay.h"

#define REPORTING_PERIOD_MS 500

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
  
  void Capture() {
	  active = !active;
    Display();	
  }
  
  void Select() {
  	selected = true;
  	Display();
  }
  
  void Unselect() {
  	active = false;
  	selected = false;
  	Display();
  }
  
};

#endif
