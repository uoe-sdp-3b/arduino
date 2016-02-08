#include <Arduino.h>
#include "SDPArduino.h"
#include "encoder.h"
#include "actions.h"

// array holding encoder rotations
int *dynamicPositions;


void initialSetup(){
  SDPsetup();
  dynamicPositions = (int *) malloc(sizeof(int)*ROTARY_COUNT);
  Serial.setTimeout(100); // time out for accepting a string
  helloWorld();
}

//////////////////////////////////////
//          Robot Stop              //
//////////////////////////////////////
void robotStop(){

  // send reply message
  Serial.println("0RS");

  motorAllStop();
  // printMotorPositions();
}

//////////////////////////////////////
//          Robot Kick              //
//////////////////////////////////////
void robotKick(int power){
  
  // send reply message
  Serial.println("0RK");
  
  int d = 1500 - (power * 10);
  //Serial.println(d);

  motorForward(ACTION_MOTOR, 70);
  delay(200);
  
  // move action motor backward
  motorBackward(ACTION_MOTOR,power);
  delay(d);
  motorAllStop();

  motorForward(ACTION_MOTOR, 70);
  delay(150);
  motorAllStop();
}

//////////////////////////////////////
//          Robot Grab              //
//////////////////////////////////////
void robotGrab(int power){

  // send reply message
  Serial.println("0RG");

  // move action motor forward
  motorForward(ACTION_MOTOR,power);
}

//////////////////////////////////////
//          Robot Forward           //
//////////////////////////////////////
void robotForwardDistance(int distance){

  Serial.println("0RF"); 

  // reset dynamicPositions
  resetDynamicPositions(&dynamicPositions);

  // setup positions of rotations
  int rot = (int) (distance * 6.2471) - 10; // 6.2471 = rotations for 1 cm (WRONG ATM)
  int left = dynamicPositions[0];
  int right = dynamicPositions[1];

  // turn on motors
  motorForward(FRONT_LEFT_MOTOR, 100);
  motorForward(FRONT_RIGHT_MOTOR, 100);

  while(left < rot || right < rot){
    delay(5);
    updateDynamicPositions(&dynamicPositions);
    //printDynamicPositions();
    left = dynamicPositions[0];
    //Serial.println(left);
    right = dynamicPositions[1];    
    //Serial.println(right);
  }

  motorAllStop(); 
}

//////////////////////////////////////
//          Robot Backward          //
//////////////////////////////////////
void robotBackwardDistance(int distance){

  Serial.println("0RB");  

  // reset dynamicPositions
  resetDynamicPositions(&dynamicPositions);

  // setup positions of rotations
  int rot = (int) (-distance * 6.2471) + 10; // 6.2471 = rotations for 1 cm (WRONG ATM)
  int left = dynamicPositions[0];
  int right = dynamicPositions[1];

  // turn on motors
  motorBackward(FRONT_LEFT_MOTOR, 100);
  motorBackward(FRONT_RIGHT_MOTOR, 100);

  while(left > rot || right > rot){
    delay(5);
    updateDynamicPositions(&dynamicPositions);
    //printDynamicPositions();
    left = dynamicPositions[0];
    //Serial.println(left);
    right = dynamicPositions[1];    
    //Serial.println(right);
  }

  motorAllStop();
}

//////////////////////////////////////
//          Robot Left              //
//////////////////////////////////////
void robotTurnAntiClockWise(int degrees){

  Serial.println("0RL");
  
  // reset dynamicPositions
  resetDynamicPositions(&dynamicPositions);

  // setup positions of rotations
  int rot = (int) (degrees*0.9); // 1.26414 = rotations for 1 degrees (WRONG ATM)
  int left = dynamicPositions[0];
  int right = dynamicPositions[1];

  // turn on motors
  motorForward(FRONT_RIGHT_MOTOR, 50);
  motorBackward(FRONT_LEFT_MOTOR, 50);
  motorForward(TURNING_MOTOR, 50);

  while(left > -rot || right < rot){
    // is this delay needed?
    delay(5);
    updateDynamicPositions(&dynamicPositions);
    left = dynamicPositions[0];
    right = dynamicPositions[1];    
  }

  motorAllStop();
}

//////////////////////////////////////
//          Robot Right             //
//////////////////////////////////////
void robotTurnClockwise(int degrees){
  
  // reset dynamicPositions
  resetDynamicPositions(&dynamicPositions);

  // setup positions of rotations
  int rot = (int) (degrees*0.9); // 1.26414 = rotations for 1 degrees (WRONG ATM)
  int left = dynamicPositions[0];
  int right = dynamicPositions[1];

  // turn on motors
  motorBackward(FRONT_RIGHT_MOTOR, 50);
  motorForward(FRONT_LEFT_MOTOR, 50);
  motorBackward(TURNING_MOTOR, 50);

  while(left < rot || right > -rot){
    // this delay may not be needed
    delay(5);
    updateDynamicPositions(&dynamicPositions);
    left = dynamicPositions[0];
    right = dynamicPositions[1];    
  }

  motorAllStop();

  Serial.println("0RR");
}


// after this is done need to match names to switch statements
// also need to include header file.
