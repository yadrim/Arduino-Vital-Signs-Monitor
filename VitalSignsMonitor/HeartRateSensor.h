
#ifndef __HEARTRATESENSOR_H_INCLUDED__
#define __HEARTRATESENSOR_H_INCLUDED__

#include "SensorData.h"
#include "SensorBase.h"
#include "MAX30100_PulseOximeter.h"


/**
 *  Implementacion del sensor de ritmo cardiaco
 *  hereda de SensorBase
 */
class HeartRateSensor : public SensorBase
{

  PulseOximeter sensor; // sensor de ritmo cardiaco
  int dataIndex; // posicion del arreglo de datos
  
  uint32_t lastBeatDetected;
  
public:

  HeartRateSensor() {
    type = HEARTRATE; // establecemos el tipo de nuestro sensor, en este caso HeartRate
  }

  void Setup(){
    dataIndex= 0; //inicializamos la posicion del arreglo de datos en 0
    
    sensor.setOnBeatDetectedCallback((void*) &HeartRateSensor::onBeatDetected); // evento que detecta cuando el sensor obtiene valor
    initialized = sensor.begin(); // se inicializa el sensor
  }

  void Update() {
    sensor.update(); // actualizamos el sensor  
    canDisplay = (millis() - lastDisplayUpdate) > REPORTING_PERIOD_MS; // verificamos si ya paso el intervalo de actualizacion
  }

  void Display() {

    double heartRate = 0;

    Serial.print("Heart rate:");
    Serial.print(sensor.getHeartRate(), 0);
    Serial.print("bpm / SpO2:");
    Serial.print(sensor.getSpO2());
    Serial.println("%");

    if(millis() - lastBeatDetected < 5000) {
      heartRate = sensor.getHeartRate();

      tft.setCursor(50,250);
      tft.setTextColor(0xFFE0, 0x0000);
      tft.setTextSize(3);  
      tft.print(heartRate, 0);
     
      tft.setCursor(164,250);
      tft.setTextColor(0xFFE0, 0x0000);
      tft.setTextSize(3);
      tft.print(sensor.getSpO2());
    }
    else {
      // borrar el dato de la pantalla aca
    }
       
    lastDisplayUpdate = millis(); // establecemos la ultima actualizacion de pantalla
  }

private:

/**
 *  Sensor detecta un latido
 */
  void onBeatDetected() {

    lastBeatDetected = millis();

    SensorData beatData; // declaramos un dato de sensor y establecemos sus valores
    beatData.time = millis();
    beatData.data = sensor.getHeartRate();
    beatData.type = type;

    //data.push(beatData);    
  }

};

#endif
