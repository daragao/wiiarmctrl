#pragma once

#include <yarp/os/all.h>

#include "GLWindow.h"

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
	void prepapreGLWindow(int argc, char *argv[]);
private:
	BufferedPort<Bottle> *_handlerPort;
	GLWindow *_glWindow;
};
