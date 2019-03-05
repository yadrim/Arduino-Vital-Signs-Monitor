#include "HeartRateSensor.h"
#include "TemperatureSensor.h"
#include "PressureSensor.h"

#include <Adafruit_GFX.h>    // Include core graphics library
#include <Adafruit_ILI9341.h> // Include Adafruit_ILI9341 library to drive the display

#define bselect 6    // PIN para el boton para moverse entre items
#define benter 5     // PIN para el boton de Activar/Seleccionar
#define bmenu 12      // PIN Para el boton de cambiar entre pantallas
#define bmemory 7    // PIN para el boton de guardar los datos 

// Enumeracion de los botones que puede presionar el usuario
enum ActionTypeEnum {
  NONE = 0,
  ENTER = 1,
  MENU = 2,
  SELECT = 3,
  MEMORY = 4
};

// Enumeracion de las pantallas disponibles en el programa
enum DisplayEnum {
  CAPTURE = 1,     // pantalla de captura de signos vitales
  PATIENTS = 2,    // pantalla de listado de pacientes
  DATALIST = 3     // pantalla de listado de datos de un paciente
};

/*
 *  Sensores de signos vitales
 */
HeartRateSensor heartRate;
TemperatureSensor temperature;
PressureSensor pressure;

SensorBase *currentSensor; // puntero al sensor que esta seleccionado 
int selectedSensor;        // tipo de sensor seleccionado


/*
 *  Objetos y variables globales
 */
StorageManager storage;    // objeto que administra el acceso a datos en la EEPROM

int action;                // accion/boton que presiona el usuario
int menu;                  // pantalla actual del programa
bool resetMenu;            // bandera para indicar si se necesita volver a dibujar toda la pantalla 
uint32_t lastActionTime;   // tiempo en que se ejecuto la ultima accion del usuario

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  SetupDisplay();          // inicializar y configurar la pantalla 

  storage.Setup();         // configurar el repositorio de datos

  /*
    temperature.Setup();
    pressure.Setup();
    heartRate.Setup();
  */

  /*
   * Establecer valores por defecto
   * pantalla de captura, sensor de temperatura y puntero a sensor de temperatura
   */
  menu = CAPTURE;
  selectedSensor = TEMPERATURE;
  currentSensor = &temperature;
  lastActionTime = 0;

  DisplayCaptureScreen();  //dibuja la pantalla principal

  Serial.print("Complete Setup");
}

void loop() {
  action = NONE;           // en cada ciclo de la funcion loop, establecemos por defecto que no se ha ejecutado ninguna acccion

  // verificamos y asignamos que boton se esta presionando
  if (digitalRead(bselect) == HIGH)
    action = SELECT;
  if (digitalRead(benter) == HIGH)
    action = ENTER;
  if (digitalRead(bmenu) == HIGH)
    action = MENU;
  if (digitalRead(bmemory) == HIGH)
    action = MEMORY;

  // si se presiono algun boton, procesamos en la funcion UpdateDisplay
  if (action != NONE)
    UpdateDisplay();
    
  // actualizamos siempre en cada ciclo del loop, dependiendo de la pantalla actual
  switch (menu) {
    case CAPTURE:
      UpdateCaptureSensor();
      break;
  }
}

void DisplayPatientsScreen()
{
  ClearScreen();

  tft.setCursor(70, 10);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.println("PATIENTS");

  storage.ReadPatient();

  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);

  int posX = 40;

  do {
    tft.setCursor(5, posX);
    tft.println(storage.currentPatient.name);

    posX += 20;
  } while(storage.NextPatient());

  storage.ReadPatient();
  SelectPatient(storage.currentPatient);
}


/* 
 *  Funcion para determinar que pantalla va a procesar el boton/comando presionado
 */
