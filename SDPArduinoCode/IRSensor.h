#ifndef HEADER_IRSENSOR
  #define HEADER_IRSENSOR


#define IR_PORT_NO 3     // port no for IR (change according to actual port)

  void IRSensorValue(int *IRread);
  void IRSensorPrint(int *IRread);

#endif
