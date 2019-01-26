#ifndef __SENSORDATA_H_INCLUDED__
#define __SENSORDATA_H_INCLUDED__

#include <EEPROM.h>
#include "LinkedList.h"


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

struct Patient {
  int position;
  bool active;
  
  char name[50];
  BloodTypeEnum bloodType;
  int birthDate;
};

struct PatientData{
  int position;
  int patient;
  bool active;

  int date;
  int time;
  
  float data1;
  float data2;
  float data3;
  float data4;
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
      address += sizeof(Patient) * position;

      return address;
    }

    int CalculatePatientDataAddress(int patientPosition, int dataPosition) {
      int address = 0;

      address = sizeof(DeviceSettings);
      address += (sizeof(Patient) + (sizeof(PatientData) * 10)) * patientPosition;
      address += (sizeof(Patient) + (sizeof(PatientData) * dataPosition));

      return address;
    }
  
  void ReadPatients() {
    Patient *patient;
    int memoryAddress;
    
    for(int i=0; i < PATIENT_COUNT; i++) {
      patient = &patients[i];
      memoryAddress = CalculatePatientAddress(i);

      Serial.print("Memory Address: ");
      Serial.print(memoryAddress);
      Serial.println();
      
      EEPROM_Read(memoryAddress, patient);

      Serial.print("Reading patient:");

      //Serial.print(patient->position);
      Serial.print(patient->name);
      Serial.println();
    }
    }
    
  public:
  
    Patient patients[PATIENT_COUNT];
    DeviceSettings settings;

    void Setup() {
      EEPROM_Read(0, settings);

      Serial.print("Memory Ready: ");
      Serial.print(settings.ready);
      Serial.println();
    
      if(!settings.ready){
        Serial.print("Begin to register patients");
        ClearMemory();
        RegisterPatients();
        
        settings.ready = 1;
        EEPROM_Write(0, settings);
      }
      
      ReadPatients();
    }

  void ShowPatients() {
     Patient patient;
    
    for(int i=0; i < PATIENT_COUNT; i++) {
      patient = patients[i];
      Serial.print("Patient name: ");
      Serial.print(patient.position);
      Serial.print(patient.name);
      Serial.println();
    }
  }
  
  void ClearMemory() {
    for(int i=0; i < EEPROM.length(); i++)
      EEPROM_Write(i, 0);
  }
  
  void RegisterPatients() {
    Patient patient;
    PatientData data;
    
    for(int i = 0; i < PATIENT_COUNT; i++){
      // datos generales de paciente
      patient.position = i;
      patient.bloodType = OP;
      patient.birthDate = 0;
      patient.active = false;
      sprintf(patient.name, "Paciente %d", i + 1);
      
      // guardar en la EEPROM
      SavePatient(patient);
      
      // registrar 10 datos de paciente
      for(int x = 0; x <PATIENT_DATA_COUNT; x++){
        data.position = 0;
        data.patient = 0;
        data.active = 0;
        data.date = 0;
        data.time = 0;
        
        data.data1 = 0;
        data.data2 = 0;
        data.data3 = 0;
        data.data4 = 0;
        
        // guardar en la EEPROM
        SavePatientData(data);
      }
    }
  }

  /*
  
  LinkedList<Patient*> GetActivePatients() {
    LinkedList<Patient*> result = new LinkedList<Patient*>();
    Patient* current;
    
    for(int i=0; i < PATIENT_COUNT; i++) {
      current = patients[i];
      
      if(current.active)
        result.add(&current);
    }

    return result;
  }
  
  LinkedList<PatientData> GetPatientData(int patient) {
      LinkedList<PatientData> result = new LinkedList<PatientData>();
    int memoryAddress;

      for(int i = 0; i < PATIENT_DATA_COUNT; i++) {
        PatientData *data = malloc(sizeof(PatientData));
        memoryAddress = CalculatePatientDataAddress(patient, i);
    
    EEPROM_Read(memoryAddress, data);
    result.add(data);
      }

      return result;
    }*/
    
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
};

#endif
