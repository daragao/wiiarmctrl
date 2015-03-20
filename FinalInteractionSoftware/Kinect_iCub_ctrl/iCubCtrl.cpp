#include "iCubCtrl.h"

iCubCtrl::iCubCtrl(string localPortName, string remotePortName)
{
	initVars();
	initVelParts(localPortName,remotePortName);
}

iCubCtrl::~iCubCtrl(void)
{
}

void iCubCtrl::initVars(){
	for(int i = 0 ; i < MAX_PARTS; i++){
		_partMotor[i].axes = -1;
		_partMotor[i].encoders = NULL;
		_partMotor[i].limitsCtrl = NULL;
		_partMotor[i].polyDriver = NULL;
		_partMotor[i].velocityCtrl = NULL;
	}
}

bool iCubCtrl::initVelParts(string localPortName, string remotePortName){
	initPartPolyDriver(ARM_RIGHT,localPortName+"/right_arm",remotePortName+"/right_arm");
	initPartPolyDriver(ARM_LEFT,localPortName+"/left_arm",remotePortName+"/left_arm");
	initPartPolyDriver(TORSO,localPortName+"/torso",remotePortName+"/torso");
	initPartPolyDriver(HEAD,localPortName+"/head",remotePortName+"/head");
	return true;
}

bool iCubCtrl::initPartPolyDriver(PartIndex partIndex, string localPortName, string remotePortName){
	PartCtrl_t *partMotor = &_partMotor[partIndex];
	string motorPropertiesStr = "(device remote_controlboard) (local "+localPortName+") (remote "+remotePortName+")";
	Property motorProperties(motorPropertiesStr.c_str());
	partMotor->polyDriver = new PolyDriver(motorProperties);
	if(partMotor->polyDriver==NULL||!partMotor->polyDriver->isValid()) return false;
	partMotor->polyDriver->view(partMotor->velocityCtrl);
	if (partMotor->velocityCtrl==NULL) return false;
	partMotor->polyDriver->view(partMotor->limitsCtrl);
	if (partMotor->limitsCtrl==NULL) return false;
	partMotor->polyDriver->view(partMotor->encoders);
	if (partMotor->encoders==NULL) return false;
	partMotor->velocityCtrl->getAxes(&partMotor->axes);
	return true;
}

void iCubCtrl::close(){
	stopAll();//stop all motors
	for(int i = 0; i < MAX_PARTS; i++)
		if(_partMotor[i].polyDriver!=NULL) _partMotor[i].polyDriver->close();
}

double iCubCtrl::angle2speed(PartCtrl_t *partCtrl, int motorIndex, double angle){
	double resSpeed = 0.0;
	double limMin;
	double limMax;
	double currAngle;
	partCtrl->encoders->getEncoder(motorIndex,&currAngle);
	double anglesDelta;

	partCtrl->limitsCtrl->getLimits(motorIndex,&limMin,&limMax);//CHECK LIMITS AND PREVENT FROM BEING OVERRIDEN
	if(angle > limMax) angle = limMax;
	if(angle < limMin) angle = limMin;
	anglesDelta = abs(angle-currAngle) * 0.5;

	resSpeed = (anglesDelta)*2.0;
	if(anglesDelta < TOLERANCE_TRESHOLD){
		resSpeed = resSpeed * (anglesDelta/TOLERANCE_TRESHOLD);
	}

	double direction = (angle-currAngle)>0?1:-1;//define the direction

	return (resSpeed*direction);
}

bool iCubCtrl::moveToAngle(PartCtrl_t *partCtrl, int motorIndex, double angle){
	double speed = angle2speed(partCtrl,motorIndex,angle);
	IVelocityControl *vel = partCtrl->velocityCtrl;
	if(abs(speed) <= STOP_SPEED) {
		vel->stop();
		return true;
	}
	vel->velocityMove(motorIndex,speed);
	return false;
}

/****************************************************************************
*********************************MOVE_FUNCTIONS******************************
****************************************************************************/
//return true if reached its goal

bool iCubCtrl::moveLeftArm(int motorIndex, double angle){
	return moveToAngle(&_partMotor[ARM_LEFT],motorIndex,angle);
}

bool iCubCtrl::moveRightArm(int motorIndex, double angle){
	return moveToAngle(&_partMotor[ARM_RIGHT],motorIndex,angle);
}

bool iCubCtrl::moveTorso(int motorIndex, double angle){
	return moveToAngle(&_partMotor[TORSO],motorIndex,angle);
}

bool iCubCtrl::moveHead(int motorIndex, double angle){
	return moveToAngle(&_partMotor[HEAD],motorIndex,angle);
}

void iCubCtrl::stopAll(){
	_partMotor[ARM_LEFT].velocityCtrl->stop();
	_partMotor[ARM_RIGHT].velocityCtrl->stop();
	_partMotor[TORSO].velocityCtrl->stop();
	_partMotor[HEAD].velocityCtrl->stop();
}
