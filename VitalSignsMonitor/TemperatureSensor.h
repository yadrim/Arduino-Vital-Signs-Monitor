#ifndef __TEMPERATURESENSOR_H_INCLUDED__
#define __TEMPERATURESENSOR_H_INCLUDED__

#include "SensorDisplay.h"
#include "SensorData.h"
#include "SensorBase.h"

#include <Adafruit_MLX90614.h>

class TemperatureSensor : public SensorBase
{

  Adafruit_MLX90614 sensor = Adafruit_MLX90614();  // sensor de temperatura
  double objectTemp;
  
public:

  TemperatureSensor() {
    type = TEMPERATURE; // establecemos el tipo de nuestro sensor, en este caso TEMPERATURE
  }

  void Setup(){   
    initialized = sensor.begin(); // se inicializa el sensor
  }

  void Update() {
    double ambientTemp;

    ambientTemp = sensor.readAmbientTempC();
    objectTemp = sensor.readObjectTempC();

    canDisplay = (millis() - lastDisplayUpdate) > 500; // verificamos si ya paso el intervalo de actualizacion 
  }

  void Display() {

    Serial.print("Temperature:");    
    Serial.print(objectTemp, 0);

    ShowTemperature(selected, objectTemp);        
       
    lastDisplayUpdate = millis(); // establecemos la ultima actualizacion de pantalla
  }

  double GetValue() 
  {
    return objectTemp;
  }
  
};


#endif
