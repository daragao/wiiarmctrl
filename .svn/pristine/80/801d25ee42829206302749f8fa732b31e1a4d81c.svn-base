#include "iCubiKinCtrl.h"

iCubiKinCtrl::iCubiKinCtrl(string localPortPrefix,string robotName,string partName,double trajTime)
{
	initVars();
	initRobot(localPortPrefix,robotName,partName,trajTime);
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

bool iCubiKinCtrl::initRobot(string localPortPrefix,string robotName, string partName, double trajTime){
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

	_partCart->setTrajTime(10);

	return true;
}

void iCubiKinCtrl::updatePosition(bool isTracking){
	double radius = 0.3;
	static long lastTimestamp = -1;
	Vector moveTo, pose(3), ori(4), normalSolverVector(3);
	Vector shoulderPosition = getShoulderPosition();
	normalSolverVector[0] = (_icubData->solverVector[0]/norm(_icubData->solverVector));
	normalSolverVector[1] = (_icubData->solverVector[1]/norm(_icubData->solverVector));
	normalSolverVector[2] = (_icubData->solverVector[2]/norm(_icubData->solverVector));
	moveTo = (normalSolverVector*radius)+shoulderPosition;
	
	if(!isTracking){
		resetSolver();
		return;
	}

	if(lastTimestamp !=_icubData->lastBottleTimestamp){
		if(_icubData->controlID==1){//alter position
			_partCart->goToPosition(moveTo);
		}
		if(_icubData->controlID==2){//alter orientation
		}
		//printf("pos(%f,%f,%f)\n",moveTo[0],moveTo[1],moveTo[2]);
	}

	lastTimestamp = _icubData->lastBottleTimestamp;
}

void iCubiKinCtrl::resetSolver(){
	double radius = 0.3;
	Vector firstPoint(3);
	firstPoint.zero();
	firstPoint[1] = 1;
	Vector moveTo(3), pose(3),ori(3);
	Vector shoulderPosition = getShoulderPosition();
	_partCart->stopControl();
	_partCart->getPose(pose,ori);
	moveTo = pose-shoulderPosition;
	radius = norm(pose);
	double moveToNorm = norm(moveTo);
	moveTo[0] = moveTo[0]/moveToNorm;
	moveTo[1] = moveTo[1]/moveToNorm;
	moveTo[2] = moveTo[2]/moveToNorm;
	double angle = acos(dot(firstPoint,moveTo));
	moveTo = cross(firstPoint,moveTo);
	Vector result(4);
	result[0] = moveTo[0];
	result[1] = moveTo[1];
	result[2] = moveTo[2];
	result[3] = angle;
	Matrix newRotM = axis2dcm(result);
}



//UTILS-----------------------

Vector iCubiKinCtrl::getShoulderPosition(){
	Vector shoulderPos(3), shoulderOri(4);
	_partCart->getPose(4, shoulderPos, shoulderOri);
	return shoulderPos;
}


//http://www.geometrictools.com/Documentation/EulerAngles.pdf
Vector iCubiKinCtrl::matrix2Euler(Matrix rMatrix){
	return matrix2Euler_XYZ(rMatrix);
}

Vector iCubiKinCtrl::matrix2Euler_XYZ(Matrix rMatrix){
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


Vector iCubiKinCtrl::matrix2Euler_XZY(Matrix rMatrix){

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


Vector iCubiKinCtrl::matrix2Euler_YZX(Matrix rMatrix){
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
