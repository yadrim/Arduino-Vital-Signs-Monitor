#ifndef __CONECTIONMANAGER_H_INCLUDED__
#define __CONECTIONMANAGER_H_INCLUDED__

#include <SoftwareSerial.h>  
SoftwareSerial BT(10,11);    // pines TX y RX 
 

//int input;
//int led=8;

void setup(){
{
  Serial.begin(9600); 
  BT.begin(38400);    
}

void CONECTION(){
  if(BT.available())    // Si llega un dato por el puerto BT se envía al monitor serial
  Serial.write(BT.read());
 
  if(Serial.available())  // Si llega un dato por el monitor serial se envía al puerto BT
  BT.write(Serial.read());

}
crear una clase igua a storage











#endif
