#pragma once

#include <yarp/os/RateThread.h>

#include "WiiPortCtrl.h"
#include "Wiimote.h"

using namespace yarp::os;

class WiiThread : public RateThread
{
private:
	Wiimote *_wm;
	WiiPortCtrl **_wiiPorts;
	void portReaderUpdate(int id, Wiimote *wm, WiiPortCtrlReader *wiiPortReader);
public:
	WiiThread(Wiimote *wm, WiiPortCtrl **wiiPorts, double threadPeriod);
	~WiiThread(void);
	virtual bool threadInit();
	virtual void run();
	virtual void threadRelease();
	/*virtual void onStop();
	virtual void beforeStart();
	virtual void afterStart(bool success);*/
};
