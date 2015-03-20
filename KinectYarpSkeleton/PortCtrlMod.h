#pragma once

#include <string>

#include <yarp/os/all.h>

#include "KinectSkeletonTracker.h"

using namespace std;

using namespace yarp::os;

class PortCtrlMod: public RFModule, public TypedReaderCallback<Bottle>
{
public:
	double getPeriod();
	bool updateModule();
	bool respond(const Bottle& command, Bottle& reply);
	bool configure(ResourceFinder &rf);
	bool interruptModule();
	bool close();
	virtual void onRead(Bottle &b);
private:
	BufferedPort<Bottle> *_handlerPort;
	KinectSkeletonTracker *_skeletonTracker;
	void sendKinectData();
};
