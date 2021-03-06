
#include "HeartRateSensor.h"
#include "TemperatureSensor.h"
#include "PressureSensor.h"
#include "CommunicationManager.h"

#include <Adafruit_GFX.h>    // Include core graphics library
#include <Adafruit_ILI9341.h> // Include Adafruit_ILI9341 library to drive the display
#include <ArduinoJson.h>

#define bselect 25    // PIN para el boton para moverse entre items
#define benter 24     // PIN para el boton de Activar/Seleccionar
#define bmenu 23     // PIN Para el boton de cambiar entre pantallas
#define bmemory 22    // PIN para el boton de guardar los datos 

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
  DATALIST = 3,     // pantalla de listado de datos de un paciente
  CONECTION = 4
};

/*
    Sensores de signos vitales
*/
HeartRateSensor heartRate;
TemperatureSensor temperature;
PressureSensor pressure;

SensorBase *currentSensor; // puntero al sensor que esta seleccionado
int selectedSensor;        // tipo de sensor seleccionado


/*
    Objetos y variables globales
*/
StorageManager storage;    // objeto que administra el acceso a datos en la EEPROM
CommunicationManager communication;

int action;                // accion/boton que presiona el usuario
int menu;                  // pantalla actual del programa
bool resetMenu;            // bandera para indicar si se necesita volver a dibujar toda la pantalla
uint32_t lastActionTime;   // tiempo en que se ejecuto la ultima accion del usuario
int ShowPatientMode;       // 1- Save Mode 2- Show Mode

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  SetupDisplay();          // inicializar y configurar la pantalla

  storage.Setup();         // configurar el repositorio de datos
  communication.Setup();
  communication.SetOnMessageCallBack(MessageReceived);

  Serial.println("configuracion completa");
  heartRate.Setup();

  /*
    temperature.Setup();
    pressure.Setup();
    heartRate.Setup();
  */

  /*
     Establecer valores por defecto
     pantalla de captura, sensor de temperatura y puntero a sensor de temperatura
  */
  menu = CAPTURE;
  selectedSensor = TEMPERATURE;
  currentSensor = &temperature;
  lastActionTime = 0;

  DisplayCaptureScreen();  //dibuja la pantalla principal

  Serial.print("Complete Setup");

}

void loop() {
  RTC.updateTime();
  //heartRate.sensor.update();
  communication.Update();

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
      ClockDate();
      UpdateCaptureSensor();
      break;
  }
}

void DisplayPatientsScreen()
{
  ClearScreen();

  tft.setCursor(70, 10);
  tft.setTextColor(0x01CE);
  tft.setTextSize(2);
  tft.println("PATIENTS");
  tft.drawLine(0, 30, 240, 30, 0x00AA);

  storage.ReadPatient();

  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);

  int posY = 40;

  do {
    tft.setCursor(5, posY);
    tft.println(storage.currentPatient.name);

    posY += 20;
  } while (storage.NextPatient());

  storage.ReadPatient();
  SelectPatient(storage.currentPatient);
}


