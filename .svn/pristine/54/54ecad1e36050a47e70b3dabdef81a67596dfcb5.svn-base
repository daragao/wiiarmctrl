#include "Wiimote.h"

IWiimote	*Wiimote::_wiiSensor;
Vector		Wiimote::_acc, Wiimote::_gForce, Wiimote::_mpAngleRate, Wiimote::_mpPointCoords;
float		Wiimote::_calibStatus;
bool		Wiimote::_botB,Wiimote::_botA;
long		Wiimote::_sequenceNum;
double		Wiimote::_lastRotMat[3][3];

Wiimote::Wiimote()
{
}

Wiimote::~Wiimote(void)
{
	if(_wiiDriver!=NULL)
		_wiiDriver->close();
	printf("iCubCtrl: closing Wiimote!\n");
}

bool Wiimote::initDevice(){
	PolyDriver *polyDriver = new PolyDriver("WiimoteDevice");
	if (!polyDriver->isValid()) {
		printf("WiimoteDevice PolyDriver not available\n"); 
		return false;
	}
	IWiimote *iWiimote;
	polyDriver->view(iWiimote);
	if (iWiimote==NULL) {
		printf("*** Device failed to connect\n");
		return false;
	}
	_wiiDriver = polyDriver;
	_wiiSensor = iWiimote;
	return true;
}

void Wiimote::initVars(){
	_acc.resize(6);
	_acc.zero();
	_gForce.resize(3);
	_gForce.zero();
	_mpAngleRate.resize(4);
	_mpAngleRate.zero();
	_mpPointCoords.resize(6);//mpCoords+mpNormal
	_mpPointCoords.zero();
	_mpPointCoords[0] = 0;
	_mpPointCoords[1] = -1;
	_mpPointCoords[2] = 0;
	_mpPointCoords[3] = 0;
	_mpPointCoords[4] = 0;
	_mpPointCoords[5] = 1;
	_botB = false;
	_botA = false;
	_isMotionPlusON = false;
}

bool Wiimote::initWiimote(){
	initVars();
	_wiiSensor->initController();
	return true;
}

void Wiimote::rebootWiimote(){
	_wiiSensor->close();
	Sleep(1*1000);
	_isMotionPlusON = false;
	initWiimote();
}


bool Wiimote::refreshWiiSensor(){
	Vector mpAngleRate,gForce,acc;
	mpAngleRate.resize(3);
	mpAngleRate.zero();
	gForce.resize(3);
	gForce.zero();
	acc.resize(3);
	acc.zero();
	int id = 0;
	static bool isSensing = false;
	bool anyNews = _wiiSensor->wiiRefresh();
	_sequenceNum = _wiiSensor->getSequenceNumber(0);
	if(anyNews){
		//BUTTONS
		if(_wiiSensor->wiiGetBotState(id,IWiimote::PLUS)){
			_wiiSensor->wiiSetParts(id,1,IWiimote::MOTION_PLUS);
			_isMotionPlusON = true;
		}
		if(_wiiSensor->wiiGetBotState(id,IWiimote::MINUS)){
			_wiiSensor->wiiSetParts(id,0,IWiimote::MOTION_PLUS);
			_isMotionPlusON = false;
		}
		if(_wiiSensor->wiiGetBotState(id,IWiimote::ONE)){
			_wiiSensor->wiiSetParts(id,1, IWiimote::MONTION_SENSE);
			isSensing = true;
		}
		if(_wiiSensor->wiiGetBotState(id,IWiimote::TWO)){
			_wiiSensor->wiiSetParts(id,0, IWiimote::MONTION_SENSE);
			isSensing = false;
		}
		if(_wiiSensor->wiiGetBotState(id,IWiimote::B)) _botB = true;
		else _botB = false;
		if(_wiiSensor->wiiGetBotState(id,IWiimote::A)) _botA = true;
		else _botA = false;

		//DATA
		if(_isMotionPlusON){
			if(_calibStatus < 1){
				_calibStatus = _wiiSensor->calibrateMotionPlus(id);
			}else {
				_wiiSensor->readMotionPlusAngle(id,mpAngleRate);
			}
		}
		if(isSensing){
			_wiiSensor->readGForce(id,gForce);
			_wiiSensor->readAcc(id,acc);
		}
	}
	_mpAngleRate = mpAngleRate;
	_gForce = gForce;
	_acc = acc;
	return false;
}

Vector Wiimote::getGyroCoords(double gyroCoordsArg[3],double gyroCoordsNormalArg[3]){
	double gyro[3] = {0,0,0};
	double gyroCoords[3] = {gyroCoordsArg[0],gyroCoordsArg[1],gyroCoordsArg[2]};
	double gyroCoordsNormal[3] = {gyroCoordsNormalArg[0],gyroCoordsNormalArg[1],gyroCoordsNormalArg[2]};
	static double rotMatrix[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
	Vector gyroCoordsVector;
	gyroCoordsVector.resize(6);
	gyroCoordsVector.zero();
	double gyroVector[3] = {gyroCoordsArg[0],gyroCoordsArg[1],gyroCoordsArg[2]};
	double gyroVectorNormal[3] = {gyroCoordsNormalArg[0],gyroCoordsNormalArg[1],gyroCoordsNormalArg[2]};

	if(_calibStatus == 1){
		gyro[0] = _mpAngleRate[0];
		gyro[1] = _mpAngleRate[1];
		gyro[2] = _mpAngleRate[2];

		//get new coords
		Utils::makeRodriguesRotation(gyroCoords,rotMatrix,gyroVector,gyro);
		//get new coords normal
		Utils::makeRodriguesRotation(gyroCoordsNormal,rotMatrix,gyroVectorNormal,gyro);
	}
	gyroCoordsVector[0] = gyroCoords[0];
	gyroCoordsVector[1] = gyroCoords[1];
	gyroCoordsVector[2] = gyroCoords[2];
	gyroCoordsVector[3] = gyroCoordsNormal[0];
	gyroCoordsVector[4] = gyroCoordsNormal[1];
	gyroCoordsVector[5] = gyroCoordsNormal[2];
	return gyroCoordsVector;
}