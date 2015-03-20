#pragma once
#include <stdio.h>
#include <yarp/sig/Vector.h>
#include <yarp/os/RateThread.h>
#include <yarp/os/Time.h>

#include "iCubCtrl.h"
#include "Wiimote.h"

#include "SimpleTest.h"//TESTCLASS

using namespace yarp::os;
using namespace yarp::sig;

class ThreadCtrl : public RateThread
{
private:
	Wiimote *_wiimote;
	iCubCtrl *_icub;
public:
	ThreadCtrl(const double period, Wiimote *wiimote, iCubCtrl *icub);
	~ThreadCtrl(void);
	virtual bool threadInit();
	virtual void beforeStart();
	virtual void afterStart(bool s);
	virtual void run();
	virtual void threadRelease();

	void initThread(ThreadCtrl *thr);
};
