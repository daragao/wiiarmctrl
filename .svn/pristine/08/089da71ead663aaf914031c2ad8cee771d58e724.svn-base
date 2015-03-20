#pragma once

#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <sstream>
#include <string>

#include "iCubMotorCtrl.h"
#include "iCubIKinCtrl.h"
#include "GraspPortCtrl.h"
#include "Utils.h"

#define VERY_LITTLE 0.1

#define WII_MOTIONPLUS_VOCAB VOCAB2('M','P')

#define WII_MPSOLVER_VOCAB VOCAB3('S','O','L')

#define WII_BOT_VOCAB VOCAB3('B','O','T')
#define WII_LED_VOCAB VOCAB3('L','E','D')

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

#define WII_LED1_VOCAB VOCAB4('L','E','D','1')
#define WII_LED2_VOCAB VOCAB4('L','E','D','2')
#define WII_LED3_VOCAB VOCAB4('L','E','D','3')
#define WII_LED4_VOCAB VOCAB4('L','E','D','4')

#define WII_RUMBLE_VOCAB VOCAB4('R','U','M','B')

//SOLVER
#define WIISOLVER_AXISANGLE_VOCAB VOCAB4('A','X','I','S')
#define WIISOLVER_POINT_VOCAB VOCAB3('P','N','T')
#define WIISOLVER_ROTMAT_VOCAB VOCAB4('R','O','T','M')

using namespace std;

using namespace yarp::os;
using namespace yarp::sig;

class WiiPortCtrlReader;

class WiiPortCtrl
{
public:
	WiiPortCtrl(int wiimoteID, GraspPortCtrl *graspCtrl, iCubMotorCtrl *icub, iCubIKinCtrl *icubKin);
	~WiiPortCtrl(void);
	void closeMotorPorts();
	void setMotionPlus(bool setOn);
	void setRobotPartCtrl(int partID);
	void moveRobotMotor(bool isBotPressed, double mpRaw[3],double mpAngle[3]);
	void moveRobotKinematic(bool isBotPressed, bool doStop, double solverPoint[3], double rotMAtrix[3][3]);
	void moveRelativeRobotKinematic(int pressedBot);
	void setMotorCtrlMode();
	void setKinCtrlMode();
	void addToKinematicRadius(double meters);
	void doGraspAction();
private:
	enum CtrlMode { NONE, MOTOR, KINEMATIC };
	CtrlMode _ctrlMode;
	iCubMotorCtrl *_icubMotor;
	iCubIKinCtrl *_icubKin;
	GraspPortCtrl *_graspCtrl;
	string _wiiPortPrefix, _wiiSolverPortPrefix;
	BufferedPort<Bottle> *_bots,*_mp,*_wiiWriter, *_solver, *_solverWriter;
	WiiPortCtrlReader *_portCtrleader;
	void initVars();
	void openMotorPorts();
	void setLEDs(bool led1, bool led2, bool led3, bool led4);
	void setRumble(bool isOn);
	void setSolverKinematicState(bool isOrientationActive, double pos[3], double ori[4], double rotMAtrix[3][3]);
	void setSolverVector(double x, double y, double z);
	void setSolverAxisAngle(double angle, double x, double y, double z);
	void setRotationMatrix(double rotMAtrix[3][3]);
	BufferedPort<Bottle> *openConnectReaderWiiPort(string remotePortName, string localPortName);
	void resetUpVector(double newRotMatrix[3][3], double solverPoint[3], double rotMatrix[3][3]);
};

class WiiPortCtrlReader : public TypedReaderCallback<Bottle>
{
public:
	WiiPortCtrlReader(WiiPortCtrl *wiiPortCtrl);
	~WiiPortCtrlReader(void);
	virtual void onRead(Bottle& b);
private:
	WiiPortCtrl *_wiiPortCtrl;
	bool _useKinematicButtons, _kinBotPressed;
	bool _botB;
	void initVars();
	void checkButtons(Bottle& b);
	void updateMotionPlusData(Bottle& b);
	void updateSolverData(Bottle& b);
};