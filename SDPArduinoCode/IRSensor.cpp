#include "IRSensor.h"
#include "SDPArduino.h"
#include <Arduino.h>

// global variables (probably will need)
#define BASE_READING 5.0 //arbitrary number for the normal distance the IR sensor can see 
						//will need to update this when we know how far IR sees when fitted
#define IR_READING 0.0
#define IR_PORT_NO 3 //probably temporary set up for port #
					 //possible port candidates: 3,5,6,9


void setupIRSensor()
{
	Serial.begin(57600); //what ever the baud rate is

	//Fetch IR Sensor Reading and update global variable
	IRReading = readDigitalSensorData(IR_PORT_NO); //what is the difference between rDSD and digitalRead? 

}


int updateIRSensor()
{
	int sensor_reading =0; // 0 => ball not caught
						   // 1 => ball caught
	
	//check to see if ball has been caught
	if(IR_READING < BASE_READING) 
	{
		sensor_reading = 1;

		//ball has been caught
		//send some message
	}

	//ball still not caught, do nothing
	else;

	return sensor_reading;
}


void printIRSensor()
{
	Serial.printf("IR Reading: %lf \n", IR_READING);
}