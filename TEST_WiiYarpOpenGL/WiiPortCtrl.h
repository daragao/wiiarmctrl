#pragma once
#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <iostream>
#include <sstream>
#include <string>
#include "GLWindow.h"

#define WII_ACC_VOCAB VOCAB3('A','C','C')
#define WII_IR_VOCAB VOCAB2('I','R')

#define WII_BOT_VOCAB VOCAB3('B','O','T')

#define WII_MOTIONPLUS_VOCAB VOCAB2('M','P')

#define WII_RUMBLE_VOCAB VOCAB4('R','U','M','B')

#define WII_BOTA_VOCAB VOCAB1('A')
#define WII_BOTB_VOCAB VOCAB1('B')
#define WII_BOTUP_VOCAB VOCAB1('U')
#define WII_BOTDOWN_VOCAB VOCAB1('D')
#define WII_BOTRIGHT_VOCAB VOCAB1('R')
#define WII_BOTLEFT_VOCAB VOCAB1('L')
#define WII_BOTHOME_VOCAB VOCAB1('H')
#define WII_BOTMINUS_VOCAB VOCAB1('M')
#define WII_BOTPLUS_VOCAB VOCAB1('P')
#define WII_BOTONE_VOCAB VOCAB1('1')
#define WII_BOTTWO_VOCAB VOCAB1('2')

#define WII_ICUBSOLVER_VOCAB VOCAB3('S','O','L')

#define WIISOLVER_ANGLE_VOCAB VOCAB3('A','N','G')
#define WIISOLVER_AXISANGLE_VOCAB VOCAB4('A','X','I','S')

using namespace std;

using namespace yarp::os;
using namespace yarp::sig;

class WiiPortCtrlBotsReader;

class WiiPortCtrl
{
public:
	WiiPortCtrl(int wiimoteID, GLWindow *glWin);
	~WiiPortCtrl(void);
	void closePorts();
	BufferedPort<Bottle> *getReader();
	BufferedPort<Bottle> *getSolverReader();
private:
	GLWindow *_glWin;
	WiiPortCtrl *_wiiPortCtrl;
	BufferedPort<Bottle> *_bots,*_reader,*_mp,*_solver,*_solverInput;
	WiiPortCtrlBotsReader *_botsReader;
	void openPorts(int wiimoteID);
};

class WiiPortCtrlBotsReader : public TypedReaderCallback<Bottle>
{
public:
	WiiPortCtrlBotsReader(WiiPortCtrl *wiiPortCtrl, GLWindow *glWin);
	~WiiPortCtrlBotsReader(void);
	virtual void onRead(Bottle& b);
private:
	GLWindow *_glWin;
	WiiPortCtrl *_wiiPortCtrl;
	double normVector(double newVector[3]);
};
