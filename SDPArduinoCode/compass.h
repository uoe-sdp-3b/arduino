
#ifndef HEADER_COMPASS
  #define HEADER_COMPASS
	
	//global variables
/*	MagnetometerRaw raw;
	MagnetometerScaled scaled;
	float heading;*/
	//float headingDegrees;

	void setupCompass();
	float updateCompass();
	void printCompass(float headingDegrees);

#endif
