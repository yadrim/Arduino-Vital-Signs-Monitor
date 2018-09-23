#ifndef __TEMPERATURESENSOR_H_INCLUDED__
#define __TEMPERATURESENSOR_H_INCLUDED__

#include "SensorDisplay.h"
#include "SensorData.h"
#include "SensorBase.h"

#include <Adafruit_MLX90614.h>

class TemperatureSensor : public SensorBase
{

  Adafruit_MLX90614 sensor = Adafruit_MLX90614();  // sensor de temperatura
  
public:

  TemperatureSensor() {
    type = TEMPERATURE; // establecemos el tipo de nuestro sensor, en este caso TEMPERATURE
  }

  void Setup(){   
    initialized = sensor.begin(); // se inicializa el sensor
  }

  void Update() {
    double ambientTemp;
    double objectTemp;

    ambientTemp = sensor.readAmbientTempC();
    objectTemp = sensor.readObjectTempC();

    if(ambientTemp != objectTemp && (ambientTemp - objectTemp) > 5) {      
      canDisplay = (millis() - lastDisplayUpdate) > REPORTING_PERIOD_MS; // verificamos si ya paso el intervalo de actualizacion 
    }
  }

  void Display() {

    Serial.print("Temperature:");    
    Serial.print(sensor.readObjectTempC(), 0);

    tft.setCursor(50,250);
    tft.setTextColor(0xFFE0, 0x0000);
    tft.setTextSize(3);  
    tft.print(sensor.readObjectTempC(), 0);
       
    lastDisplayUpdate = millis(); // establecemos la ultima actualizacion de pantalla
  }
  
};


#endif
