#include <Arduino.h>

// #include "gyro.h"   // for reading and updating the angle rotated using gyro sensor
#include "actions.h"    // for being able to access global variables 
#include "SDPArduino.h" // for turning motors off and on
#include "encoder.h" // for reading, updating and printing encoder board information
#include "compass.h" // for reading, printing, updating compass
// #include "IRSensor.h"

#include <math.h>

// variable to hold encoder readings
int dynamicPositions[ROTARY_COUNT] = {0};
// variable to hold compass readings
float heading = 0.0;
// variables for holding the current angle that has been roatated based on gyro information
float angleRotated = 0.0;
// variable stating if ball is caught or not
int caughtBall = 0; // 0 = false; 1 = true

// scaling the motors from commands recieved over RF
int sLeft = 100;
int sRight = 94;



void initialSetup(){

  // initial pin layout, and serial, wire begin
  SDPsetup();

  // setup compass
  setupCompass(&heading);

  // setup gyro sensor
  // setupGyro();

  // set time out for reading 7 bytes from radio link
  Serial.setTimeout(100);

  // print hello world to acknowledge robot booting up
  helloWorld();
}

//////////////////////////////////////
//          Robot Stop              //
//////////////////////////////////////
void robotStop(){

  motorAllStop();
}

//////////////////////////////////////
//          Robot Kick              //
//////////////////////////////////////
void robotKick(int power){
  
  int d = 2000 - (power * 10);
  //Serial.println(d);

  // motorBackward(ACTION_MOTOR, 70);
  // delay(200);
  
  // move action motor backward
  motorForward(ACTION_MOTOR,power);
  delay(d);
  motorAllStop();

  // motorForward(ACTION_MOTOR, 70);
  // delay(150);
  // motorAllStop();
}

//////////////////////////////////////
//          Robot Forward           //        1st
//////////////////////////////////////
void robotForwardDistance(int distance){

  // reset dynamicPositions
  resetDynamicPositions(dynamicPositions);

  // setup positions of rotations
  int rot = (int) (distance * 7.22) - 10; // 6.2471 = rotations for 1 cm (WRONG ATM)
  int left = dynamicPositions[0];
  int right = dynamicPositions[1];

  // turn on motors
  motorForward(FRONT_LEFT_MOTOR, int(sLeft));
  motorForward(FRONT_RIGHT_MOTOR, int(sRight));

  while(left < rot || right < rot){
    updateDynamicPositions(dynamicPositions);
    left = dynamicPositions[0];
    right = dynamicPositions[1];    
  }

  motorAllStop(); 
  // printDynamicPositions(dynamicPositions);
}

//////////////////////////////////////
//          Robot Backward          //        2nd
//////////////////////////////////////
void robotBackwardDistance(int distance){

  // reset dynamicPositions
  resetDynamicPositions(dynamicPositions);

  // setup positions of rotations
  int rot = (int) (-distance * 7.22) + 10; // 6.2471 = rotations for 1 cm (WRONG ATM)
  int left = dynamicPositions[0];
  int right = dynamicPositions[1];

  // turn on motors
  motorBackward(FRONT_LEFT_MOTOR, int(sLeft));
  motorBackward(FRONT_RIGHT_MOTOR, int(sRight));

  while(left > rot || right > rot){
    delay(5);
    updateDynamicPositions(dynamicPositions);
    left = dynamicPositions[0];
    right = dynamicPositions[1];    
  }

  motorAllStop();
}

//////////////////////////////////////
//          Robot Left              //        3rd CALIBRATE
//////////////////////////////////////
void robotTurnAntiClockwise(int degrees){

  // reset dynamicPositions
  resetDynamicPositions(dynamicPositions);

  // setup positions of rotations
  int rot = (int) (degrees*0.87); // 1.26414 = rotations for 1 degrees (WRONG ATM)
  int left = dynamicPositions[0];
  int right = dynamicPositions[1];

  // turn on motors
  motorForward(FRONT_RIGHT_MOTOR, int(90*sRight));
  motorBackward(FRONT_LEFT_MOTOR, int(90*sLeft));

  while(left > -rot || right < rot){
    
    updateDynamicPositions(dynamicPositions);
    left = dynamicPositions[0];
    right = dynamicPositions[1]; 
  }

  motorAllStop();
}  

