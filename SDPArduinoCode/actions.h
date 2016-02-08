

#ifndef HEADER_ACTIONS
  #define HEADER_ACTIONS

	// motor numberes on power I/O board
	#define TURNING_MOTOR 0
	#define FRONT_LEFT_MOTOR 1
	#define FRONT_RIGHT_MOTOR 2
	#define ACTION_MOTOR 3

	/*// should do this!
	int coordinateX;
	int coordinateY;
	int angle;*/

	void initialSetup(void);
	void robotStop(void);
	void robotKick(int power);
	void robotGrab(int power);
	void robotForwardDistance(int distance);
	void robotBackwardDistance(int distance);
	void RobotTurnClockwise(int degrees);
	void robotTurnAntiClockwise(int degrees);

#endif
