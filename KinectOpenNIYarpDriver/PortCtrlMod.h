#pragma once

#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include "KinectCtrl.h"

using namespace yarp::os;
using namespace yarp::sig;

class PortCtrlMod: public RFModule
{
public:
	double getPeriod();
	bool updateModule();
	bool respond(const Bottle& command, Bottle& reply);
	bool configure(ResourceFinder &rf);
	bool interruptModule();
	bool close();
private:
	BufferedPort<Bottle> *_handlerPort;
	KinectCtrl *_kinect;
};
