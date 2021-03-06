#ifndef __SENSORDATA_H_INCLUDED__
#define __SENSORDATA_H_INCLUDED__

#include <EEPROM.h>



#define PATIENT_COUNT 5
#define PATIENT_DATA_COUNT 5


enum SensorType {
  TEMPERATURE = 0,
  PRESSURE = 1,
  HEARTRATE = 2,
};

enum BloodTypeEnum {
  AP = 0,
  AN = 1,
  BP = 2,
  BN = 3,
  ABP = 4,
  ABN = 5,
  OP = 6,
  ON = 7
};

struct SensorData {
  unsigned long time;
  SensorType type;
  unsigned long data;
};

struct DeviceSettings {
  bool ready;
  char identifier[10];
};

struct DateTime {
  int day;
  int month;
  int year;

  int hours;
  int minutes;
  int seconds;
};

struct Patient {
  int position;
  bool active;

  char name[50];
  BloodTypeEnum bloodType;
  DateTime birthDate;
};

struct PatientData {
  int position;
  int patient;
  bool active;

  DateTime date;

  float data1; // temperature
  float data2; // pressure
  float data3; // BPM
  float data4; // SPO2
};


template <class T> int EEPROM_Write(int ee, const T& value)
{
  const byte* p = (const byte*)(const void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    EEPROM.write(ee++, *p++);
  return i;
}

template <class T> int EEPROM_Read(int ee, T& value)
{
  byte* p = (byte*)(void*)&value;
  unsigned int i;
  for (i = 0; i < sizeof(value); i++)
    *p++ = EEPROM.read(ee++);
  return i;
}

class StorageManager {

  private:

    int CalculatePatientAddress(int position) {
      int address = 0;

      address = sizeof(DeviceSettings);
      address += (sizeof(Patient) + (sizeof(PatientData) * PATIENT_DATA_COUNT)) * position;
      return address;
      
      
    }

    int CalculatePatientDataAddress(int patientPosition, int dataPosition) {
      int address = 0;

      address = sizeof(DeviceSettings);
      address += (sizeof(Patient) + (sizeof(PatientData) * PATIENT_DATA_COUNT)) * patientPosition;
      address += (sizeof(Patient) + (sizeof(PatientData) * dataPosition));
      return address;
    }

  public:

    //Patient patients[PATIENT_COUNT];
    //PatientData patientData[PATIENT_DATA_COUNT];
    DeviceSettings settings;

    int selectedPatient;
    int selectedData;
    
    Patient currentPatient;
    PatientData currentData;

    void Setup() {
      EEPROM_Read(0, settings);

      //settings.ready = 0;
      if (!settings.ready) {
      Serial.print("Begin to register patients");
      ClearMemory();

      RegisterPatients();

      settings.ready = 1;
      EEPROM_Write(0, settings);
      }

      ShowPatients();
    }

    void ReadPatient() {
      int memoryAddress;
      selectedPatient = 0;

      memoryAddress = CalculatePatientAddress(selectedPatient);
      EEPROM_Read(memoryAddress, currentPatient);
    }

    bool NextPatient() {
      int memoryAddress;
      selectedPatient++;

      if (selectedPatient >= PATIENT_COUNT)
        return false;

      memoryAddress = CalculatePatientAddress(selectedPatient);
      EEPROM_Read(memoryAddress, currentPatient);

      return true;
    }

    void ReadPatientData() {
      int memoryAddress;
      selectedData = 0;

      memoryAddress = CalculatePatientDataAddress(selectedPatient, selectedData);
      EEPROM_Read(memoryAddress, currentData);
    }

    bool NextPatientData() {
      int memoryAddress;
      selectedData++;

      if (selectedData >= PATIENT_DATA_COUNT)
        return false;

      memoryAddress = CalculatePatientDataAddress(selectedPatient, selectedData);
      EEPROM_Read(memoryAddress, currentData);

      return true;
    }

   void ShowPatients() {
      ReadPatient();

      do {
        
        Serial.print("Reading patient: ");

        Serial.print(currentPatient.position);
        Serial.print(currentPatient.name);
        Serial.println();

        ShowPatientData();
      } while (NextPatient() == true);
    }

   void ShowPatientData() {
      ReadPatientData();
      
     


      do {
//        
        Serial.print(" - Position: #");
        Serial.print(currentData.position + 1);
        Serial.print(" - Active: ");
        Serial.print(currentData.active);
        
        Serial.print("Temp: ");
        Serial.print(currentData.data1);
        Serial.print("- Pressure: ");
        Serial.print(currentData.data2);
        Serial.print("- BPM: ");
        Serial.print(currentData.data3);
        Serial.print("- SPO2: ");
        Serial.print(currentData.data4);
        
        Serial.print("  -Date: ");
        Serial.print(currentData.date.day);
        Serial.print("/");
        Serial.print(currentData.date.month);
        Serial.print("/");
        Serial.print(currentData.date.year) ; 
        Serial.print("  -Hrs: ");
        Serial.print(currentData.date.hours);
        Serial.print(":");
        Serial.print(currentData.date.minutes);
        Serial.print(":");
        Serial.print(currentData.date.seconds) ; 

        Serial.println();
        
        
      } while (NextPatientData() == true);
    }

    void ClearMemory() {
      for (int i = 0; i < EEPROM.length(); i++)
        EEPROM_Write(i, 0);
    }

    void RegisterPatients() {
      Patient patient;
      PatientData data;

      for (int i = 0; i < PATIENT_COUNT; i++) {
        // datos generales de paciente
        patient.position = i;
        patient.bloodType = OP;
        patient.active = false;
        sprintf(patient.name, "Paciente %d \n", i + 1);

        // guardar en la EEPROM
        SavePatient(patient);

        // registrar 10 datos de paciente
        for (int x = 0; x < PATIENT_DATA_COUNT; x++) {
          data.position = x;
          data.patient = 0;
          data.active = 0;

          data.data1 = 0;
          data.data2 = 0;
          data.data3 = 0;
          data.data4 = 0;

          // guardar en la EEPROM
          SavePatientData(data);
        }
      }
    }
    
    Patient SavePatient(Patient patient) {
      int memoryAddress;

      Serial.print("Saving patient: ");
      Serial.print(patient.position);
      Serial.print(patient.name);
      Serial.println();

      memoryAddress = CalculatePatientAddress(patient.position);
      EEPROM_Write(memoryAddress, patient);

      return patient;
    }

    PatientData SavePatientData(PatientData data) {
      int memoryAddress;

      memoryAddress = CalculatePatientDataAddress(data.patient, data.position);
      EEPROM_Write(memoryAddress, data);

      return data;
    }

    PatientData InsertPatientData(Patient patient, PatientData data) {
      PatientData aux;

      data.position = 0;
      data.active = 1;
      data.patient = patient.position;
      aux = data;
      
      ReadPatientData();  

      do {
        SavePatientData(aux);

        currentData.position = aux.position + 1;
        aux = currentData;
      } while (NextPatientData() == true);
      
      return data;
    }
};

#endif
