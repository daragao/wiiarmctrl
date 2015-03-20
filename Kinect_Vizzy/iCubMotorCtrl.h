#pragma once

#include <string>

#include <yarp/os/Time.h>
#include <yarp/os/Network.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/sig/Vector.h>

#include <math.h>

#include <yarp/math/Math.h>
//#include <iCub/ctrl/math.h>

#include "iCubGeneric.h"

using namespace std;

using namespace yarp::dev;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp;

using namespace yarp::math;
//using namespace iCub::ctrl;

#ifndef M_PI
#define M_PI	3.14159265358979323846
#define M_PI_2	M_PI/2
#endif

class iCubMotorCtrl : public iCubGeneric
{
public:
	iCubMotorCtrl(string localPortPrefix,string robotName,string partName);
	~iCubMotorCtrl(void);
	void close();
	void updatePosition(bool isTracking);
	void setSpeed(double acc, double speed);
private:
	double _speed;
	PolyDriver *_robotDevice;
	IPositionControl *_pos;
	IVelocityControl *_vel;
	IControlLimits *_limits;
	IEncoders *_encs;
	PartName _partName;
	void initVars();
	bool initRobot(string localPortPrefix,string robotName, string partName);
	bool moveMotor(int motorIndex, double angleDeg);
	bool moveMotorPosition(int motorIndex, double angleDeg);
	bool moveMotorVelocity(int motorIndex, double angleDeg);
	void updateLeftArmPosition();
	void updateRightArmPosition();
	void updateTorsoPosition();
	void updateShouldersPosition();
	void updateHeadPosition();
	Matrix makeDHMatrix(double a, double alpha, double d, double theta);
	Vector matrix2Euler(Matrix rMatrix);
	Vector matrix2Euler_XYZ(Matrix rMatrix);
	Vector matrix2Euler_ZYX(Matrix rMatrix);
	Vector matrix2Euler_YZY(Matrix rMatrix);
	Vector matrix2Euler_ZYZ(Matrix rMatrix);
	Vector matrix2Euler_XYX(Matrix rMatrix);
	Vector dcm2euler(const Matrix &R);
};
