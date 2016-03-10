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
#define RESEND_2_ACK 17

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

  //
  int message[3] = {0,0,0};
  bool check = read(message);
  bool regonized =  false;
  int opcode = message[0];
  int arg = message[1];
  int seqNo = message[2];



    if(check){
    
      switch (opcode){

        case STOP:              robotStop();
        regonized = true;
        break;
        
        case FORWARD:           robotForwardDistance(arg);
        regonized = true;
        break;

        case BACKWARD:          robotBackwardDistance(arg);
        regonized = true;
        break;

        case LEFT:              robotTurnAntiClockwise(arg);
        regonized = true;
        break;

        case RIGHT:             robotTurnClockwise(arg);
        regonized = true;
        break;

        case KICK:              robotKick(arg);
        regonized = true;
        break;

        case OPEN_GRABBER:      openGrabber(arg);
        regonized = true;
        break;
        
        case CLOSE_GRABBER:     closeGrabber(arg);
        regonized = true;
        break;


        case READ_COMPASS:      readCompass();
        regonized = true;
        break;

        case READ_INFRARED:     readInfrared();
        regonized = true;
        break;

        case READ_SONAR:
        regonized = true;
        break;

        case SCALE_LEFT:        scaleLeft(arg);
        regonized = true;
        break;

        case SCALE_RIGHT:       scaleRight(arg);
        regonized = true;
        break;

        case GET_INFO:          getInfo();
        regonized = true;
        break;

        case FORWARD_SLOW:     forwardSlow(arg);
        regonized = true;

        case PING:              ping();
        regonized = true;
        counter += 1;
        Serial.print("COUNTER = ");
        Serial.println(counter);

        break;

        case RESEND_2_ACK:    // resend 2nd ack with current seqNo
        if(seqNo == 0){
          Serial.println("001"); // corr = 0; seqNo = 0; done = 1; unregonized command = 0
          }
          else{
            Serial.println("011"); // corr = 0; seqNo = 1; done = 1; unregonized command = 0
          }
        break;
          
        default:         Serial.println("0001"); // corr = 0; seqNo = 0; done = 1; UNKOWN COMAMAND
        break;
      
      }


      // print 2nd acknowledgement message
      if(regonized){
        if(seqNo == 0){
          Serial.println("001"); // corr = 0; seqNo = 0; done = 1;
        }
        else{
          Serial.println("011"); // corr = 0; seqNo = 1; done = 1;
        }
      }

    }

  
}
