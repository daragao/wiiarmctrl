#pragma once

#include <string>
#include <stdio.h>
#include <sstream>
#include <ctime>
#include <iostream>
#include <fstream>
#include <yarp/os/all.h>

#include "iCubMotorCtrl.h"

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
	iCubMotorCtrl *_icub;
	BufferedPort<Bottle> *_handlerPort;
	 FILE *_pFile;
	 string _userTestsPortName, _wiimotePortName;
	 time_t ltime; /* calendar time */
	 void writeMiliSeconds();
	 void writeTimeStamp();
};