void UpdateDisplay() {
  // validacion para evitar procesar un boton multiples veces
  if ((millis() - lastActionTime) < 500)
    return;

  Serial.println();
  Serial.print("Action:");
  Serial.print(action);

  // si se presiono el boton de MENU, actualizamos la variable menu e indicamos que necesita dibujar la nueva pantalla
  if (action == MENU)
  {
    if (menu == DATALIST)
      menu = CAPTURE;
    else
      menu++;

    resetMenu = true;
  }

  // verificamos la pantalla actual y procesamos sobre ella
  switch (menu)
  {
    case CAPTURE:
      UpdateCapture();
      break;

    case PATIENTS:
      UpdatePatients();
      break;

    case DATALIST:
      break;
  }

  // establecemos el tiempo en que fue procesado el boton, para evitar procesarlo multiples veces  
  lastActionTime = millis();
}

// funcion para procesar los botones de la pantalla de CAPTURE
void UpdateCapture() {
  // si resetMenu esta en true, dibujamos toda la pantalla de CAPTURA
  if (resetMenu) {
    DisplayCaptureScreen();
    resetMenu = false;
  }

  // verificamos el tipo de boton presionado
  // SELECT: avanza sobre los sensores disponibles, desactiva el actual y selecciona el siguiente
  // ENTER:  le indica al sensor seleccionado que debe de comenzar a capturar datos
  // MEMORY: selecciona un paciente y guarda en la EEPROM los datos que se han recogido
  switch (action)
  {
    case SELECT:
      Serial.print("Current Sensor:");
      Serial.print(currentSensor->type);

      // desactivamos el sensor actual
      currentSensor->Unselect();             

      // avanzamos al proximo sensor
      if (selectedSensor == HEARTRATE)
        selectedSensor = TEMPERATURE;
      else
        selectedSensor++;

      // verificamos y actualizamos el puntero currentSensor con el sensor seleccionado
      switch (selectedSensor)
      {
        case TEMPERATURE:
          currentSensor = &temperature;
          break;

        case HEARTRATE:
          currentSensor = &heartRate;
          break;

        case PRESSURE:
          currentSensor = &pressure;
          break;
      }

      // seleccionamos el nuevo sensor
      currentSensor->Select();

/*
      Serial.print("Change to Sensor:");
      Serial.print(currentSensor->type);

      Serial.print("Selected Sensor:");
      Serial.print(selectedSensor);*/
      break;

    case ENTER:
      // indicar al sensor 
      currentSensor->Capture();
      Serial.print("Sensor Active:");
      Serial.print(currentSensor->active);
      break;

    case MEMORY:       
      Serial.print("Begin select patient");
      // si el sensor esta activo, detenerlo y cambiar a la pantalla de paciente
      if(currentSensor->active)
        currentSensor->Capture();

      Serial.print("Select first patient");
      // seleccionar el primer paciente
      storage.ReadPatient();

      Serial.print("Change to PATIENT Menu");
      
      // establecer la pantalla de pacientes
      menu= PATIENTS;
      DisplayPatientsScreen();
      break;
  }

  
}

// funcion para actualizar y mostrar los datos del sensor seleccionado
void UpdateCaptureSensor() {
  if (currentSensor->active) {
    currentSensor->Update();

    if (currentSensor->canDisplay) {
      currentSensor->Display();
    }
  }
}

// Patients functions

void UpdatePatients() {
  if (resetMenu) {
    DisplayPatientsScreen(); 
    resetMenu = false;
  }

  switch (action)
  {
    case SELECT:
      UnselectPatient(storage.currentPatient);

      Serial.print("Previous patient: ");
      Serial.print(storage.selectedPatient);

      if(!storage.NextPatient())
        storage.ReadPatient(); 

      SelectPatient(storage.currentPatient);

      Serial.print(" - Current patient: ");
      Serial.print(storage.currentPatient.name);

      Serial.println();
      break;

    case ENTER:
      PatientData data;
      data.data1 = temperature.GetValue();
      data.data2 = pressure.GetValue();
      data.data3 = heartRate.GetValue();
      data.data4 = heartRate.GetSPO();

      Serial.print("Saving data");
      storage.InsertPatientData(storage.currentPatient, data);

      storage.ShowPatientData();
      
      menu = CAPTURE;
      DisplayCaptureScreen();
      temperature.Display();
      pressure.Display();
      heartRate.Display();
      break;
  }

  resetMenu = false;
}
