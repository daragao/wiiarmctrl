#include "iCubMotorCtrl.h"

iCubMotorCtrl::iCubMotorCtrl(string localPortPrefix,string robotName, string partName)
{
	initVars();
	initRobot(localPortPrefix,robotName,partName);
}

iCubMotorCtrl::~iCubMotorCtrl(void)
{
}

void iCubMotorCtrl::initVars(){
	_pos = NULL;
	_jointMotors[0].resize(3);
	_jointMotors[1].resize(3);
	_jointMotors[2].resize(3);
	_angleMultiplier[0].resize(3);
	_angleMultiplier[1].resize(3);
	_angleMultiplier[2].resize(3);
	//SET JOINTS
	_jointMotors[0] = -1;//this option is not available
	_jointMotors[1][0] = 0;
	_jointMotors[1][1] = 2;
	_jointMotors[1][2] = 1;
	_jointMotors[2][0] = 3;
	_jointMotors[2][1] = 4;
	_jointMotors[2][2] = -1;
	if(_partName==LEFT_ARM){//left part
		_angleMultiplier[1][0] = -1;
		_angleMultiplier[1][1] = 1;
		_angleMultiplier[1][2] = -1;
		_angleMultiplier[2][0] = 1;
		_angleMultiplier[2][1] = 1;
		_angleMultiplier[2][2] = 1;
	}else{//right part
		_angleMultiplier[1][0] = -1;
		_angleMultiplier[1][1] = -1;
		_angleMultiplier[1][2] = 1;
		_angleMultiplier[2][0] = 1;
		_angleMultiplier[2][1] = -1;
		_angleMultiplier[2][2] = -1;
	}
}

void iCubMotorCtrl::close(){
	_robotDevice->close();
}

bool iCubMotorCtrl::initRobot(string localPortPrefix,string robotName, string partName){
	Time::turboBoost();

	if(partName=="right_arm"){
		_partName = RIGHT_ARM;
	}else if(partName=="left_arm"){
		_partName = LEFT_ARM;
	}else if(partName=="torso"){
		_partName = TORSO;
	}else if(partName=="right_leg"){
		_partName = RIGHT_LEG;
	}else if(partName=="left_leg"){
		_partName = LEFT_LEG;
	}

	string localPort = "/" + localPortPrefix+"/motor/"+partName;
	string remotePort = "/" + robotName+"/"+partName;
	Property options;
	options.put("device", "remote_controlboard");
	options.put("local", localPort.c_str());
	options.put("remote", remotePort.c_str());

	PolyDriver *robotDevice = new PolyDriver(options);
	_robotDevice = robotDevice;
	if (!robotDevice->isValid()) {
		printf("Device not available.  Here are the known devices:\n");
		printf("%s", Drivers::factory().toString().c_str());
		return false;
	}

	IPidControl *pids;
	IControlLimits *limits;
	IPositionControl *pos;
	IEncoders *encs;
	IVelocityControl *vel;

	bool ok;
	ok = robotDevice->view(limits);
	ok = ok && robotDevice->view(pos);
	ok = ok && robotDevice->view(encs);
	ok = ok && robotDevice->view(pids);
	ok = ok && robotDevice->view(vel);

	if (!ok) {
		printf("Problems acquiring interfaces\n");
		return 0;
	}

	int nj=0;
	pos->getAxes(&nj);
	Vector encoders;
	Vector command;
	encoders.resize(nj);
	command.resize(nj);

	_limits = limits;
	_pos = pos;
	_encs = encs;
	_vel = vel;


	//first zero all joints
	setSpeed(50,10);
	command=0;
	if(_partName != TORSO && _partName != LEFT_LEG && _partName != RIGHT_LEG){
		command[1]=90;
		command[3]=45;
	}
	_pos->positionMove(command.data());

	return true;
}

void iCubMotorCtrl::setSpeed(double acc, double speed){
	_speed = speed;
	int nj=0;
	_pos->getAxes(&nj);
	Vector tmp;
	tmp.resize(nj);
	int i;
	for (i = 0; i < nj; i++) {
		tmp[i] = acc;
	}
	_pos->setRefAccelerations(tmp.data());
	_vel->setRefAccelerations(tmp.data());
	for (i = 0; i < nj; i++) {
		tmp[i] = speed;
		_pos->setRefSpeed(i, tmp[i]);
	}
}

bool iCubMotorCtrl::moveMotor(int motorIndex, double angleDeg){
	if(motorIndex==-1)	return false;
	bool result = false;
	double limMin;
	double limMax;
	double currAngle;
	double speed = _speed;
	double offset = 10;
	_encs->getEncoder(motorIndex,&currAngle);
	_limits->getLimits(motorIndex,&limMin,&limMax);
	//if(!(limMin>angleDeg||limMax<angleDeg)){
		//if(currAngle+2<angleDeg||angleDeg<currAngle-2)result = _pos->positionMove(motorIndex,angleDeg);
		if(abs(angleDeg-currAngle)>offset){
			speed = _speed;
		}else {
			//_vel->stop(motorIndex);
			//_pos->stop(motorIndex);
			//_vel->velocityMove(motorIndex,0);

			//if(abs(angleDeg-currAngle)<2) speed = 0;
			//else 
				speed = (speed/2) * abs((angleDeg-currAngle)/offset);
		}
		if(currAngle<angleDeg){
			_vel->velocityMove(motorIndex,speed);
		}
		if(currAngle>angleDeg){
			_vel->velocityMove(motorIndex,-speed);
		}
	//}
	return result;
}

void iCubMotorCtrl::updatePosition(bool isTracking){
	double currAngle;
	int controlID = _icubData->controlID;
	if(isTracking){
		Vector angle = _icubData->mpAngleVector;
		//_pos->relativeMove((int)_jointMotors[controlID][0],angle[0]*_angleMultiplier[controlID][0]);
		//_pos->relativeMove((int)_jointMotors[controlID][1],angle[1]*_angleMultiplier[controlID][1]);
		//_pos->relativeMove((int)_jointMotors[controlID][2],angle[2]*_angleMultiplier[controlID][2]);
		moveMotor((int)_jointMotors[controlID][0],angle[0]*_angleMultiplier[controlID][0]);
		moveMotor((int)_jointMotors[controlID][1],angle[1]*_angleMultiplier[controlID][1]);
		moveMotor((int)_jointMotors[controlID][2],angle[2]*_angleMultiplier[controlID][2]);
	}else{
		Time::delay((2.0/1000.0));
		int nj;
		_pos->getAxes(&nj);
		for(int i = 0; i < nj; i++){
			_pos->stop(i);
			_vel->stop(i);
		}
		_encs->getEncoder((int)_jointMotors[controlID][0],&currAngle);
		_icubData->mpAngleVector[0] = currAngle;
		_encs->getEncoder((int)_jointMotors[controlID][1],&currAngle);
		_icubData->mpAngleVector[1] = currAngle;
		_encs->getEncoder((int)_jointMotors[controlID][2],&currAngle);
		_icubData->mpAngleVector[2] = currAngle;
	}
}



