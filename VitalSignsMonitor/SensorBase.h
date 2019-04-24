#ifndef __SENSORBASE_H_INCLUDED__
#define __SENSORBASE_H_INCLUDED__

#include "SensorData.h"
#include "SensorDisplay.h"

#define REPORTING_PERIOD_MS 500  // intervalo de tiempo en milsegundos para actualizar la pantalla, con esto evitamos parpadeos.

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
  
  bool initialized;           // indica si se inicializo bien
  bool canDisplay;            // indica si puede mostrar datos en pantalla
  bool active;                // indica si el sensor esta capturando datos
  bool selected;              // indica si esta visualmente seleccionado

  SensorType type;            // tipo del sensor

  virtual void Setup() {}     // funcion para configurar el sensor, se debe sobreescribir para cada implementacion
  
  virtual void Update() {}    // funcion para actualizar el sensor en cada ciclo del loop, se debe sobreescribir para cada implementacion
  
  virtual void Display() {}   // funcion para mostrar la informacion capturada por el sensor, se debe sobreescribir para cada implementacion

  // funcion para comenzar o detener la captura de datos
  void Capture() {            
	  active = !active;
    if(active)
      Setup();
    Display();	
  }

  // funcion para seleccionar el sensor
  void Select() {
  	selected = true;
  	Display();
  }

  // funcion para deseleccionar el sensor, si esta capturando datos, detiene la operacion y muestra los datos
  void Unselect() {
  	active = false;
  	selected = false;
  	Display();
  }
  
};

#endif
