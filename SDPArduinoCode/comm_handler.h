/*
refactor out reading from serial
decoding message 
1. decode seqNo (if seqNo != currentSeqNo): send_ACK_1
2. decode [opcode, argument, checksum]
3. if checksum != calculated_checksum: send_corr_1
4. run command with opcode and argument
5. return to listening


Need 2 functions for reading
read_from_loop -> normal read behaviour
read_from_action -> sends "busy" unless global interupt
*/


#ifndef HEADER_COMPASS
  #define HEADER_COMPASS

	bool read(int *message);
	int getCurrentSeqNo();
	void serialFlush();


#endif
