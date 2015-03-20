#pragma once

#include <iostream>
#include <string>
#include <cmath>

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

using namespace std;

using namespace yarp::dev;
using namespace yarp::os;

#define TOLERANCE_TRESHOLD 10.0
#define STOP_SPEED 0.1
#define MAX_PARTS 4

class iCubCtrl
{
public:
	iCubCtrl(string localPortName, string remotePortName);
	~iCubCtrl(void);
	void close();
	//	MOVE FUNCTIONS
	bool moveLeftArm(int motorIndex, double angle);
	bool moveRightArm(int motorIndex, double angle);
	bool moveTorso(int motorIndex, double angle);
	bool moveHead(int motorIndex, double angle);
	void stopAll();
private:
	enum PartIndex { ARM_RIGHT = 0, ARM_LEFT = 1, TORSO = 2, HEAD = 3 };
	struct PartCtrl_t {
		int axes;
		IVelocityControl *velocityCtrl;
		PolyDriver *polyDriver;
		IControlLimits *limitsCtrl;
		IEncoders *encoders;
	} _partMotor [MAX_PARTS];
	void initVars();
	bool initVelParts(string localPortName, string remotePortName);
	bool initPartPolyDriver(PartIndex partIndex, string localPortName, string remotePortName);
	bool moveToAngle(PartCtrl_t *partCtrl, int motorIndex, double angle);
	double angle2speed(PartCtrl_t *partCtrl, int motorIndex, double angle);
};
