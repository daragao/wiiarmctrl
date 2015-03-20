#include "iCubMotorCtrl.h"

iCubMotorCtrl::iCubMotorCtrl(iCubCtrl *icub)
{
	initVars();
	_icub = icub;
}

void iCubMotorCtrl::setAsLeft()
{
	_isLeft = true;
}

void iCubMotorCtrl::setAsRight()
{
	_isLeft = false;
}

iCubMotorCtrl::~iCubMotorCtrl(void)
{
}

void iCubMotorCtrl::toggleActivePart(int partID){
	iCubCtrl *icub = _icub;
	if(partID == 0)
		_areJointsValid = false;
	else _areJointsValid = true;
	switch(partID){
		case 0:
			_angleMultiplier[0] = 0;
			_angleMultiplier[1] = 0;
			_angleMultiplier[2] = 0;
			_actJoints[0] = -1;
			_actJoints[1] = -1;
			_actJoints[2] = -1;
			_angle[0] = 0;
			_angle[1] = 0;
			_angle[2] = 0;
			break;
		case 1:
			if(_isLeft){//left part
				_angleMultiplier[0] = -1;
				_angleMultiplier[1] = 1;
				_angleMultiplier[2] = -1;
			}else{//right part
				_angleMultiplier[0] = -1;
				_angleMultiplier[1] = -1;
				_angleMultiplier[2] = 1;
			}
			_actJoints[0] = 0;
			_actJoints[1] = 2;
			_actJoints[2] = 1;
			resetSavedAngle();
			break;
		case 2:
			if(_isLeft){//left part
				_angleMultiplier[0] = 1;
				_angleMultiplier[1] = 1;
				_angleMultiplier[2] = 1;
			}else{//right part
				_angleMultiplier[0] = 1;
				_angleMultiplier[1] = -1;
				_angleMultiplier[2] = -1;
			}
			_actJoints[0] = 3;
			_actJoints[1] = 4;
			_actJoints[2] = 6;
			resetSavedAngle();
			break;
	}
}

void iCubMotorCtrl::resetSavedAngle(){
	if(!_areJointsValid) return;
	iCubCtrl *icub = _icub;
	_angle[0] = icub->getAngle(_actJoints[0]);
	_angle[1] = icub->getAngle(_actJoints[1]);
	_angle[2] = icub->getAngle(_actJoints[2]);
}

void iCubMotorCtrl::initVars(){
	_isLeft = false;
	_areJointsValid = false;
	toggleActivePart(0);
	_maxspeed = 0;
	_maxacceleration = 0;
}

void iCubMotorCtrl::setSpeed(double speed){
	_maxspeed = speed;
}

double iCubMotorCtrl::getSpeed(){
	return _maxspeed;
}

void iCubMotorCtrl::setAcceleration(double acceleration){
	_maxacceleration = acceleration;
}

double iCubMotorCtrl::getAcceleration(){
	return _maxacceleration;
}

void iCubMotorCtrl::setSpeedMultiplier(double speedmultiplier){
	_speedmultiplier = speedmultiplier;
}

double iCubMotorCtrl::getSpeedMultiplier(){
	return _speedmultiplier;
}

void iCubMotorCtrl::stopMotion(){
	if(!_areJointsValid) return;
	iCubCtrl *icub = _icub;
	icub->stopJoint(_actJoints[0]);
	icub->stopJoint(_actJoints[1]);
	icub->stopJoint(_actJoints[2]);
}

void iCubMotorCtrl::velocityMoveActivePart(double angle1,double angle2,double angle3){
	if(!_areJointsValid) return;
	iCubCtrl *icub = _icub;
	double maxAcc = _maxacceleration;
	double maxSpeed = _maxspeed;
	double speedmultiplier = _speedmultiplier;
	double angleDirection[3] = {0,0,0};
	double startSpeed[3] = {0,0,0};
	double acceleration[3] = {20,20,20};
	//double acceleration[3] = {0,0,0};

	icub->velocityMove(_actJoints[0],angle1*_angleMultiplier[0],maxAcc,maxSpeed);
	icub->velocityMove(_actJoints[1],angle2*_angleMultiplier[1],maxAcc,maxSpeed);
	icub->velocityMove(_actJoints[2],angle3*_angleMultiplier[2],maxAcc,maxSpeed);

	Time::turboBoost();
	//static double _lastTime = Time::now();
	//double currTime = Time::now();
	//double timeDelta = currTime - _lastTime;
	//if(timeDelta==0||timeDelta<0.00001) return;

	/*
	if(angle1>0) angleDirection[0] = 1;
	if(angle1<0) angleDirection[0] = -1;
	if(angle2>0) angleDirection[1] = 1;
	if(angle2<0) angleDirection[1] = -1;
	if(angle3>0) angleDirection[2] = 1;
	if(angle3<0) angleDirection[2] = -1;

	startSpeed[0] = abs(angle1/timeDelta)*speedmultiplier*_angleMultiplier[0]*angleDirection[0];
	startSpeed[1] = abs(angle2/timeDelta)*speedmultiplier*_angleMultiplier[1]*angleDirection[1];
	startSpeed[2] = abs(angle3/timeDelta)*speedmultiplier*_angleMultiplier[2]*angleDirection[2];

	if(abs(startSpeed[0])>maxSpeed) startSpeed[0] = maxSpeed*angleDirection[0]*_angleMultiplier[0];
	if(abs(startSpeed[1])>maxSpeed) startSpeed[1] = maxSpeed*angleDirection[1]*_angleMultiplier[1];
	if(abs(startSpeed[2])>maxSpeed) startSpeed[2] = maxSpeed*angleDirection[2]*_angleMultiplier[2];

	velocityMove(_actJoints[0],acceleration[0],startSpeed[0]);
	velocityMove(_actJoints[1],acceleration[1],startSpeed[1]);
	velocityMove(_actJoints[2],acceleration[2],startSpeed[2]);*/

	/*update time*/
	//_lastTime = currTime;
}

void iCubMotorCtrl::velocityMove(int jointID, double acceleration, double startSpeed){
	iCubCtrl *icub = _icub;
	if(startSpeed == 0)
		icub->stopJoint(jointID);
	else
		icub->velocityMove(jointID,acceleration,startSpeed);
}