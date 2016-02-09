#ifndef HEADER_ACTIONS
  #define HEADER_ACTIONS

	// motor numberes on power I/O board
	#define FRONT_LEFT_MOTOR 1
	#define FRONT_RIGHT_MOTOR 2
	#define ACTION_MOTOR 3

	// eventually current location: X,Y,angle


	void initialSetup();
	void robotStop();
	void robotKick(int power);
	void robotGrab(int power);
	void robotForwardDistance(int distance);
	void robotBackwardDistance(int distance);
	void robotTurnClockwise(int degrees);
	void robotTurnAntiClockwise(int degrees);
	void robotOpen(int power);
	void robotClose(int power);

#endif
