#include "IRSensor.h"
#include "SDPArduino.h"
#include <Arduino.h>


void IRSensorValue(int *IRread)
{
  int sensor_reading = readDigitalSensorData(IR_PORT_NO);
  if(sensor_reading == 0){
    *IRread = 1;
  }
  else{
    *IRread = 0;
  }
}


void IRSensorPrint(int *IRread)
{
  Serial.println(*IRread);
}