/*
    Funcion para determinar que pantalla va a procesar el boton/comando presionado
*/
void UpdateDisplay() {
  // validacion para evitar procesar un boton multiples veces
  if ((millis() - lastActionTime) < 400)

    return;

  Serial.println();
  Serial.print("Action:");
  Serial.print(action);

  // si se presiono el boton de MENU, actualizamos la variable menu e indicamos que necesita dibujar la nueva pantalla
  if (action == MENU)
  {
    switch (menu)
    {
      case CAPTURE:
        menu = PATIENTS;
        break;

      case PATIENTS:
        menu = CONECTION;
        break;
        
      case CONECTION:
        menu = CAPTURE;
        break; 

      case DATALIST:
        menu = PATIENTS;
        break;

 
    }

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
      UpdatePatientData();
      break;

    case CONECTION:
      UpdateConection();
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
      Serial.print(" Current Sensor: ");
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
      if (currentSensor->active)
        currentSensor->Capture();

      Serial.print("Select first patient");
      // seleccionar el primer paciente
      storage.ReadPatient();

      Serial.print("Change to PATIENT Menu");

      // Indicamos que la pantalla de pacientes sera para guardar informacion
      ShowPatientMode = 1;

      // establecer la pantalla de pacientes
      menu = PATIENTS;
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

    // Indicamos que la seleccion de pacientes sera para mostrar datos
    ShowPatientMode = 2;
  }

  switch (action)
  {
    case SELECT:
      UnselectPatient(storage.currentPatient);

      Serial.print("Previous patient: ");
      Serial.print(storage.selectedPatient);

      if (!storage.NextPatient())
        storage.ReadPatient();

      SelectPatient(storage.currentPatient);

      Serial.print(" - Current patient: ");
      Serial.print(storage.currentPatient.name);

      Serial.println();

      break;

    case ENTER:
      if (ShowPatientMode == 2) {
        menu = DATALIST;
        DisplayPatientDataScreen();
        ShowPatientReg();
      }
      break;

    case MEMORY:
      if (ShowPatientMode == 1) {

        PatientData data;
        data.active = true;
        data.data1 = temperature.GetValue();
        data.data2 = pressure.GetValue();
        data.data3 = heartRate.GetValue();
        data.data4 = heartRate.GetSPO2();

        data.date.day = RTC.dayofmonth;
        data.date.month = RTC.month;
        data.date.year = RTC.year;
        data.date.hours = RTC.hours;
        data.date.minutes = RTC.minutes;
        data.date.seconds = RTC.seconds;

        Serial.print("Saving data");
        storage.InsertPatientData(storage.currentPatient, data);

        menu = CAPTURE;
        DisplayCaptureScreen();
        temperature.Display();
        pressure.Display();
        heartRate.Display();
      }
      break;
  }

  resetMenu = false;
}

void UpdatePatientData() {
  if (resetMenu) {
    DisplayPatientsScreen();
    resetMenu = false;
  }

  switch (action)
  {
    case SELECT:
      menu = PATIENTS;
      DisplayPatientsScreen();
      Serial.print("regresar a SELECT");
      break;
  }

  resetMenu = false;
}


void ShowPatientReg() {
  storage.ReadPatientData();

  tft.setCursor(90, 35);
  tft.setTextColor(0xFFE0);// 0xCCA1
  tft.setTextSize(1);
  tft.print(storage.currentPatient.name);

  int Y1 = 75;
  int Y2 = 85;
  int Y3 = 55;
  int Y4 = 55;

  do {

    tft.setTextColor(0x00AA);//0x00AA
    tft.setTextSize(1);
    tft.setCursor(25, Y3);
    tft.print(storage.currentData.date.day);
    tft.print("/");
    tft.print(storage.currentData.date.month);
    tft.print("/");
    tft.print(storage.currentData.date.year);

    tft.setCursor(180, Y4);
    tft.print(storage.currentData.date.hours);
    tft.print(":");
    tft.print(storage.currentData.date.minutes);
    tft.print(":");
    tft.print(storage.currentData.date.seconds);


    tft.setTextColor(0xBC21);//   0xBC21,0xFFE0
    tft.setTextSize(1);
    tft.setCursor(45, Y1);
    tft.println(storage.currentData.data1);
    tft.setCursor(80, Y1);
    tft.print((char)167); tft.print("C");
    tft.setCursor(45, Y2);
    tft.print(storage.currentData.data2);
    tft.setCursor(80, Y2);
    tft.print("mmHg");
    tft.setCursor(155, Y1);
    tft.print(storage.currentData.data3);
    tft.setCursor(190, Y1);
    tft.print("bpm");
    tft.setCursor(155, Y2);
    tft.print(storage.currentData.data4);
    tft.setCursor(190, Y2);
    tft.print("%");

    Y1 += 54;
    Y2 += 54;
    Y3 += 54;
    Y4 += 54;

  } while (storage.NextPatientData() == true);
}

