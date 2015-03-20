#pragma once
#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <iostream>
#include <sstream>
#include <string>

#define WII_ACC_VOCAB VOCAB3('A','C','C')
#define WII_IR_VOCAB VOCAB2('I','R')
#define WII_STATUS_VOCAB VOCAB4('S','T','A','T')
#define WII_BOT_VOCAB VOCAB3('B','O','T')

#define WII_MOTIONPLUS_VOCAB VOCAB2('M','P')

#define WII_LED_VOCAB VOCAB3('L','E','D')
#define WII_LED1_VOCAB VOCAB4('L','E','D','1')
#define WII_LED2_VOCAB VOCAB4('L','E','D','2')
#define WII_LED3_VOCAB VOCAB4('L','E','D','3')
#define WII_LED4_VOCAB VOCAB4('L','E','D','4')

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

using namespace std;

using namespace yarp::os;
using namespace yarp::sig;

class WiiPortCtrlReader : public TypedReaderCallback<Bottle>
{
public:
	WiiPortCtrlReader(int wiimoteID);
	~WiiPortCtrlReader(void);
	virtual void onRead(Bottle& b);
	bool getRumble();
	bool getAccelerometer();
	bool getIR();
	bool getMotionPlus();
	bool getLED(int ledNum);
private:
	bool _acc,_ir,_mp,_led1,_led2,_led3,_led4,_rumble;
	void initVar(int wiimoteID);
	bool wiiBottleSwitch(Bottle& b);
	void setAccelerometer(int isAccOn);
	void setIR(int isIROn);
	void setMotionPlus(int isMPOn);
	void setLEDs(int led1, int led2, int led3, int led4);
	void setRumble(int isRumbleOn);
};

class WiiPortCtrl
{
public:
	WiiPortCtrl(int wiimoteID, string wiimotePortPrefix);
	~WiiPortCtrl(void);
	void closePorts();
	void writeAccData(Vector accGforce, Vector accOri);
	void writeIRData(bool isIRVisible[4], Vector irSource[4], Vector irCursor);
	void writeBotState(bool botA, bool botB, bool botUP, bool botDOWN, bool botLEFT, bool botRIGHT, bool botMINUS, bool botPLUS, bool botONE, bool botTWO, bool botHOME);
	void writeStatusData(long eventNum, bool isConnected, bool isLEDOn[4], float battery_level);
	void writeMotionPlusData(double motionPlusCalibrationStatus, Vector mpRaw, Vector mpAngle);
	WiiPortCtrlReader *getWiiPortCtrlReader();
private:
	BufferedPort<Bottle> *_bots,*_sensors,*_mp,*_status,*_reader;
	WiiPortCtrlReader *_readerCtrlClass;
	void openPorts(int wiimoteID, string wiimotePortPrefix);
	void writeAccData(BufferedPort<Bottle> *port, Vector accGforce, Vector accOri);
	void writeIRData(BufferedPort<Bottle> *port, bool isIRVisible[4], Vector irSource[4], Vector irCursor);
	void writeBotState(BufferedPort<Bottle> *port, bool botA, bool botB, bool botUP, bool botDOWN, bool botLEFT, bool botRIGHT, bool botMINUS, bool botPLUS, bool botONE, bool botTWO, bool botHOME);
	void writeStatusData(BufferedPort<Bottle> *port, long eventNum, bool isConnected, bool isLEDOn[4], float battery_level);
	void writeMotionPlusData(BufferedPort<Bottle> *port, double motionPlusCalibrationStatus, Vector mpRaw, Vector mpAngle);
};
