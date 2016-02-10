// insdie main loop

    // sending file though I2C
    if (bytes_to_store > 0) {      
      byte incoming = Serial.read();
      storeByte(incoming);
      return;
    }

// inside switch statement
    case STORE: bytes_to_store = arg;
    break;
    




////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                Milestone 1 communication functions                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// global variable
int bytes_to_store = 0;

void storeByte(byte one_byte){
  int register_address = 69; 
  Wire.beginTransmission(register_address); // open I2C communication to intended receiver
  Wire.write( one_byte );   // sends the string (which is the file contents)
  Wire.endTransmission(); // end I2C communcation.
  Serial.println(one_byte, DEC);
  bytes_to_store--;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
//								Old movement functions													 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////
//          Robot Forward           //
//////////////////////////////////////

void moveRobotForward(int power){
    
  // need to create a reply message to let the PC acknowledge the accepted request
  Serial.println("0RF");
  
  //motorStop(0); // this might be useful, in the case the robot is already in a turning move

  motorForward(FRONT_LEFT_MOTOR, power);
  motorForward(FRONT_RIGHT_MOTOR, (int) (power*0.99));
}

//////////////////////////////////////
//          Robot Back              //
//////////////////////////////////////

void moveRobotBackward(int power){
  
  // send reply message
  Serial.println("0RB");
  
  //motorStop(0); // again might be useful if the robot is in a turning move

  // stop the motors first incase they are moving forward (to prevent mechinical failure)
  motorStop(FRONT_LEFT_MOTOR); 
  motorStop(FRONT_RIGHT_MOTOR);
  
  // set motors to move backwards
  motorBackward(FRONT_LEFT_MOTOR, power);
  motorBackward(FRONT_RIGHT_MOTOR, ((int) power*0.99));
  
}

//////////////////////////////////////
//          Robot Left              //
//////////////////////////////////////

void rotateRobotLeft(int power){
  
  // send reply message 
  Serial.println("0RL");
  
  // set motors for left rotation
  motorForward(FRONT_RIGHT_MOTOR, power);
  motorBackward(FRONT_LEFT_MOTOR, power);
  motorForward(TURNING_MOTOR, power);
  
}

//////////////////////////////////////
//          Robot Right             //
//////////////////////////////////////

void rotateRobotRight(int power){
  
  // send reply message 
  Serial.println("0RL");
  
  // set motors for left rotation
  motorBackward(FRONT_RIGHT_MOTOR, power);
  motorForward(FRONT_LEFT_MOTOR, power);
  motorBackward(TURNING_MOTOR, power);
  
}


//////////////////////////////////////////////////////////////////////////////
//        encoder board updates and prints not needed atm                   //
//////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////
// These 3 I may get rid off
int positions[ROTARY_COUNT] = {0};
int UO[ROTARY_COUNT] = {0};
int rotations[ROTARY_COUNT] = {0};
////////////////////////////////////////////

void updateMotorPositions() {
  // Request motor position deltas from rotary slave board
  Wire.requestFrom(ROTARY_SLAVE_ADDRESS, ROTARY_COUNT);
  
  // Update the recorded motor positions
  for(int i = 0; i < ROTARY_COUNT; i++) {

    // another integer array OU (over/uder 0-30000)
    if(positions[i] > 30000){
      int temp = positions[i];
      int remendier = temp - 30000;
      positions[i] = remendier;
      UO[i] += 1;      
    }

    if(positions[i] < -30000 && positions[i] < 0){
      int temp = positions[i];
      int remendier = temp + 30000;
      positions[i] = remendier;
      UO[i] -= 1;
    }
    
    int r = (int8_t) Wire.read();  // Must cast to signed 8-bit type
    positions[i] -= r;
    dynamicPositions[i] -= r;

    // will go soon
    rotations[i] = -r;

    
  }
}

void printDynamicPositions(){
  Serial.print("dynamic positions: ");
  for (int i = 0; i < ROTARY_COUNT; i++){
  Serial.print(dynamicPositions[i]);
  Serial.print(' ');
  }
  Serial.println();
  delay(PRINT_DELAY);  // Delay to avoid flooding serial out  
}

void printMotorPositions() {
  Serial.print("Motor positions: ");
  for (int i = 0; i < ROTARY_COUNT; i++) {
    Serial.print(positions[i]);
    Serial.print(' ');
  }
  Serial.print("                      ");
  Serial.print("UO: ");
  for (int i = 0; i < ROTARY_COUNT; i++) {
    Serial.print(UO[i]);
    Serial.print(' ');
  }
  Serial.print("                       ");
  Serial.print("rotations: ");
  for (int i = 0; i < ROTARY_COUNT; i++) {
    Serial.print(rotations[i]);
    Serial.print(' ');
  }
  Serial.println();
  delay(PRINT_DELAY);  // Delay to avoid flooding serial out
}


//////////////////////////////////////
//          Robot Left              //
//////////////////////////////////////
void robotTurnAntiClockwise(int degrees){

  Serial.println("0RL");
  
  // reset dynamicPositions
  resetDynamicPositions(dynamicPositions);

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
  
  // reset dynamicPositions
  resetDynamicPositions(dynamicPositions);

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
    updateDynamicPositions( dynamicPositions);
    left = dynamicPositions[0];
    right = dynamicPositions[1];    
  }

  motorAllStop();

  Serial.println("0RR");
}


  Serial.println("0RL");
  float orginalHeading = updateCompass();
  float finalHeading = fmod((orginalHeading - degrees), 360);
  float difference = orginalHeading - finalHeading;
  
  if(orginalHeading < finalHeading){
    difference = finalHeading - orginalHeading;
  }
  // turn on motors
  motorBackward(FRONT_RIGHT_MOTOR, 60);
  motorForward(FRONT_LEFT_MOTOR, 60);

  while(difference > 0){
    orginalHeading = updateCompass();
    difference = orginalHeading - finalHeading;
    
    if(orginalHeading < finalHeading){
      difference = finalHeading - orginalHeading;
    }







    ////////////////////////////////////////////
      Serial.println("0RL");
  int orginalAngle = (int) updateCompass();
  int finalAngle = (orginalAngle - degrees) % 360;
  int difference = degrees;

  if(finalAngle < 0){
    finalAngle = 360 + finalAngle;
  }
  
  // turn on motors
  motorBackward(FRONT_RIGHT_MOTOR, 50);
  motorForward(FRONT_LEFT_MOTOR, 50);

  while(difference > 2){

    int currentAngle = (int) updateCompass();
    difference = (finalAngle - currentAngle) % 360;
    if(difference < 0 ){
      difference = 360 + difference;
    }



    Serial.print(currentAngle);
    Serial.print("            ");
    Serial.print(finalAngle);
    Serial.print("            ");
    Serial.println(difference);
    delay(100);
  }

  // Serial.println("1");
  motorAllStop();
  // Serial.println("2");