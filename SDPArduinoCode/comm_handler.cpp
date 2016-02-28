#include <stdlib.h>
#include <Arduino.h>

/////////////////////////////////////////////////////////////////////////////////////
//                       Check if command not redundant                            //
/////////////////////////////////////////////////////////////////////////////////////
// bool ignore(int seqNo){
//   if (seqNo == lastSeqNo) {
//     if (done) {
//       Serial.println(DONE);
//     }
//     return true;
//   } else {
//     lastSeqNo = seqNo;
//     done = false;
//     return false;
//   }
// }


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