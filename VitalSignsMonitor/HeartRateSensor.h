
#ifndef __HEARTRATESENSOR_H_INCLUDED__
#define __HEARTRATESENSOR_H_INCLUDED__

#include "SensorData.h"
#include "SensorBase.h"
#include "SensorDisplay.h"
#include "MAX30100_PulseOximeter.h"


/**
 *  Implementacion del sensor de ritmo cardiaco
 *  hereda de SensorBase
 */
class HeartRateSensor : public SensorBase
{

  PulseOximeter sensor; // sensor de ritmo cardiaco
  uint32_t lastBeatDetected;
  
public:

  HeartRateSensor() {
    type = HEARTRATE; // establecemos el tipo de nuestro sensor, en este caso HeartRate
  }

  void Setup(){
    sensor.setOnBeatDetectedCallback((void*) &HeartRateSensor::onBeatDetected); // evento que detecta cuando el sensor obtiene valor
    initialized = sensor.begin(); // se inicializa el sensor
  }

  void Update() {
    sensor.update(); // actualizamos el sensor  
    canDisplay = (millis() - lastDisplayUpdate) > REPORTING_PERIOD_MS; // verificamos si ya paso el intervalo de actualizacion
  }

  void Display() {
    Serial.print("Heart rate:");
    Serial.print(sensor.getHeartRate(), 0);
    Serial.print("bpm / SpO2:");
    Serial.print(sensor.getSpO2());
    Serial.println("%");

    ShowHeartRate(selected, sensor.getHeartRate(), sensor.getSpO2());
       
    lastDisplayUpdate = millis(); // establecemos la ultima actualizacion de pantalla
  }

private:

/**
 *  Sensor detecta un latido
 */
  void onBeatDetected() {
    lastBeatDetected = millis(); 
  }

};

#endif
