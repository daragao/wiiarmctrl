#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/CartesianControl.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/os/Time.h>
#include <yarp/sig/Vector.h>

using namespace std;

using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::os;
using namespace yarp;

class iCubCtrl
{
public:
	iCubCtrl(string robotName, string partName);
	~iCubCtrl(void);
	bool initRobotKinematic();
	bool initRobotMotor();
	void closeRobot();
	void stopJoint(int jointID);
	void stopKinematics();
	void positionMove(int jointID, double acceleration, double speed, double angle);
	void velocityMove(int jointID, double acceleration, double startSpeed);
	bool velocityMove(int jointID, double angleDeg, double acceleration, double speed);
	void setCartTrajTime(double seconds);
	void setCartDOF(int dofSize, ...);
	void cartesianMove(double transX, double transY, double transZ);
	void cartesianMove(double transX, double transY, double transZ, double oriAngle, double oriX, double oriY, double oriZ);
	double getAngle(int jointID);
	bool isOverLimit(int jointID, double angle);
	void getCartesianPose(double posArray[3], double oriArray[4]);
	void getCartesianPose(int joint, double posArray[3], double oriArray[4]);
	void cartesianDesired(double pos[3], double ori[4]);
private:
	string _robot, _part;
	PolyDriver *_robotMotorDevice, *_robotKinematicDevice;
	IPositionControl *_pos;
	IVelocityControl *_vel;
	IEncoders *_enc;
	IPidControl *_pid;
	IAmplifierControl *_amp;
	IControlLimits *_lim;
	ICartesianControl *_armCart;
	int _jnts;
	void initVars();
};
