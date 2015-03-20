#pragma once

#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <iostream>
#include <sstream>
#include <string>
#include "WiiSphericalCoord.h"

#define WII_MOTIONPLUS_VOCAB VOCAB2('M','P')

//SOLVER VOCABs
#define WII_ICUBSOLVER_VOCAB VOCAB3('S','O','L')

#define WIISOLVER_AXISANGLE_VOCAB VOCAB4('A','X','I','S')

#define WIISOLVER_POINT_VOCAB VOCAB3('P','N','T')
#define WIISOLVER_ROTMAT_VOCAB VOCAB4('R','O','T','M')

using namespace std;

using namespace yarp::os;
using namespace yarp::sig;

class WiiYarpPortsReader;

class WiiYarpPortsCtrl
{
public:
	WiiYarpPortsCtrl(int wiimoteID, double sensitivity, WiiSphericalCoord *wiiSphericalCoord, double accelerator);
	~WiiYarpPortsCtrl(void);
	void setMotionPlus(bool setOn);
	void updateMotionPlusData(double status, double xRate, double yRate, double zRate, double xAngle, double yAngle, double zAngle);
	void setVectorPosition(double xPos, double yPos, double zPos);
	void setAxisAngle(double angle, double xPos, double yPos, double zPos);
	void setRotationMatrix(double rotMatrix[3][3]);
	void writeDataToPort();
	void closePorts();
private:
	double _veryLittle, _accelerator;
	WiiSphericalCoord *_wiiSphericalCoord;
	WiiYarpPortsReader *_portReader;
	BufferedPort<Bottle> *_mp,*_wiiYarpWriter,*_icubSolverWriter,*_icubSolverReader;
	void initVars();
	void openPorts(int wiimoteID, string classPortPrefix);
	BufferedPort<Bottle> *openWiiYarpPortConnection(int wiimoteID, bool isWriter, string classPortPrefix, string part);
	void setReaderPorts(int wiimoteID, string classPortPrefix);
	void setWriterPorts(int wiimoteID, string classPortPrefix);
};

class WiiYarpPortsReader : public TypedReaderCallback<Bottle>
{
public:
	WiiYarpPortsReader(WiiYarpPortsCtrl *portsCtrl);
	~WiiYarpPortsReader(void);
	virtual void onRead(Bottle& b);
private:
	WiiYarpPortsCtrl *_portsCtrl;
};