#include <Arduino.h>
#include <Wire.h>
#include "compass.h"
#include "HMC5883L.h"


HMC5883L compass;
int error = 0;
float headingDegrees;

void setupCompass(){

  // create compass
  compass = HMC5883L();

  // set compass error, print message if in correct
  error = compass.SetScale(1.3); // Set the scale of the compass.
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));

  // set measurement type to continous to prevent compass going idel
  error = compass.SetMeasurementMode(Measurement_Continuous);
  if(error != 0)
    Serial.println(compass.GetErrorText(error));
}

float updateCompass(){

  // Retrive the raw values from the compass (not scaled).
  MagnetometerRaw raw = compass.ReadRawAxis();
  
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: 2� 37' W, which is 2.617 Degrees, or (which we need) 0.0456752665 radians, I will use 0.0457
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = -0.0445059;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  headingDegrees = heading * 180/M_PI;

  // Normally we would delay the application by 66ms to allow the loop
  // to run at 15Hz (default bandwidth for the HMC5883L).
  // However since we have a long serial out (104ms at 9600) we will let
  // it run at its natural speed.
  //delay(66);
  
  return headingDegrees;

}

void printCompass(int headingDegrees){
  
  Serial.print("Heading: ");
  Serial.print(headingDegrees);
  Serial.println(" Degrees");

}


/*
  Serial.print("Raw:\t");
  Serial.print(raw.XAxis);
  Serial.print("   ");   
  Serial.print(raw.YAxis);
  Serial.print("   ");   
  Serial.print(raw.ZAxis);
  Serial.print("   \tScaled:\t");
   
  Serial.print(scaled.XAxis);
  Serial.print("   ");   
  Serial.print(scaled.YAxis);
  Serial.print("   ");   
  Serial.print(scaled.ZAxis);*/
