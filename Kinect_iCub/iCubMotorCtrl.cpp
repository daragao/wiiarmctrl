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

void iCubMotorCtrl::updateHandJoints(){
	for(int joint=7; joint <= 15; joint++){
		moveMotor(joint,_icubData->jointsHand[joint-7]);
	}
}

void iCubMotorCtrl::updateRightArmPosition(){
	Vector euler;
	euler.resize(3);
	//TORSO
	Matrix torsoM = _icubData->jointsOri[SKEL_TORSO];

	//RIGHT SHOULDER
	Matrix shoulderM = _icubData->jointsOri[SKEL_RIGHT_SHOULDER];
	Matrix relShoulderM = shoulderM*torsoM.transposed();

	euler = matrix2Euler(relShoulderM);
	moveMotor(0,(euler[1]*(180.0/M_PI)));
	moveMotor(1,180-(euler[2]*(180.0/M_PI))-90);
	moveMotor(2,(euler[0]*(180.0/M_PI)));
	//printf("euler(%.2f,%.2f,%.2f)\n",euler[0]*(180.0/M_PI),euler[1]*(180.0/M_PI),euler[2]*(180.0/M_PI));

	//RIGHT ELBOW
	Matrix elbowM = _icubData->jointsOri[SKEL_RIGHT_ELBOW];
	Matrix relElbowM = elbowM*shoulderM.transposed();
	euler = dcm2euler(relElbowM);

	moveMotor(3,(euler[1]*(180.0/M_PI)));

}

void iCubMotorCtrl::updateLeftArmPosition(){
	Vector euler;
	euler.resize(3);
	//TORSO
	Matrix torsoM = _icubData->jointsOri[SKEL_TORSO];

	//LEFT SHOULDER
	Matrix shoulderM = _icubData->jointsOri[SKEL_LEFT_SHOULDER];
	Matrix relShoulderM = shoulderM*torsoM.transposed();

	euler = matrix2Euler(relShoulderM);
	moveMotor(0,-(euler[1]*(180.0/M_PI)));
	moveMotor(1,180+(euler[2]*(180.0/M_PI))-90);
	moveMotor(2,(euler[0]*(180.0/M_PI)));
	//printf("euler(%.2f,%.2f,%.2f)\n",euler[0]*(180.0/M_PI),euler[1]*(180.0/M_PI),euler[2]*(180.0/M_PI));

	//RIGHT ELBOW
	Matrix elbowM = _icubData->jointsOri[SKEL_LEFT_ELBOW];
	Matrix relElbowM = elbowM*shoulderM.transposed();
	euler = dcm2euler(relElbowM);
	moveMotor(3,(euler[1]*(180.0/M_PI)));
}

void iCubMotorCtrl::updateTorsoPosition(){
	Vector euler;
	euler.resize(3);
	//WAIST
	Matrix waistM = _icubData->jointsOri[SKEL_WAIST];

	//TORSO
	Matrix torsoM = _icubData->jointsOri[SKEL_TORSO];
	Matrix relTorsoM = torsoM*waistM.transposed();

	euler = matrix2Euler(torsoM);
	moveMotor(0,(euler[1]*(180.0/M_PI)));
	moveMotor(1,-(euler[2]*(180.0/M_PI)));
	moveMotor(2,(euler[0]*(180.0/M_PI)));
}

void iCubMotorCtrl::updateLeftLegPosition(){
	Vector euler;
	euler.resize(3);
	//PLANE GROUND
	Matrix groundM(3,3);
	groundM.eye();

	//LEFT HIP
	Matrix hipM = _icubData->jointsOri[SKEL_LEFT_HIP];
	Matrix relHipM = hipM*groundM.transposed();

	euler = matrix2Euler(relHipM);
	moveMotor(0,-(euler[0]*(180.0/M_PI)));
	moveMotor(1,(euler[2]*(180.0/M_PI)));
	moveMotor(2,(euler[1]*(180.0/M_PI)));

	//LEFT KNEE
	Matrix kneeM = _icubData->jointsOri[SKEL_LEFT_KNEE];
	Matrix relKneeM = kneeM*hipM.transposed();

	euler = matrix2Euler(relKneeM);
	moveMotor(3,-(euler[0]*(180.0/M_PI)));
	//printf("euler(%.2f,%.2f,%.2f)\n",euler[0]*(180.0/M_PI),euler[1]*(180.0/M_PI),euler[2]*(180.0/M_PI));
}

