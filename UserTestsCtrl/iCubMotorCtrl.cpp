#include "iCubMotorCtrl.h"

iCubMotorCtrl::iCubMotorCtrl(string localPortPrefix,string robotName, string partName)
{
	_localPortPrefix = localPortPrefix;
	_robotName = robotName;
	_partName = partName;
	Time::turboBoost();
	initVars();
	initRobot(localPortPrefix,robotName,partName);
}

iCubMotorCtrl::~iCubMotorCtrl(void)
{
}

void iCubMotorCtrl::initVars(){
	_pos = NULL;
}

void iCubMotorCtrl::close(){
	_robotDevice->close();
}

bool iCubMotorCtrl::initRobot(string localPortPrefix,string robotName, string partName){
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
	goToHome();
	/*setSpeed(50,10);
	command=0;
	command[1]=90;
	command[3]=45;
	_pos->positionMove(command.data());*/

	return true;
}

void iCubMotorCtrl::reconnect(){
	close();
	initVars();
	initRobot(_localPortPrefix,_robotName,_partName);
}

void iCubMotorCtrl::goToHome(){
	int nj=0;
	_pos->getAxes(&nj);
	Vector command(nj);
	setSpeed(50,10);
	command=0;
	command[1]=90;
	command[3]=45;
	_pos->positionMove(command.data());
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
	bool result = false;
	double limMin;
	double limMax;
	double currAngle;
	double speed = _speed;
	double offset = 10;
	_encs->getEncoder(motorIndex,&currAngle);
	_limits->getLimits(motorIndex,&limMin,&limMax);
	_pos->positionMove(motorIndex,angleDeg);
	return result;
}

//UTILS-----------------------

//http://www.geometrictools.com/Documentation/EulerAngles.pdf
Vector iCubMotorCtrl::matrix2Euler(Matrix rMatrix){
	Vector eulerAngles(3);
	// Assuming the angles are in radians.
	//RxRyRz
	double thetaX,thetaY,thetaZ;
	if (rMatrix[0][2] < 1) {
		if (rMatrix[0][2] > -1){

			thetaY = asin(rMatrix[0][2]);
			thetaX = atan2(-rMatrix[1][2],rMatrix[2][2]);
			thetaZ = atan2(-rMatrix[0][1],rMatrix[0][0]);
		}else{// rMatrix[0][2] == -1	
			// Not a unique solution: thetaZ - thetaX = atan2(r10,r11)
			thetaY = -M_PI_2;
			thetaX = -atan2(rMatrix[1][0],rMatrix[1][1]);
			thetaZ = 0;
		}
	}else{// rMatrix[0][2] == 1

		// Not a unique solution: thetaZ + thetaX = atan2(r10,r11)
		thetaY = +M_PI_2;
		thetaX = atan2(rMatrix[1][0],rMatrix[1][1]);
		thetaZ = 0;
	}
	eulerAngles[0] = thetaX;
	eulerAngles[1] = thetaY;
	eulerAngles[2] = thetaZ;
	return eulerAngles;
}
