#ifndef HEADER_ACTIONS
  #define HEADER_ACTIONS

	// motor numberes on power I/O board
	#define FRONT_LEFT_MOTOR 1
	#define FRONT_RIGHT_MOTOR 2
	#define ACTION_MOTOR 0

	#define IR_PORT_NO 0     // port no for IR (change according to actual port)


	// eventually current location: X,Y,angle


	void initialSetup();
	void robotStop();
	void robotKick(int power);
	void robotGrab(int power);
	void robotForwardDistance(int distance);
	void robotBackwardDistance(int distance);
	void robotTurnClockwise(int degrees);
	void robotTurnAntiClockwise(int degrees);
	void openGrabber(int power);
	void closeGrabber(int power);
	void ping();

	void readCompass();
	void readInfrared();
	void readSonar();
	void scaleLeft(int scale);
	void scaleRight(int scale);
	void getInfo();

	void forwardSlow(int distance);

#endif
