#include <stdlib.h>
#include <Wire.h> // never remove this, for some reason it is needed here!
#include "actions.h"
#include "comm_handler.h"


// Inbound message definitions
#define STOP 0
#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4
#define KICK 5 
// #define GRAB 7
// #define STORE 7
#define OPEN_GRABBER 6
#define CLOSE_GRABBER 7

#define READ_COMPASS 8
#define READ_INFRARED 9
#define READ_SONAR 10
#define SCALE_LEFT 11
#define SCALE_RIGHT 12

#define PING 14
#define GET_INFO 15
#define FORWARD_SLOW 16
// #define RESEND_2_ACK 17

int counter = 0;

/////////////////////////////////////////////////////////////////////////////////////
//                                  initialSetup                                   //
/////////////////////////////////////////////////////////////////////////////////////
void setup(){

  // sensor setup : actions.cpp/.h
  initialSetup();

}

////////////////////////////////////////////////////////////////////////////////////////
//                              Main loop                                             //
////////////////////////////////////////////////////////////////////////////////////////

void loop(){

  // read input from RF 
  int message[3] = {0,0,0};
  bool check = read(message);

  // save input from RF and use accordinly
  bool regonized =  true;
  int opcode = message[0];
  int arg = message[1];
  int seqNo = message[2];


    if(check){
    
      switch (opcode){

        case STOP:              robotStop();
        break;
        
        case FORWARD:           robotForwardDistance(arg);
        break;

        case BACKWARD:          robotBackwardDistance(arg);
        break;

        case LEFT:              robotTurnAntiClockwise(arg);
        break;

        case RIGHT:             robotTurnClockwise(arg);
        break;

        case KICK:              robotKick(arg);
        break;

        case OPEN_GRABBER:      openGrabber(arg);
        break;
        
        case CLOSE_GRABBER:     closeGrabber(arg);
        break;


        case READ_COMPASS:      readCompass();
        break;

        case READ_INFRARED:     readInfrared();
        break;

        case READ_SONAR:
        break;

        case SCALE_LEFT:        scaleLeft(arg);
        break;

        case SCALE_RIGHT:       scaleRight(arg);
        break;

        case GET_INFO:          getInfo();
        break;

        case FORWARD_SLOW:     forwardSlow(arg);

        case PING:              ping();
        counter += 1;
        Serial.print("!");
        Serial.println(counter);
        break;
          
        default:         Serial.println("0001"); // corr = 0; seqNo = 0; done = 1; UNKOWN COMAMAND
        regonized = false;
        break;
      }

      // print acknowledgement message to state its completed the execution of the command
      if(regonized){
        // serialFlush(); // empty serial (as only repeated commands will be stored in there at this moement in time)
        if(seqNo == 0){
          Serial.println("001"); // corr = 0; seqNo = 0; done = 1;
        }
        else{
          Serial.println("011"); // corr = 0; seqNo = 1; done = 1;
        }
      }

    } // end of check

  
}
