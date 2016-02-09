
#ifndef HEADER_ENCODER
  #define HEADER_ENCODER
        
        // Encoder Board Variables
        #define ROTARY_SLAVE_ADDRESS 5
        #define ROTARY_COUNT 6
        #define PRINT_DELAY 200

	void printDynamicPositions(int *dynamicPositions);
	void resetDynamicPositions(int *dynamicPositions);
	void updateDynamicPositions(int *dynamicPositions);

#endif

// possible
//void updateDistanceFromOrigin(int *dynamicPositions);
