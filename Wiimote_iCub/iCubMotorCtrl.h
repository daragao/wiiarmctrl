#pragma once

#include <string>

#include <yarp/os/Network.h>
#include <yarp/os/Time.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/sig/Vector.h>

//#include <yarp/math/Math.h>
#include <iCub/ctrl/math.h>

#include "iCubGeneric.h"

using namespace std;

using namespace yarp::dev;
using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp;

//using namespace yarp::math;
using namespace iCub::ctrl;

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
	Vector _jointMotors[3],_angleMultiplier[3];
	void iCubMotorCtrl::initVars();
	bool initRobot(string localPortPrefix,string robotName, string partName);
	bool moveMotor(int motorIndex, double angleDeg);
};
