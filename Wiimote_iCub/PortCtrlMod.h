#pragma once

#include <string>

#include "iCubGeneric.h"
#include "iCubMotorCtrl.h"
#include "iCubiKinCtrl.h"

#include <yarp/os/all.h>

//VOCABS
#define WII_MOTIONPLUS_VOCAB VOCAB2('M','P')
#define WII_MPSOLVER_VOCAB VOCAB3('S','O','L')
#define WII_BOT_VOCAB VOCAB3('B','O','T')
#define WII_LED_VOCAB VOCAB3('L','E','D')
//BOTS
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
//LEDS
#define WII_LED1_VOCAB VOCAB4('L','E','D','1')
#define WII_LED2_VOCAB VOCAB4('L','E','D','2')
#define WII_LED3_VOCAB VOCAB4('L','E','D','3')
#define WII_LED4_VOCAB VOCAB4('L','E','D','4')
//RUMBLE
#define WII_RUMBLE_VOCAB VOCAB4('R','U','M','B')
//SOLVER
#define WIISOLVER_AXISANGLE_VOCAB VOCAB4('A','X','I','S')
#define WIISOLVER_POINT_VOCAB VOCAB3('P','N','T')
#define WIISOLVER_ROTMAT_VOCAB VOCAB4('R','O','T','M')

#define VERY_LITTLE 0.3

#define MAX_ICUBGEN 7
#define TOTAL_WIIMOTE_PARTS 5

using namespace std;

using namespace yarp::os;

typedef enum WIIMOTE_PART{
	BOTS=0,
	MP=1,
	SOLVERWRITER=2,
	SOLVERREADER=3,
	WIIWRITER=4
}WiimotePart;

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
	bool _kinematicControl;
	iCubGeneric *_icubGen[MAX_ICUBGEN];
	int _niCubgen;
	bool _isTracking;

	BufferedPort<Bottle> *_handlerPort[TOTAL_WIIMOTE_PARTS];

	void setupPorts(string localPortPrefix, string wiiSolverRemotePort, string wiimoteRemotePort);
	void parseBotBottle(Bottle botBottle);
	void parseMPBottle(Bottle mpBottle);
	void parseSolverBottle(Bottle solverBottle);
};
