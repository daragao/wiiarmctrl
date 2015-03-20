#include "WiiPortCtrl.h"

WiiPortCtrl::WiiPortCtrl(int wiimoteID, string wiimotePortPrefix)
{
	_readerCtrlClass = new WiiPortCtrlReader(wiimoteID);
	openPorts(wiimoteID,wiimotePortPrefix);
}

WiiPortCtrl::~WiiPortCtrl(void)
{
}

void WiiPortCtrl::openPorts(int wiimoteID, string wiimotePortPrefix){
	//wirters
	stringstream out;
	out << wiimoteID;
	string portPrefix = "/" + wiimotePortPrefix + out.str();//"/wiimote"+out.str();
	BufferedPort<Bottle> *port;
	port = new BufferedPort<Bottle>();
	port->open((portPrefix+"/bots").c_str());
	_bots = port;
	port = new BufferedPort<Bottle>();
    port->open((portPrefix+"/sensors").c_str());
	_sensors = port;
	port = new BufferedPort<Bottle>();
    port->open((portPrefix+"/ext/motionplus").c_str());
	_mp = port;
	port = new BufferedPort<Bottle>();
    port->open((portPrefix+"/status").c_str());
	_status = port;
	//reader
	port = new BufferedPort<Bottle>();
	port->open((portPrefix+"/reader").c_str());
	port->useCallback(*_readerCtrlClass);
	_reader = port;
}

void WiiPortCtrl::writeStatusData(long eventNum, bool isConnected, bool isLEDOn[4], float battery_level){
	writeStatusData(_status,eventNum,isConnected,isLEDOn,battery_level);
}

void WiiPortCtrl::writeStatusData(BufferedPort<Bottle> *port, long eventNum, bool isConnected, bool isLEDOn[4], float battery_level){
	if(isConnected){
		int totalLEDs = (isLEDOn[0]?1:0) + (isLEDOn[1]?1:0) + (isLEDOn[2]?1:0) + (isLEDOn[3]?1:0);
		Bottle& output = port->prepare();
		output.clear();
		output.addVocab(WII_STATUS_VOCAB);
		output.addDouble(eventNum);
		output.addDouble(battery_level);
		output.addInt(totalLEDs);
		if(isLEDOn[0]) output.addVocab(WII_LED1_VOCAB);
		if(isLEDOn[1]) output.addVocab(WII_LED2_VOCAB);
		if(isLEDOn[2]) output.addVocab(WII_LED3_VOCAB);
		if(isLEDOn[3]) output.addVocab(WII_LED4_VOCAB);
		port->write();
	}
}

void WiiPortCtrl::writeAccData(Vector accGforce, Vector accOri){
	writeAccData(_sensors,accGforce,accOri);
}

void WiiPortCtrl::writeAccData(BufferedPort<Bottle> *port, Vector accGforce, Vector accOri){
	Bottle& output = port->prepare();
    output.clear();
	output.addVocab(WII_ACC_VOCAB);
	output.addDouble(accGforce[0]);
	output.addDouble(accGforce[1]);
	output.addDouble(accGforce[2]);
	output.addDouble(accOri[0]);
	output.addDouble(accOri[1]);
	output.addDouble(accOri[2]);
    port->write();
}

void WiiPortCtrl::writeIRData(bool isIRVisible[4], Vector irSource[4], Vector irCursor){
	writeIRData(_sensors,isIRVisible,irSource,irCursor);
}

void WiiPortCtrl::writeIRData(BufferedPort<Bottle> *port, bool isIRVisible[4], Vector irSource[4], Vector irCursor){
	int visibleIR = (isIRVisible[0]?1:0) + (isIRVisible[1]?1:0) + (isIRVisible[2]?1:0) + (isIRVisible[3]?1:0);
	if(!visibleIR) return;
	Bottle& output = port->prepare();
    output.clear();
	output.addVocab(WII_IR_VOCAB);
	output.addInt(visibleIR);
	for(int i = 0; i < 4; i++){
		if(isIRVisible[i]){
			output.addInt(i);
			output.addDouble(irSource[i][0]);
			output.addDouble(irSource[i][1]);
			output.addDouble(irSource[i][2]);
		}
	}
	output.addDouble(irCursor[0]);
	output.addDouble(irCursor[1]);
	output.addDouble(irCursor[2]);
    port->write();
}

void WiiPortCtrl::writeBotState(bool botA, bool botB, 
								bool botUP, bool botDOWN, bool botLEFT, bool botRIGHT, 
								bool botMINUS, bool botPLUS, bool botONE, bool botTWO, bool botHOME){
	writeBotState(_bots,botA,botB,botUP,botDOWN,botLEFT,botRIGHT,botMINUS,botPLUS,botONE,botTWO,botHOME);
}

