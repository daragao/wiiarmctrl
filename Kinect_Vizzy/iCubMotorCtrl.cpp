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
	_vel = NULL;
}

void iCubMotorCtrl::close(){
	_robotDevice->close();
}

bool iCubMotorCtrl::initRobot(string localPortPrefix,string robotName, string partName){

	if(partName=="right_arm"){
		_partName = RIGHT_ARM;
	}else if(partName=="left_arm"){
		_partName = LEFT_ARM;
	}else if(partName=="torso"){
		_partName = TORSO;
	}else if(partName=="head"){
		_partName = HEAD;
	}else if(partName=="shoulders"){
		_partName = SHOULDERS;
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
	if(_partName==SHOULDERS){
		command[1]=90;
		command[4]=90;
		//command[3]=45;
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

bool iCubMotorCtrl::moveMotorPosition(int motorIndex, double angleDeg){
	bool result = false;
	double limMin;
	double limMax;
	double currAngle;
	double currSpeed;
	double speed = _speed;
	double offset = 5;
	_encs->getEncoder(motorIndex,&currAngle);
	_encs->getEncoderSpeed(motorIndex,&currSpeed);
	_limits->getLimits(motorIndex,&limMin,&limMax);

	if(!(limMin>angleDeg||limMax<angleDeg)){
		if((abs(angleDeg-currAngle)>offset)){
			speed = _speed;
			_pos->setRefSpeed(motorIndex,speed);
		}else {
			speed = (speed/2) * abs((angleDeg-currAngle)/offset);
			_pos->setRefSpeed(motorIndex,speed);
		}
		_pos->positionMove(motorIndex,angleDeg);
	}

	return result;
}

bool iCubMotorCtrl::moveMotor(int motorIndex, double angleDeg){
	return moveMotorVelocity(motorIndex,angleDeg);
}

bool iCubMotorCtrl::moveMotorVelocity(int motorIndex, double angleDeg){
	bool result = false;
	double limMin;
	double limMax;
	double currAngle;
	double speed = _speed;
	double offset = 10;
	_encs->getEncoder(motorIndex,&currAngle);
	_limits->getLimits(motorIndex,&limMin,&limMax);
	if(abs(angleDeg-currAngle)>offset){
		speed = _speed;
	}else {
		speed = (speed/2) * abs((angleDeg-currAngle)/offset);
	}
	if(currAngle<angleDeg){
		_vel->velocityMove(motorIndex,speed);
	}else if(currAngle>angleDeg){
		_vel->velocityMove(motorIndex,-speed);
	}
	return result;
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
	//moveMotor(0,(euler[1]*(180.0/M_PI)));
	//moveMotor(1,180-(euler[2]*(180.0/M_PI))-90);
	//moveMotor(2,(euler[0]*(180.0/M_PI)));
	moveMotor(0,-(euler[0]*(180.0/M_PI)));
	//printf("euler(%.2f,%.2f,%.2f)\n",euler[0]*(180.0/M_PI),euler[1]*(180.0/M_PI),euler[2]*(180.0/M_PI));

	//RIGHT ELBOW
	Matrix elbowM = _icubData->jointsOri[SKEL_RIGHT_ELBOW];
	Matrix relElbowM = elbowM*shoulderM.transposed();
	euler = dcm2euler(relElbowM);
	moveMotor(1,(euler[1]*(180.0/M_PI)));
	//printf("euler(%.2f,\t%.2f,\t%.2f)\n",euler[0]*(180.0/M_PI),euler[1]*(180.0/M_PI),euler[2]*(180.0/M_PI));
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
	//moveMotor(0,-(euler[1]*(180.0/M_PI)));
	//moveMotor(1,180+(euler[2]*(180.0/M_PI))-90);
	moveMotor(0,-(euler[0]*(180.0/M_PI)));
	//printf("euler(%.2f,%.2f,%.2f)\n",euler[0]*(180.0/M_PI),euler[1]*(180.0/M_PI),euler[2]*(180.0/M_PI));

	//LEFT ELBOW
	Matrix elbowM = _icubData->jointsOri[SKEL_LEFT_ELBOW];
	Matrix relElbowM = elbowM*shoulderM.transposed();
	euler = dcm2euler(relElbowM);
	moveMotor(1,(euler[1]*(180.0/M_PI)));
	//printf("euler(%.2f,\t%.2f,\t%.2f)\n",euler[0]*(180.0/M_PI),euler[1]*(180.0/M_PI),euler[2]*(180.0/M_PI));
}

void iCubMotorCtrl::updateTorsoPosition(){
	Vector euler;
	euler.resize(3);

	//TORSO
	Matrix torsoM = _icubData->jointsOri[SKEL_TORSO];

	euler = matrix2Euler_XYZ(torsoM);
	moveMotor(0,-(euler[0]*(180.0/M_PI)));
}

void iCubMotorCtrl::updateShouldersPosition(){
	Vector euler;
	euler.resize(3);
	Matrix shoulderM;
	Matrix relShoulderM;
	Vector eulerXYZ;

	//TORSO
	Matrix torsoM = _icubData->jointsOri[SKEL_TORSO];
	Matrix torsoTransM = torsoM.transposed();

	//LEFT SHOULDER
	shoulderM = _icubData->jointsOri[SKEL_LEFT_SHOULDER];
	relShoulderM = shoulderM*torsoTransM;	

	eulerXYZ = matrix2Euler_XYZ(relShoulderM);
	//printf("XYZ: euler[0]:%2.2f\t\teuler[1]:%2.2f\t\teuler[2]:%2.2f\n",(eulerXYZ[0]*(180.0/M_PI)),(eulerXYZ[1]*(180.0/M_PI)),(eulerXYZ[2]*(180.0/M_PI)));

	if (eulerXYZ[0]<0 && eulerXYZ[2]>0){
		moveMotor(1,90+(90-(eulerXYZ[1]*(180.0/M_PI))));
		moveMotor(2,(-eulerXYZ[2]*(180.0/M_PI))+90-15);
	}else{
		moveMotor(1,(eulerXYZ[1]*(180.0/M_PI)));
		moveMotor(2,(eulerXYZ[2]*(180.0/M_PI))+90-15);
	}
	/*moveMotor(1,(eulerXYZ[1]*(180.0/M_PI)));
	moveMotor(2,(eulerXYZ[2]*(180.0/M_PI))+90-15);*/
	//moveMotor(2,(eulerYZY[1]*(180.0/M_PI))-90-15);
	//printf("YZY: euler[0]:%2.2f\t\teuler[1]:%2.2f\t\teuler[2]:%2.2f\n",(eulerYZY[0]*(180.0/M_PI)),(eulerYZY[1]*(180.0/M_PI)),(eulerYZY[2]*(180.0/M_PI)));


	//RIGHT SHOULDER
	shoulderM = _icubData->jointsOri[SKEL_RIGHT_SHOULDER];
	relShoulderM = shoulderM*torsoTransM;

	eulerXYZ = matrix2Euler_XYZ(relShoulderM);
	//printf("XYZ: euler[0]:%2.2f\t\teuler[1]:%2.2f\t\teuler[2]:%2.2f\n",(eulerXYZ[0]*(180.0/M_PI)),(eulerXYZ[1]*(180.0/M_PI)),(eulerXYZ[2]*(180.0/M_PI)));

	if (eulerXYZ[0]<0 && eulerXYZ[2]<0){
		moveMotor(4,(90+(90+(eulerXYZ[1]*(180.0/M_PI)))));
		moveMotor(5,-(-eulerXYZ[2]*(180.0/M_PI))-(90-15));
	}else{
		moveMotor(4,-(eulerXYZ[1]*(180.0/M_PI)));
		moveMotor(5,-((eulerXYZ[2]*(180.0/M_PI))-(90-15)));
	}
	/*moveMotor(4,-(eulerXYZ[1]*(180.0/M_PI)));
	moveMotor(5,-((eulerXYZ[2]*(180.0/M_PI))-(90-15)));*/

}

void iCubMotorCtrl::updateHeadPosition(){
}

void iCubMotorCtrl::updatePosition(bool isTracking){
	if(isTracking){
		if(_partName==RIGHT_ARM)
			updateRightArmPosition();
		if(_partName==LEFT_ARM)
			updateLeftArmPosition();
		if(_partName==TORSO)
			updateTorsoPosition();
		if(_partName==HEAD)
			updateHeadPosition();
		if(_partName==SHOULDERS)
			updateShouldersPosition();
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

Matrix iCubMotorCtrl::makeDHMatrix(double a, double alpha, double d, double theta){
	Matrix dhM(4,4);

	dhM[0][0] = cos(theta);
	dhM[0][1] = -sin(theta)*cos(alpha);
	dhM[0][2] = sin(theta)*sin(alpha);
	dhM[0][3] = a*cos(theta);

	dhM[1][0] = sin(theta);
	dhM[1][1] = cos(theta)*cos(alpha);
	dhM[1][2] =  -cos(theta)*sin(alpha);
	dhM[1][3] = a*sin(theta);

	dhM[2][0] = 0;
	dhM[2][1] = sin(alpha);
	dhM[2][2] = cos(alpha);
	dhM[2][3] = d;

	dhM[3][0] = 0;
	dhM[3][1] = 0;
	dhM[3][2] = 0;
	dhM[3][3] = 1;

	return dhM;
}

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

Vector iCubMotorCtrl::matrix2Euler_ZYX(Matrix rMatrix){
	Vector eulerAngles(3);
	// Assuming the angles are in radians.
	//RzRyRx
	double thetaX,thetaY,thetaZ;
	if (rMatrix[2][0] < 1) {
		if (rMatrix[2][0] > -1){

			thetaY = asin(-rMatrix[2][0]);
			thetaZ = atan2(rMatrix[1][0],rMatrix[0][0]);
			thetaX = atan2(-rMatrix[2][1],rMatrix[2][2]);
		}else{// rMatrix[2][0] == -1	
			// Not a unique solution: thetaZ - thetaX = atan2(r10,r11)
			thetaY = +M_PI_2;
			thetaZ = atan2(-rMatrix[1][2],rMatrix[1][1]);
			thetaX = 0;
		}
	}else{// rMatrix[2][0] == 1

		// Not a unique solution: thetaZ + thetaX = atan2(r10,r11)
		thetaY = -M_PI_2;
		thetaZ = atan2(rMatrix[1][2],rMatrix[1][1]);
		thetaX = 0;
	}
	eulerAngles[0] = thetaX;
	eulerAngles[1] = thetaY;
	eulerAngles[2] = thetaZ;
	return eulerAngles;
}

Vector iCubMotorCtrl::matrix2Euler_YZY(Matrix rMatrix){
	Vector eulerAngles(3);
	// Assuming the angles are in radians.
	//RzRyRx
	double thetaZ,thetaY0,thetaY1;
	if (rMatrix[1][1] < 1) {
		if (rMatrix[1][1] > -1){

			thetaZ = acos(-rMatrix[1][1]);
			thetaY0 = atan2(rMatrix[2][1],-rMatrix[0][1]);
			thetaY1 = atan2(-rMatrix[1][2],rMatrix[1][0]);
		}else{// rMatrix[1][1] == -1	
			// Not a unique solution: thetaZ - thetaX = atan2(r10,r11)
			thetaZ = +M_PI;
			thetaY0 = -atan2(rMatrix[2][0],rMatrix[2][2]);
			thetaY1 = 0;
		}
	}else{// rMatrix[1][1] == 1

		// Not a unique solution: thetaZ + thetaX = atan2(r10,r11)
		thetaZ = 0;
		thetaY0 = atan2(-rMatrix[2][0],rMatrix[2][2]);
		thetaY1 = 0;
	}
	eulerAngles[0] = thetaY0;
	eulerAngles[1] = thetaZ;
	eulerAngles[2] = thetaY1;
	return eulerAngles;
}

Vector iCubMotorCtrl::matrix2Euler_ZYZ(Matrix rMatrix){
	Vector eulerAngles(3);
	// Assuming the angles are in radians.
	//RzRyRx
	double thetaY,thetaZ0,thetaZ1;
	if (rMatrix[2][2] < 1) {
		if (rMatrix[2][2] > -1){

			thetaY = acos(-rMatrix[2][2]);
			thetaZ0 = atan2(rMatrix[1][2],rMatrix[0][2]);
			thetaZ1 = atan2(rMatrix[2][1],-rMatrix[2][0]);
		}else{// rMatrix[2][2] == -1	
			// Not a unique solution: thetaZ - thetaX = atan2(r10,r11)
			thetaY = +M_PI;
			thetaZ0 = -atan2(rMatrix[1][0],rMatrix[1][1]);
			thetaZ1 = 0;
		}
	}else{// rMatrix[2][2] == 1

		// Not a unique solution: thetaZ + thetaX = atan2(r10,r11)
		thetaY = 0;
		thetaZ0 = atan2(rMatrix[1][0],rMatrix[1][1]);
		thetaZ1 = 0;
	}
	eulerAngles[0] = thetaZ0;
	eulerAngles[1] = thetaY;
	eulerAngles[2] = thetaZ1;
	return eulerAngles;
}

Vector iCubMotorCtrl::matrix2Euler_XYX(Matrix rMatrix){
	Vector eulerAngles(3);
	// Assuming the angles are in radians.
	//RzRyRx
	double thetaY,thetaX0,thetaX1;
	if (rMatrix[0][0] < 1) {
		if (rMatrix[0][0] > -1){

			thetaY = acos(rMatrix[0][0]);
			thetaX0 = atan2(rMatrix[1][0],-rMatrix[2][0]);
			thetaX1 = atan2(rMatrix[0][1],rMatrix[0][2]);
		}else{// rMatrix[2][2] == -1	
			// Not a unique solution: thetaZ - thetaX = atan2(r10,r11)
			thetaY = +M_PI;
			thetaX0 = -atan2(-rMatrix[1][2],rMatrix[1][1]);
			thetaX1 = 0;
		}
	}else{// rMatrix[2][2] == 1

		// Not a unique solution: thetaZ + thetaX = atan2(r10,r11)
		thetaY = 0;
		thetaX0 = atan2(-rMatrix[1][2],rMatrix[1][1]);
		thetaX1 = 0;
	}
	eulerAngles[0] = thetaX0;
	eulerAngles[1] = thetaY;
	eulerAngles[2] = thetaX1;
	return eulerAngles;
}


Vector iCubMotorCtrl::dcm2euler(const Matrix &R){
    if (R.rows()<3 || R.cols()<3)
    {
        return Vector(0);
    }

    Vector v(3);
    double r2 = R(2,0)*R(2,0) + R(2,1)*R(2,1);
    if (r2 > 0)
    {
        v[1]=atan2(sqrt(r2), R(2,2));
	    v[0]=atan2(R(1,2)/sin(v[1]), R(0,2)/sin(v[1]));
	    v[2]=atan2(R(2,1)/sin(v[1]),-R(2,0)/sin(v[1]));
    }
    else
    {
        v[1]=0;
	    v[0]=atan2(R(1,0), R(0,0));
	    v[2]=0;
    }

    return v;
}
