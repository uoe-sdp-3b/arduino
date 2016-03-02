#include "IRSensor.h"
#include "SDPArduino.h"
#include <Arduino.h>


void readIR(int *IRread)
{
  int sensor_reading = readDigitalSensorData(IR_PORT_NO);
  if(sensor_reading == 0){
    *IRread = 1;
  }
  else{
    *IRread = 0;
  }
}


void printIR(int *IRread)
{
  Serial.println(*IRread);
}
