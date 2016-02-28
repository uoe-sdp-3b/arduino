#include <Arduino.h>

#include "actions.h"    // for being able to access global variables 
#include "SDPArduino.h" // for turning motors off and on
#include "encoder.h" // for reading, updating and printing encoder board information
#include "compass.h" // for reading, printing, updating compass
#include "gyro.h"   // for reading and updating the angle rotated using gyro sensor
#include "IRSensor.h"

#include <math.h>

// variable to hold encoder readings
int dynamicPositions[ROTARY_COUNT];
// variable to hold compass readings
float heading;
// variables for holding the current angle that has been roatated based on gyro information
float angleRotated;
// variable stating if ball is caught or not
int caughtBall; // 0 = false; 1 = true



void initialSetup(){

  // initial pin layout, and serial, wire begin
  SDPsetup();

  // setup compass
  setupCompass(&heading);

  // setup gyro sensor
  setupGyro();

  // set time out for reading 7 bytes from radio link
  Serial.setTimeout(100);

  // print hello world to acknowledge robot booting up
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
  resetDynamicPositions(dynamicPositions);

  // setup positions of rotations
  int rot = (int) (distance * 7.22) - 10; // 6.2471 = rotations for 1 cm (WRONG ATM)
  int left = dynamicPositions[0];
  int right = dynamicPositions[1];

  // turn on motors
  motorForward(FRONT_LEFT_MOTOR, 100);
  motorForward(FRONT_RIGHT_MOTOR, 100);

  while(left < rot || right < rot){
    updateDynamicPositions(dynamicPositions);
    //printDynamicPositions(dynamicPositions);
    left = dynamicPositions[0];
    //Serial.println(left);
    right = dynamicPositions[1];    
    //Serial.println(right);
  }

  motorAllStop(); 
  printDynamicPositions(dynamicPositions);
}

//////////////////////////////////////
//          Robot Backward          //
//////////////////////////////////////
void robotBackwardDistance(int distance){

  Serial.println("0RB");  

  // reset dynamicPositions
  resetDynamicPositions(dynamicPositions);

  // setup positions of rotations
  int rot = (int) (-distance * 7.22) + 10; // 6.2471 = rotations for 1 cm (WRONG ATM)
  int left = dynamicPositions[0];
  int right = dynamicPositions[1];

  // turn on motors
  motorBackward(FRONT_LEFT_MOTOR, 100);
  motorBackward(FRONT_RIGHT_MOTOR, 100*0.97);

  while(left > rot || right > rot){
    delay(5);
    updateDynamicPositions(dynamicPositions);
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
void robotTurnAntiClockwise(int degrees){

  Serial.println("0RL");

  // reset dynamicPositions
  resetDynamicPositions(dynamicPositions);

  // setup positions of rotations
  int rot = (int) (degrees*0.87); // 1.26414 = rotations for 1 degrees (WRONG ATM)
  int left = dynamicPositions[0];
  int right = dynamicPositions[1];

  // turn on motors
  motorForward(FRONT_RIGHT_MOTOR, 90);
  motorBackward(FRONT_LEFT_MOTOR, 90);

  while(left > -rot || right < rot){
    
    updateDynamicPositions(dynamicPositions);
    left = dynamicPositions[0];
    right = dynamicPositions[1]; 
  }

  motorAllStop();
}  

//////////////////////////////////////
//          Robot Right             //
//////////////////////////////////////
void robotTurnClockwise(int degrees){
  
  Serial.println("0RR");
  
  // reset dynamicPositions
  resetDynamicPositions(dynamicPositions);

  // setup positions of rotations
  int rot = (int) (degrees*0.80); // 1.26414 = rotations for 1 degrees (WRONG ATM)
  int left = dynamicPositions[0];
  int right = dynamicPositions[1];

  // turn on motors
  motorBackward(FRONT_RIGHT_MOTOR, 90);
  motorForward(FRONT_LEFT_MOTOR, 90);

  while(left < rot || right > -rot){

    updateDynamicPositions( dynamicPositions);
    left = dynamicPositions[0];
    right = dynamicPositions[1];
  }

  motorAllStop();
}

//////////////////////////////////////
//          Open grabber            //
//////////////////////////////////////
void robotOpen(int power){

  // send reply message
  Serial.println("0RO");

  // move action motor forward
  motorBackward(ACTION_MOTOR,power);
}

//////////////////////////////////////
//          Close grabber           //
//////////////////////////////////////
void robotClose(int power){
  motorForward(ACTION_MOTOR,power);
  delay(50);
  motorStop(ACTION_MOTOR);

  // send reply message
  Serial.println("0RC");

  // move action motor forward
  motorBackward(ACTION_MOTOR,power);
}