//////////////////////////////////////
//          Robot Right             //        4th CALIBRATE
//////////////////////////////////////
void robotTurnClockwise(int degrees){
  
  // reset dynamicPositions
  resetDynamicPositions(dynamicPositions);

  // setup positions of rotations
  int rot = (int) (degrees*0.80); // 1.26414 = rotations for 1 degrees (WRONG ATM)
  int left = dynamicPositions[0];
  int right = dynamicPositions[1];

  // turn on motors
  motorBackward(FRONT_RIGHT_MOTOR, int(90*sRight));
  motorForward(FRONT_LEFT_MOTOR, int(90*sLeft));

  while(left < rot || right > -rot){

    updateDynamicPositions( dynamicPositions);
    left = dynamicPositions[0];
    right = dynamicPositions[1];
  }

  motorAllStop();
}

//////////////////////////////////////
//          Open grabber            //    THIS NEEDS CHANGED!! DONE
//////////////////////////////////////
void openGrabber(int power){

  // move action motor forward
  motorForward(ACTION_MOTOR,100);
}

//////////////////////////////////////
//          Close grabber           //     THIS NEEDS CHANGED!! DONE
//////////////////////////////////////
void closeGrabber(int power){
  motorBackward(ACTION_MOTOR,power);
  delay(50);
  motorStop(ACTION_MOTOR);

  // move action motor forward
  // motorForward(ACTION_MOTOR,power); ???????
}

//////////////////////////////////////
//            PING                  //
//////////////////////////////////////

void ping(){
  Serial.println("PONG");
}


/////////////////////////////////////
//        READ COMPASS             //
/////////////////////////////////////
void readCompass(){

    updateCompass(&heading);
    Serial.print("compass heading: ");
    printCompass(&heading);
}




/////////////////////////////////////
//        READ INFRARED            //
/////////////////////////////////////
void readInfrared(){

  int sensor_reading = digitalRead(3);
  if(sensor_reading == 0){
    Serial.println("y");
  }
  else{
    Serial.println("n");
  }

}




/////////////////////////////////////
//        READ SONAR               //
/////////////////////////////////////
void readSonar(){

}




/////////////////////////////////////
//        SCALE LEFT               //
/////////////////////////////////////
void scaleLeft(int scale){
  sLeft = scale;
  Serial.print("scale left: ");
  Serial.println(sLeft);
}




/////////////////////////////////////
//        SCALE RIGHT              //
/////////////////////////////////////
void scaleRight(int scale){
  sRight = scale;
  Serial.print("scale right: ");
  Serial.println(sRight);
}


/////////////////////////////////////
//    PRINT DYNAMIC POSITIONS      //
/////////////////////////////////////
void getInfo(){
  printDynamicPositions(dynamicPositions);

  Serial.print("left sclae: ");
  Serial.println(sLeft);
  Serial.print("right scale: ");
  Serial.println(sRight);
}




void forwardSlow(int distance){

  // reset dynamicPositions
  resetDynamicPositions(dynamicPositions);

  // setup positions of rotations
  int rot = (int) (distance * 7.22) - 10; // 6.2471 = rotations for 1 cm (WRONG ATM)
  int left = dynamicPositions[0];
  int right = dynamicPositions[1];

  // turn on motors
  motorForward(FRONT_LEFT_MOTOR, int(sLeft*0.7));
  motorForward(FRONT_RIGHT_MOTOR, int(sRight*0.7));

  while(left < rot || right < rot){
    updateDynamicPositions(dynamicPositions);
    left = dynamicPositions[0];
    right = dynamicPositions[1];    
  }

  motorAllStop(); 
  // printDynamicPositions(dynamicPositions);
}