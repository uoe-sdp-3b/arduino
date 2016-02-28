#include <Arduino.h>
#include <Wire.h>
#include "encoder.h"


/////////////////////////////////////
//          Encoder Board          //
/////////////////////////////////////

void printDynamicPositions(int *dynamicPositions){
  Serial.print("Dynamic Positions: ");
  for (int i = 0; i < ROTARY_COUNT; i++) {
    Serial.print(dynamicPositions[i]);
    Serial.print(' ');
  }
  Serial.println();
  delay(PRINT_DELAY);  // Delay to avoid flooding serial out
}

void resetDynamicPositions(int *dynamicPositions){
  int i;
  for(i = 0; i < 5; i++){
    dynamicPositions[i] = 0;
  }
}

void updateDynamicPositions(int *dynamicPositions){
  // Request motor position deltas from rotary slave board
  Wire.requestFrom(ROTARY_SLAVE_ADDRESS, ROTARY_COUNT);
  
  // Update the recorded motor positions
  for(int i = 0; i < ROTARY_COUNT; i++) {
    
    int r = (int8_t) Wire.read();  // Must cast to signed 8-bit type
    dynamicPositions[i] -= r;    
  }
}

// possible
// void updateDistanceFromOrigin(int *dynamicPositions){

// }