void iCubMotorCtrl::updateRightLegPosition(){
	Vector euler;
	euler.resize(3);
	//PLANE GROUND
	Matrix groundM(3,3);
	groundM.eye();

	//LEFT HIP
	Matrix hipM = _icubData->jointsOri[SKEL_RIGHT_HIP];
	Matrix relHipM = hipM*groundM.transposed();

	euler = matrix2Euler(relHipM);
	moveMotor(0,-(euler[0]*(180.0/M_PI)));
	moveMotor(1,(euler[2]*(180.0/M_PI)));
	moveMotor(2,(euler[1]*(180.0/M_PI)));

	//LEFT KNEE
	Matrix kneeM = _icubData->jointsOri[SKEL_RIGHT_KNEE];
	Matrix relKneeM = kneeM*hipM.transposed();

	euler = matrix2Euler(relKneeM);
	moveMotor(3,-(euler[0]*(180.0/M_PI)));
}

void iCubMotorCtrl::updatePosition(bool isTracking){
	if(isTracking){
		if(_partName==RIGHT_ARM){
			updateRightArmPosition();
			updateHandJoints();
		}
		if(_partName==LEFT_ARM)
			updateLeftArmPosition();
		if(_partName==TORSO)
			updateTorsoPosition();
		if(_partName==RIGHT_LEG)
			updateRightLegPosition();
		if(_partName==LEFT_LEG)
			updateLeftLegPosition();
	}else{
		Time::delay((2.0/1000.0));
		int nj;
		_pos->getAxes(&nj);
		for(int i = 0; i < nj; i++){
			_pos->stop(i);
			_vel->stop(i);
		}
	}
}




//UTILS-----------------------

//http://www.geometrictools.com/Documentation/EulerAngles.pdf
Vector iCubMotorCtrl::matrix2Euler(Matrix rMatrix){
	return matrix2Euler_XYZ(rMatrix);
}

Vector iCubMotorCtrl::matrix2Euler_XYZ(Matrix rMatrix){
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


Vector iCubMotorCtrl::matrix2Euler_XZY(Matrix rMatrix){

	Vector eulerAngles(3);
	// Assuming the angles are in radians.
	//RxRzRy
	double thetaX,thetaY,thetaZ;
	if (rMatrix[0][1] < 1) {
		if (rMatrix[0][1] > -1){

			thetaZ = asin(-rMatrix[0][1]);
			thetaX = atan2(rMatrix[2][1],rMatrix[1][1]);
			thetaY = atan2(rMatrix[0][2],rMatrix[0][0]);
		}else{// rMatrix[0][1] == -1	
			// Not a unique solution: thetaZ - thetaX = atan2(r10,r11)
			thetaZ = +M_PI_2;
			thetaX = atan2(-rMatrix[2][0],rMatrix[2][2]);
			thetaY = 0;
		}
	}else{// rMatrix[0][1] == 1

		// Not a unique solution: thetaZ + thetaX = atan2(r10,r11)
		thetaZ = -M_PI_2;
		thetaX = atan2(-rMatrix[2][0],rMatrix[2][2]);
		thetaY = 0;
	}
	eulerAngles[0] = thetaX;
	eulerAngles[1] = thetaY;
	eulerAngles[2] = thetaZ;
	return eulerAngles;
}


Vector iCubMotorCtrl::matrix2Euler_YZX(Matrix rMatrix){
	Vector eulerAngles(3);
	// Assuming the angles are in radians.
	//RxRzRy
	double thetaX,thetaY,thetaZ;
	if (rMatrix[1][0] < 1) {
		if (rMatrix[1][0] > -1){

			thetaZ = asin(rMatrix[1][0]);
			thetaY = atan2(-rMatrix[2][0],rMatrix[0][0]);
			thetaX = atan2(-rMatrix[1][2],rMatrix[1][1]);
		}else{// rMatrix[1][0] == -1	
			// Not a unique solution: thetaZ - thetaX = atan2(r10,r11)
			thetaZ = -M_PI_2;
			thetaY = -atan2(rMatrix[2][1],rMatrix[2][2]);
			thetaX = 0;
		}
	}else{// rMatrix[1][0] == 1

		// Not a unique solution: thetaZ + thetaX = atan2(r10,r11)
		thetaZ = +M_PI_2;
		thetaY = atan2(rMatrix[2][1],rMatrix[2][2]);
		thetaX = 0;
	}
	eulerAngles[0] = thetaX;
	eulerAngles[1] = thetaY;
	eulerAngles[2] = thetaZ;
	return eulerAngles;
}
