#ifndef __COMMUNICATION_MANAGER_H_INCLUDED__
#define __COMMUNICATION_MANAGER_H_INCLUDED__

#include <SoftwareSerial.h>
#include "SensorData.h"

SoftwareSerial bluetooth(10,11);

class CommunicationManager 
{

  private:
    String inputBuffer;
    int inputIndex;
    
    String outputBuffer;    
    int outputIndex;

  public:
    typedef void (*OnMessageReceived)(String message);
    OnMessageReceived notifier;

    bool isReading;
    bool isSending;
    bool isConnected;

    void Setup() 
    {
      bluetooth.begin(115200);

      SendDataAtOnce("AT");
      if(GetDataAtOnce() != "OK")
      {
        Serial.write("No bluetooth available");
        return;
      }

      SendDataAtOnce("AT+NAME=MonitorSignosVitales");
      if(GetDataAtOnce() == "OK")
        Serial.write("Bluetooth set name");

      SendDataAtOnce("AT+PSWD=1234");
      if(GetDataAtOnce() == "OK")
        Serial.write("Bluetooth set security");

      SendDataAtOnce("AT+ROLE=0");
      if(GetDataAtOnce() == "OK")
        Serial.write("Bluetooth set as server");
    }

    bool GetDataAtOnce() 
    {
      inputBuffer = "";
      char data;

      while(bluetooth.available())
      {
        data = bluetooth.read();
        inputBuffer += data;
      }
      
      return inputBuffer.length() > 0;
    }

    bool SendDataAtOnce(String data)
    {
      outputBuffer = data;

      for(int i=0; i<outputBuffer.length(); i++)
      {
        bluetooth.write(outputBuffer[i]);
      }
    }

    void SendData(String data)
    {
      outputIndex = 0;
      outputBuffer = data;
      isSending = true;
    }

    void setOnMessageCallBack(OnMessageReceived callback)
    {
      notifier = callback;
    }

    void Update()
    {
      char data;
      
      // read commands
      if(bluetooth.available())
      {
        isReading = true;
        data = bluetooth.read();
        inputBuffer += data;

        if(data == '\0')
        {
          isReading = false;

          if(notifier != NULL)
            notifier(inputBuffer);
        }
      }

      // send data
      if(isSending)
      {
        if(outputIndex == outputBuffer.length())
          isSending = false;        
        else
        {
          data = outputBuffer[outputIndex++];
          bluetooth.write(data);
        }
      }
    }
};

#endif
