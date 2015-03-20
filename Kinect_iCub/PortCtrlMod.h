#pragma once

#include <string>

#include "iCubGeneric.h"
#include "iCubMotorCtrl.h"
#include "iCubiKinCtrl.h"

#include <yarp/os/all.h>

#define MAX_ICUBGEN 7

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
	bool _useSkeleton;
	iCubGeneric *_icubGen[MAX_ICUBGEN];
	int _niCubgen;
	bool _isTracking;

	BufferedPort<Bottle> *_handlerPort;
	BufferedPort<Vector> *_christianPort;
};
