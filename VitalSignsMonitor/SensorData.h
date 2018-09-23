#ifndef __SENSORDATA_H_INCLUDED__
#define __SENSORDATA_H_INCLUDED__

enum SensorType {
  HEARTRATE = 0,
  TEMPERATURE = 1
};

struct SensorData {
  unsigned long time;
  SensorType type;
  unsigned long data;
};

#endif
