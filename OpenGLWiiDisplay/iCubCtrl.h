#pragma once

#include <yarp/os/Network.h>
#include <yarp/os/RFModule.h>
#include <yarp/os/RateThread.h>
#include <yarp/os/Time.h>
#include <yarp/sig/Vector.h>
#include <yarp/math/Math.h>

#include <yarp/dev/Drivers.h>
#include <yarp/dev/CartesianControl.h>
#include <yarp/dev/PolyDriver.h>

#include "Utils.h"

using namespace yarp;
using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::math;

class iCubCtrl
{
public:
	static Vector _icubCoords;
	iCubCtrl(void);
	~iCubCtrl(void);
	void init_iCubCtrl();
	void setPose(double x, double y, double z);
	void setPose(Vector point);
	void refresh_iCub();
	double desirableDistance(Vector xd);
	void test();
private:
	static PolyDriver *_clientCartCtrl;
	static ICartesianControl *_armCart;
	void openCartInterface(PolyDriver*& clientCartCtrl, ICartesianControl*& armCart);
	void setPose(ICartesianControl *armCart, Vector xd, Vector od);
	void getPose(ICartesianControl *armCart, Vector &xd, Vector &od);
	void getPose(Vector &xd, Vector &od);
	void initVariables();
};
