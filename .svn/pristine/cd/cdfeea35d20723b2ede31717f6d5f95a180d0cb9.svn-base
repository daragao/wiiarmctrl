#pragma once

#include <yarp/os/all.h>
#include <iostream>
#include <string>

using namespace std;
using namespace yarp::os;

class GraspPortCtrl
{
public:
	GraspPortCtrl(void);
	~GraspPortCtrl(void);
	void initPorts(string graspPortName);
	void closePort();
	void sendCommand(string command);
private:
	    BufferedPort<Bottle> *_graspPort;
};
