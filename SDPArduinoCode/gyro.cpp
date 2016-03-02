#include "ITG3200.h"
#include <Arduino.h>


// global object gyro 
ITG3200 gyro;

	/*
		setups and calibrates the gyro sensor
	*/
	void setupGyro(){
	      gyro.init();
    	  gyro.zeroCalibrate(200,10);//sample 200 times to calibrate and it will take 200*10ms
	}


	/*
		updates the distanceRotated floating point number to now include the angle is has rotated through
		in the last 10ms 
	*/
	void updateGyro(float *distanceRotated){

		float ax,ay,az;
		gyro.getAngularVelocity(&ax,&ay,&az);

		// update distanceRotated
		*distanceRotated += ax * (1/100);

		// think about using a timer instead of constant 10ms

	}


	/*
		print angle rotated through so far
	*/
	void printGyro(float *distanceRotated){
		Serial.println(*distanceRotated);		
	}
