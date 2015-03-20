#pragma once
#include "Util.h"
#include <yarp/os/all.h>
#include <yarp/dev/all.h>
#include <yarp/os/RateThread.h>
#include <yarp/os/Time.h>
#include <yarp/dev/WiimoteInterface.h>

//#ifndef WIN32
	#include "windows.h" 
//#endif

using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;

class Wiimote;

class WiiCtrlThread: public RateThread
{
private:
	Wiimote *_wiimote;
public:
	WiiCtrlThread( Wiimote *wiimote, const double period);
	virtual bool threadInit();
	virtual void beforeStart();
	virtual void afterStart(bool s);
	virtual void run();
	virtual void threadRelease();
};

class Wiimote
{
private:
	PolyDriver *_wiiDriver;
	IWiimote *_wiiSensor;
	WiiCtrlThread *_thr;
	bool _isMotionPlusON;
	void endThread();
	bool initDevice();
public:
	Wiimote();
	~Wiimote(void);
	bool initWiimote();
	void initThread();
	void refreshWiiSensor();
	void refreshBotStates();
	void calibrateWiimote();
	void rebootWiimote();
};