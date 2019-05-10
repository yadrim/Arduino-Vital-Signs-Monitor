
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

  float heartRate;
  float SpO2;
  
public:

  HeartRateSensor() {
    type = HEARTRATE; // establecemos el tipo de nuestro sensor, en este caso HeartRate
  }

  void Setup(){
    //sensor.setOnBeatDetectedCallback((void*) &HeartRateSensor::onBeatDetected); // evento que detecta cuando el sensor obtiene valor
    initialized = sensor.begin(); // se inicializa el sensor
  }

  void Update() {
    sensor.update(); // actualizamos el sensor  

    heartRate = sensor.getHeartRate();
    SpO2 = sensor.getSpO2();
    
    canDisplay = (millis() - lastDisplayUpdate) > REPORTING_PERIOD_MS; // verificamos si ya paso el intervalo de actualizacion
  }

  void Display() {
    Serial.print(" Heart rate: ");
    Serial.print(heartRate, 0);
    Serial.print("bpm / SpO2:");
    Serial.print(heartRate);
    Serial.println("%");

    ShowHeartRate(selected, heartRate, SpO2);
       
    lastDisplayUpdate = millis(); // establecemos la ultima actualizacion de pantalla
  }

  double GetValue() 
  {
    return heartRate;
  }

  double GetSPO2() 
  {
    return SpO2;
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
