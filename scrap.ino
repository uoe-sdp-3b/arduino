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







