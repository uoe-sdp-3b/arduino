#include <stdlib.h>
#include <Wire.h>
#include "actions.h"



// Inbound message definitions
#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4
#define KICK 5 
#define GRAB 6
#define STORE 7
#define OPEN 8
#define CLOSE 9

// Outbound message definitions
#define CHECKSUM_FAILED "0CF"
#define UNRECOGNIZED_COMMAND "0UC"
#define DONE "0DN"

// Global Variables
int  lastSeqNo;
bool done;




/////////////////////////////////////////////////////////////////////////////////////
//                                  initialSetup                                   //
/////////////////////////////////////////////////////////////////////////////////////
void setup(){
  initialSetup();
}


/////////////////////////////////////////////////////////////////////////////////////
//                       Check if command not redundant                            //
/////////////////////////////////////////////////////////////////////////////////////
bool ignore(int seqNo){
  if (seqNo == lastSeqNo) {
    if (done) {
      Serial.println(DONE);
    }
    return true;
  } else {
    lastSeqNo = seqNo;
    done = false;
    return false;
  }
}


/////////////////////////////////////////////////////////////////////////////////////
//                              Instruction Decoding                               //
// This will be changed later on to improve latency issues (2 bytes instead of 7)  //
/////////////////////////////////////////////////////////////////////////////////////

// convert character to integer
int getNumFromChar(char c){
  int r = (int)c - (int)'0';
  return r;
}

// deocde signiture byte
int getSig(String c){
  int r = getNumFromChar(c[0]);
  return r;
}

// decode sequence byte
int getSeqNo(String c){
  int r = getNumFromChar(c[6]);
  return r;
}

// decode opcode byte
int getOpcode(String c){
  int r = getNumFromChar(c[1]);
  return r;
}

// decode arguemnt byte
int getArg(String c){
  int r1 = getNumFromChar(c[2]);
  int r2 = getNumFromChar(c[3]);
  int r3 = getNumFromChar(c[4]);
  return ((r1*100)+(r2*10)+r3);
}

// decode checksum byte and calculate if checksum byte is correct
int check_checksum(String c, int opcode, int arg){
  int checksum = getNumFromChar(c[5]);
  int checksum_recalculated = (opcode + arg) % 10;
  
  if(checksum == checksum_recalculated){
    return 1;
  }
  else {
    return 0;
  }
  
}

// Should empty serial buffer
// juastus has said otherwise, TEST and find solution if not.
void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
//                              Main loop                                             //
////////////////////////////////////////////////////////////////////////////////////////


void loop(){
  
  // poll motor encoder board for readings and save them
  //updateMotorPositions();

  // if message is available on our frequency accept it.
  if(Serial.available() > 0){
    
    // save message sent from PC to STRING c.
    String c = Serial.readString();
    c.trim();

    // if mesage is incomplete send error message and flush serial 
    if(c.length() != 7){
      Serial.println("0IW");
      serialFlush();
      return;
    }
    
    // initial instruction decode
    int sig = getSig(c);
    int seqNo = getSeqNo(c);
      
    // Quits if sig belongs to other teams
    // OR if command is redundant (i.e. already executed)
    if(sig != 0){ return; }
    if(ignore(seqNo)){ return; }

    // completed after sig and ignore checks to avoid unessacary computation
    // decodes instructions, fetches opcode, argument and checks the entire message
    int opcode = getOpcode(c);
    int arg = getArg(c);

    // flag that shows if command was recognized
    bool recognized = true;
    
    // if checksum is correct continue decoding message and execute
    int check = check_checksum(c, opcode, arg);
    if(check == 1){
    
      switch (opcode){

        case STOP:        robotStop();
        break;
        
        case FORWARD:     robotForwardDistance(arg);
        break;

        case BACKWARD:    robotBackwardDistance(arg);
        break;

        case LEFT:        robotTurnAntiClockwise(arg);
        break;

        case RIGHT:       robotTurnClockwise(arg);
        break;

        case KICK:        robotKick(arg);
        break;

        case GRAB:        robotGrab(arg);
        break;
        
        case OPEN:        robotOpen(arg);
        break;
        
        case CLOSE:       robotClose(arg);
        break;
          
        default:          Serial.println(UNRECOGNIZED_COMMAND); recognized = false;
        break;
      
      } // end of switch 
      if (recognized) { done = true; }
      else { lastSeqNo = not lastSeqNo; }
    } // end of if checksum
    else{ Serial.println(CHECKSUM_FAILED);
    lastSeqNo = not lastSeqNo;  } // checksum failed

  } // end of if serial.avalaible 
} // end of loop body

