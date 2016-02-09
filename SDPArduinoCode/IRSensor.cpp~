#include "IRSensor.h"
#include "SDPArduino.h"
#include <Arduino.h>

// some of these definitions are temporary
#define BASE_READING 5.0 // base value to compare IR readings to
                         // (random right now)
#define IR_PORT_NO 3     // port no for IR (change according to actual port)


int IRSensorValue()
{
  int sensor_reading = readDigitalSensorData(IR_PORT_NO);
	return sensor_reading;
}


void IRSensorPrint()
{
  Serial.println("InfraReading:");
  Serial.println(IRSensorValue(),BIN); 
}

// Try smth like this function when we know how IR readings look like
/*
bool ballGrabbed() {
  int sensor_reading = IRSensorReading();

  //check to see if ball has been caught
  if(sensor_reading < BASE_READING) 
    return true;
  else
    return false;
*/
