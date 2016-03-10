#include <stdlib.h>
#include <Arduino.h>
#include "comm_handler.h"


  // Global Variables
  int currentSeqNo = 0;


/////////////////////////////////////////////////////////////////////////////////////
//                              Instruction Decoding                               //
// This will be changed later on to improve latency issues (2 bytes instead of 7)  //
/////////////////////////////////////////////////////////////////////////////////////

int getCurrentSeqNo(){
  return currentSeqNo;
}

// convert character to integer
int getNumFromChar(char c){
  int r = (int)c - (int)'0';
  return r;
}

// decode sequence byte
int getSeqNo(String c){
  int r = getNumFromChar(c[6]);
  return r;
}

// decode opcode byte
int getOpcode(String c){
  int r10 = getNumFromChar(c[0]);
  int r1 = getNumFromChar(c[1]);
  int r = (10*r10) + r1;
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


void serialFlush(){
  while(Serial.available() > 0) {
    char t = Serial.read();
  }
}



bool read(int *message){
  
  bool check = false;
  int opcode = -1;
  int arg = -1;
  message[0] = opcode;
  message[1] = arg;
  message[2] = currentSeqNo;

    // if message is available on our frequency accept it.
  if(Serial.available() > 0){
    
    // save message sent from PC to STRING c.
    String c = Serial.readString();
    c.trim();

    // if mesage is incomplete send error message and flush serial 
    if(c.length() != 7){
      // Serial.println(c);
      Serial.println("100");  // corr = 1; seqNo = 0 (doesnt matter); done = 0; unregonized command = 1
      serialFlush();
      return false;
    }
    
    // initial instruction decode
    int instructionSeqNo = getSeqNo(c);
    int opcode = getOpcode(c);
    int arg = getArg(c);
    int check = check_checksum(c, opcode, arg);

    if(check == 1){

      // deals with the case the 2nd acknowledgement is lost! 
      // seqNo already flipped, so flip them again for sending the 2nd ack back to the python script
      if(opcode == 17){
        message[0] = opcode;
        message[1] = arg;
        message[2] = instructionSeqNo;
        return true;
      }

        if(currentSeqNo == instructionSeqNo){
          message[0] = opcode;
          message[1] = arg;
          message[2] = currentSeqNo;
        
          // print 1st acknowledgement message
          if(currentSeqNo == 0){
            Serial.println("000");
            currentSeqNo = 1;
          }

          else{
            Serial.println("010");
            currentSeqNo = 0;
          }

          return true;
        }

        else{
          // first acknowledgement not recieved by python script resend!
          if(currentSeqNo == 0){
            Serial.println("010"); // corr = 0; seqNo = 0; done = 0;
          }
          else{
            Serial.println("000"); // corr = 0; seqNo = 1; done = 0;
          }
            return false;
        }



    }
    else{
      // send corruption in insturction
      // as to resend the last command 
      if(currentSeqNo == 0){
          Serial.println("100"); // corr = 1; seqNo = currentSeqNo; done = 0;
        }
        else{
          Serial.println("110"); // corr = 1, seqNo = currentSeqNo; done = 0;
        }
        return false;
    }
  }
}


