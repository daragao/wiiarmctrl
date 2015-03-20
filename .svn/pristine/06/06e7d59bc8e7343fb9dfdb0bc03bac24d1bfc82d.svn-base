#pragma once
#include <stdio.h>
#include <yarp/os/Network.h>
#include <yarp/sig/Vector.h>
#include <yarp/sig/Matrix.h>
#include <yarp/os/Time.h>
#include <yarp/os/RateThread.h>

#include <yarp/dev/Drivers.h>
#include <yarp/dev/CartesianControl.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/PolyDriver.h>

#include "Util.h"

using namespace yarp::sig;
using namespace yarp::dev;
using namespace yarp::os;
using namespace yarp;

class iCubCtrl;

class iCubCtrlThread: public RateThread
{
private:
	iCubCtrl *_icub;
public:
	iCubCtrlThread( iCubCtrl *icub, const double period);
	virtual bool threadInit();
	virtual void beforeStart();
	virtual void afterStart(bool s);
	virtual void run();
	virtual void threadRelease();
};

class iCubCtrl
{
public:
	iCubCtrl(void);
	~iCubCtrl(void);
	bool initDevice();
	bool refreshICub();
private:
	Vector rotationMatrix(double ro, double teta, double psy, Vector posV);
	void initThread();
	bool initIkinDevice();
	bool initMotorDevice();
	Property setProperties(string robotName, string part, string device, string localPorts);
	void moveIkin(ICartesianControl * part);
	void moveMotors(PolyDriver *robotDevice);
	ICartesianControl *_ikinRightArm;
	ICartesianControl *_motorRightArm;
	PolyDriver *_robotDeviceIkinRightArm;
	PolyDriver *_robotDeviceMotorRightArm;
	iCubCtrlThread *_thr;
};
