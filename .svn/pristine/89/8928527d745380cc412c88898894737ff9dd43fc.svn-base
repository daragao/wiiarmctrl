#pragma once

#include <string>

#include <yarp/dev/CartesianControl.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/sig/Vector.h>
#include <yarp/os/Time.h>

#include <iCub/iKin/iKinFwd.h>

#include <yarp/dev/Drivers.h>
YARP_DECLARE_DEVICES(icubmod)

#include "iCubGeneric.h"

using namespace std;

using namespace yarp;
using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;

class iCubiKinCtrl : public iCubGeneric
{
public:
	iCubiKinCtrl(string localPortPrefix,string robotName,string partName,double trajTime);
	~iCubiKinCtrl(void);
	bool initRobot(string localPortPrefix,string robotName, string partName, double trajTime);
	void close();
	void updatePosition(bool isTracking);
private:
	PolyDriver *_clientCartCtrl;
	ICartesianControl *_partCart;
	void initVars();
};
