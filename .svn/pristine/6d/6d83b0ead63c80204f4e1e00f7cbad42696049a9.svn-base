#pragma once

#include "Utils.h"
#include "iCubCtrl.h"

#include <yarp/os/all.h>
#include <yarp/dev/all.h>
#include <yarp/dev/WiimoteInterface.h>

using namespace yarp::os;
using namespace yarp::dev;
using namespace yarp::sig;

class Wiimote
{
private:
	PolyDriver *_wiiDriver;
	static IWiimote *_wiiSensor;
	bool _isMotionPlusON;
	void initVars();
	void refreshBotStates();
	void calibrateWiimote();
	void rebootWiimote();
public:
	static Vector _acc, _gForce, _mpAngleRate, _mpPointCoords;
	static double _lastRotMat[3][3];
	static float _calibStatus;
	static bool _botB,_botA;
	static long _sequenceNum;
	Wiimote();
	~Wiimote(void);
	bool initDevice();
	bool initWiimote();
	bool refreshWiiSensor();
	Vector getGyroCoords(double gyroCoordsArg[3],double gyroCoordsNormalArg[3]);
};