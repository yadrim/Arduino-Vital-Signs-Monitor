#ifndef __COMMUNICATION_MANAGER_H_INCLUDED__
#define __COMMUNICATION_MANAGER_H_INCLUDED__

#include "SensorData.h"

typedef void (*OnMessageReceived)(String message);

class CommunicationManager 
{

  private:
    OnMessageReceived notifier;
  
    String inputBuffer;
    int inputIndex;
    
    String outputBuffer;    
    int outputIndex;

  public:   

    bool isReading;
    bool isSending;
    bool isConnected;

    void Setup() 
    {
      Serial1.begin(38400);
    }

    void SendData(String data)
    {
      outputIndex = 0;
      outputBuffer = data;
      isSending = true;
    }

    void SetOnMessageCallBack(OnMessageReceived callback)
    {
      notifier = callback;
    }

    void Update()
    {
      char data;
      
      // read commands
      if(Serial1.available())
      {
        isReading = true;
        data = Serial1.read();
        inputBuffer += data;

        if(data == '\0' || data == '\n')
        {
          isReading = false;

          if(notifier != NULL)
            notifier(inputBuffer);
        }
      }

      // send data
      if(isSending)
      {
        data = outputBuffer[outputIndex++];
        Serial1.write(data);

        if(outputIndex == outputBuffer.length())
          isSending = false;
      }
    }
};

#endif