void WiiPortCtrl::writeBotState(BufferedPort<Bottle> *port, bool botA, bool botB, 
								bool botUP, bool botDOWN, bool botLEFT, bool botRIGHT, 
								bool botMINUS, bool botPLUS, bool botONE, bool botTWO, bool botHOME)
{
		Bottle& output = port->prepare();
		output.clear();
		output.addVocab(WII_BOT_VOCAB);
		if(botA) output.addVocab(WII_BOTA_VOCAB);
		if(botB) output.addVocab(WII_BOTB_VOCAB);
		if(botUP) output.addVocab(WII_BOTUP_VOCAB);
		if(botDOWN) output.addVocab(WII_BOTDOWN_VOCAB);
		if(botLEFT) output.addVocab(WII_BOTLEFT_VOCAB);
		if(botRIGHT) output.addVocab(WII_BOTRIGHT_VOCAB);
		if(botMINUS) output.addVocab(WII_BOTMINUS_VOCAB);
		if(botPLUS) output.addVocab(WII_BOTPLUS_VOCAB);
		if(botONE) output.addVocab(WII_BOTONE_VOCAB);
		if(botTWO) output.addVocab(WII_BOTTWO_VOCAB);
		if(botHOME) output.addVocab(WII_BOTHOME_VOCAB);
		port->write();
}

void WiiPortCtrl::writeMotionPlusData(double motionPlusCalibrationStatus, Vector mpRaw, Vector mpAngle){
	writeMotionPlusData(_mp,motionPlusCalibrationStatus,mpRaw,mpAngle);
}

void WiiPortCtrl::writeMotionPlusData(BufferedPort<Bottle> *port, double motionPlusCalibrationStatus, Vector mpRaw, Vector mpAngle){
	Bottle& output = port->prepare();
    output.clear();
	output.addVocab(WII_MOTIONPLUS_VOCAB);
	output.addDouble(motionPlusCalibrationStatus);
	output.addDouble(mpRaw[0]);
	output.addDouble(mpRaw[1]);
	output.addDouble(mpRaw[2]);
	output.addDouble(mpAngle[0]);
	output.addDouble(mpAngle[1]);
	output.addDouble(mpAngle[2]);
    port->write();
}

WiiPortCtrlReader *WiiPortCtrl::getWiiPortCtrlReader(){
	return _readerCtrlClass;
}

void WiiPortCtrl::closePorts(){
	if(!_bots->isClosed()) _bots->close();
	if(!_mp->isClosed()) _mp->close();
	if(!_sensors->isClosed()) _sensors->close();
	if(!_reader->isClosed()) _reader->close();
	if(!_status->isClosed()) _status->close();
}



//WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader
//WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader
//WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader||WiiPortCtrlReader


WiiPortCtrlReader::WiiPortCtrlReader(int wiimoteID)
{
	initVar(wiimoteID);
}

WiiPortCtrlReader::~WiiPortCtrlReader(void)
{
}

void WiiPortCtrlReader::initVar(int wiimoteID){
	_acc = false;
	_ir = false;
	_mp = false;
	_led1 = wiimoteID==0?true:false;
	_led2 = wiimoteID==1?true:false;
	_led3 = wiimoteID==2?true:false;
	_led4 = wiimoteID==3?true:false;
	_rumble = false;
}

void WiiPortCtrlReader::onRead(Bottle& b){
	wiiBottleSwitch(b);
	return;
}

bool WiiPortCtrlReader::wiiBottleSwitch(Bottle& b){
	bool vocabFound = true;
	int vocab = b.get(0).asVocab();
	switch(vocab){
		case WII_ACC_VOCAB:
			setAccelerometer(b.get(1).asInt());
			break;
		case WII_IR_VOCAB:
			setIR(b.get(1).asInt());
			break;
		case WII_MOTIONPLUS_VOCAB:
			setMotionPlus(b.get(1).asInt());
			break;
		case WII_LED_VOCAB:
			setLEDs(b.get(1).asInt(),b.get(2).asInt(),b.get(3).asInt(),b.get(4).asInt());
			break;
		case WII_RUMBLE_VOCAB:
			setRumble(b.get(1).asInt());
			break;
		default:
			vocabFound = false;
			break;
	}
	return vocabFound;
}

void WiiPortCtrlReader::setRumble(int isRumbleOn){
	_rumble = (isRumbleOn==0?false:true);
}

bool WiiPortCtrlReader::getRumble(){
	return _rumble;
}

void WiiPortCtrlReader::setAccelerometer(int isAccOn){
	_acc = (isAccOn==0?false:true);
}

bool WiiPortCtrlReader::getAccelerometer(){
	return _acc;
}

void WiiPortCtrlReader::setIR(int isIROn){
	_ir = (isIROn==0?false:true);
}

bool WiiPortCtrlReader::getIR(){
	return _ir;
}

void WiiPortCtrlReader::setMotionPlus(int isMPOn){
	_mp = (isMPOn==0?false:true);
}

bool WiiPortCtrlReader::getMotionPlus(){
	return _mp;
}

void WiiPortCtrlReader::setLEDs(int led1, int led2, int led3, int led4){
	_led1 = (led1==0?false:true);
	_led2 = (led2==0?false:true);
	_led3 = (led3==0?false:true);
	_led4 = (led4==0?false:true);
}

bool WiiPortCtrlReader::getLED(int ledNum){
	switch(ledNum){
		case 1:
			return _led1;
		case 2:
			return _led2;
		case 3:
			return _led3;
		case 4:
			return _led4;
		default:
			return false;
	}
}