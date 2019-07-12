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
    uint32_t lastInputTime;
    
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

      data += '\n';

      Serial.println("enviando mensaje:");
      Serial.print(data.c_str());
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
        if ((millis() - lastInputTime) > 15000)
        {
          lastInputTime = 0;
          inputBuffer = "";
        }
        
        isReading = true;
        data = Serial1.read();

        Serial.print(data);

        if(data == '\0' || data == '\n' || data == '|')
        {
          isReading = false;

          Serial.println("mensaje recibido");

          if(notifier != NULL && isSending == false)
            notifier(inputBuffer);
        }

        inputBuffer += data;
        lastInputTime = millis();
      }

      // send data
      if(isSending)
      {
        Serial1.write(outputBuffer.c_str());
        isSending = false;

        /*
        data = outputBuffer[outputIndex++];
        Serial1.write(data);
        delay(30);
        Serial.print(data);
        */

        if(outputIndex == outputBuffer.length())
        {
          isSending = false;
          Serial1.flush();
        }
      }
    }
};

#endif