void UpdateConection(){
   if (resetMenu) {
     DisplayConectionScreen();
     resetMenu = false;
   }

  resetMenu = false; 
}

StaticJsonDocument<256> request;

void MessageReceived(String message)
{
  DeserializationError error;
  const char* operation;

  if(message.length() == 0)
    return;

  Serial.println(message.c_str());

  error = deserializeJson(request, message);
  if(error != NULL)
  {
    Serial.print(F("error verifying message: "));
    Serial.println(error.c_str());
    return;
  }

  operation = request["operation"];

  Serial.println(operation);
  
  if(strcmp(operation, "getDeviceStatus") == 0)
  {
    Serial.println("procesando getDeviceStatus");
    ProcessGetDeviceStatus();
  }
  if(strcmp(operation, "savePatient") == 0)
  {
    Serial.println("procesando savePatient");
    ProcessSavePatient();
  }
  if(strcmp(operation, "getPatientData") == 0)
  {
    Serial.println("procesando getPatientData");
    ProcessGetPatientData();
  }
}

void ProcessGetDeviceStatus()
{
  StaticJsonDocument<256> response;
  String message;
  
  response["SystemTime"] = HRS;
  response["heartRate"] = heartRate.initialized;
  response["temperature"] = temperature.initialized;
  response["pressure"] = pressure.initialized;

  serializeJson(response, message);
  communication.SendData(message);
}

void ProcessSavePatient()
{
  StaticJsonDocument<256> response;
  JsonObject clientObject;
  String message;
  
  Patient patient;
  int number;  

  number = request["payload"]["number"];
  if(number < 0 || number > 4)
  {
    Serial.println("Patient does not exists");
    return;
  }

  Serial.println("processing patient");

  patient.position = number - 1;
  strcpy(patient.name, request["payload"]["name"]);
  patient.birthDate.day = atoi(request["payload"]["dobDay"]);
  patient.birthDate.month = atoi(request["payload"]["dobMonth"]);
  patient.birthDate.year = atoi(request["payload"]["dobYear"]);

  Serial.println("saving patient");
  storage.SavePatient(patient);

  response["sucess"] = "true";
  serializeJson(response, message);
  communication.SendData(message);
}

void ProcessGetPatientData()
{
  StaticJsonDocument<256> response;
  JsonArray data = response.createNestedArray("data");
  String message;
  char strDate[15];
  char strTime[15];
  
  int number;

  number = atoi(request["payload"]["patient"]);
  
  if(number < 0 || number > 4)
  {
    Serial.println("Patient does not exists");
    return;
  }

  response["patient"] = number;

  Serial.println("Obtienendo datos del paciente #");
  Serial.print(number);

  storage.selectedPatient = number;
  storage.ReadPatientData();
  
  do{
  //  if(!storage.currentData.active)
    //  continue;

    Serial.println("Procesando dato de paciente");
    JsonObject item = data.createNestedObject();

    //format date and time
    sprintf(strDate,"%02d/%02d/%04d\0", storage.currentData.date.day, storage.currentData.date.month, storage.currentData.date.year);
    sprintf(strTime,"%02d:%02d:%02d\0", storage.currentData.date.hours, storage.currentData.date.minutes, storage.currentData.date.seconds);
    
    item["number"] = storage.currentData.position + 1;
    item["date"] = strDate;
    item["time"] = strTime;
    
    item["temp"] = storage.currentData.data1;
    item["pressure"] = storage.currentData.data2;
    item["bpm"] = storage.currentData.data3;    
    item["spo2"] = storage.currentData.data4;
  }while(storage.NextPatientData() == true);
  
  serializeJson(response, message);
  communication.SendData(message);
}
