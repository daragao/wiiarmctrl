#include "iCubiKinCtrl.h"

iCubiKinCtrl::iCubiKinCtrl(string localPortPrefix,string robotName,string partName)
{
	initVars();
	initRobot(localPortPrefix,robotName,partName);
}

iCubiKinCtrl::~iCubiKinCtrl(void)
{
}

void iCubiKinCtrl::initVars(){
	_clientCartCtrl = NULL;
	_partCart = NULL;
}

void iCubiKinCtrl::close(){
	if(_clientCartCtrl != NULL)
		_clientCartCtrl->close();
}

bool iCubiKinCtrl::initRobot(string localPortPrefix,string robotName, string partName){
	YARP_REGISTER_DEVICES(icubmod)

	string remotePort = "/" + robotName + "/cartesianController/" + partName;
	string localPort = "/" + localPortPrefix + "/iKin/" + partName;

	Property option;
	option.put("device","cartesiancontrollerclient");
	option.put("remote",remotePort.c_str());
	option.put("local",localPort.c_str());

	_clientCartCtrl = new PolyDriver(option);

	//view part to control
	_partCart = NULL;
	if(!_clientCartCtrl->isValid())
		return false;
	_clientCartCtrl->view(_partCart);

	_partCart->setTrajTime(1);

	return true;
}

void iCubiKinCtrl::updatePosition(bool isTracking){
	static bool setPositionZero = true;
	static long lastTimestamp = -1;
	static Vector kinectPosZero;
	static Vector icubPosZero,icubOriZero;
	Vector moveTo;
	moveTo.resize(3);
	if(lastTimestamp !=_icubData->lastBottleTimestamp){
		if(setPositionZero){
			kinectPosZero.resize(3);
			kinectPosZero[0] = _icubData->handRealPos[0];
			kinectPosZero[1] = _icubData->handRealPos[1];
			kinectPosZero[2] = _icubData->handRealPos[2];
			icubPosZero.resize(3);
			icubOriZero.resize(4);
			_partCart->getPose(icubPosZero,icubOriZero);
			setPositionZero = false;
		}
		moveTo[0] = icubPosZero[0] + (kinectPosZero[2]-_icubData->handRealPos[2])/1000;
		moveTo[1] = icubPosZero[1] + (kinectPosZero[0]-_icubData->handRealPos[0])/1000;
		moveTo[2] = icubPosZero[2] - (kinectPosZero[1]-_icubData->handRealPos[1])/1000;
		printf("pos(%f,%f,%f)\n",moveTo[0],moveTo[1],moveTo[2]);
		_partCart->goToPosition(moveTo);
	}
	lastTimestamp = _icubData->lastBottleTimestamp;
}
