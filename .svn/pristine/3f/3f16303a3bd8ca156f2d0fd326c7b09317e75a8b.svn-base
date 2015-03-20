#include "iCubIKinCtrl.h"

iCubIKinCtrl::iCubIKinCtrl(iCubCtrl *icub)
{
	_icub = icub;
	_movePosition = false;
	_minCartTrajTime = 60*2;//2 minutes if no time is specified
	_radius = 1;
	double origin2Elbow[3] = {35,10,0};//right_arm
	setPartsOn();
}

iCubIKinCtrl::~iCubIKinCtrl(void)
{
}

void iCubIKinCtrl::getShoulderPosition(double shoulderPosition[3]){
	double ori[4];
	_icub->getCartesianPose(4, shoulderPosition, ori);
}

void iCubIKinCtrl::getCartesianPose(double posArray[3], double oriArray[4]){
	_icub->getCartesianPose(posArray,oriArray);
}

void iCubIKinCtrl::setActivePart(int partID){
	switch(partID){
		case 1:
			_movePosition = true;
			break;
		case 2:
			_movePosition = false;
			break;
	}
}

bool iCubIKinCtrl::isOrientationActive(){
	return (!_movePosition);
}

void iCubIKinCtrl::addToRadius(double meters){
	_radius += meters;
	double maxRadius = 0.7;
	double minRadius = 0.15;
	if(_radius>maxRadius) _radius = maxRadius;
	if(_radius<0) _radius = minRadius;
	printf("RADIUS: %0.2f\n",_radius);
}

double iCubIKinCtrl::getRadius(){
	return _radius;
}

void iCubIKinCtrl::setRadius(double newRadius){
	_radius = newRadius;
	double maxRadius = 0.4;
	double minRadius = 0.1;
	if(_radius>maxRadius) _radius = maxRadius;
	if(_radius<minRadius) _radius = minRadius;
}

void iCubIKinCtrl::kinecticMoveActivePart(double position[3],double axisAngle[4],double solverPoint[3], double rotMatrix[3][3]){
	bool moveOnlyPosition = _movePosition;
	double rotMatrixTemp[3][3], newRotMatrix[3][3];
	double newAxisAngle[4];
	if(!moveOnlyPosition){

		Utils::axisAngle2Matrix(rotMatrixTemp,(-90*DEG_TO_RAD),0,0,1);//align the Y wiimote axis with the iCub X axis
		Utils::mult3DMatrixVector(newRotMatrix[0],rotMatrixTemp,rotMatrix[0]);
		Utils::mult3DMatrixVector(newRotMatrix[1],rotMatrixTemp,rotMatrix[1]);
		Utils::mult3DMatrixVector(newRotMatrix[2],rotMatrixTemp,rotMatrix[2]);

		Utils::rotMatrix2AxisAngle(newAxisAngle,newRotMatrix);
		movePositionWithOrientation(position,newAxisAngle);
	}else{
		//movePositionWithOrientation(solverPoint,axisAngle);
		movePosition(solverPoint[0],solverPoint[1],solverPoint[2]);
	}
}

void iCubIKinCtrl::cartesianDesired(double pos[3], double ori[4]){
	_icub->cartesianDesired(pos,ori);
}

void iCubIKinCtrl::movePosition(double x, double y, double z){
	move(false,0,0,0,0,x,y,z);
}

void iCubIKinCtrl::movePositionWithOrientation(double position[3],double axisAngle[4]){
	move(true,axisAngle[0],axisAngle[1],axisAngle[2],axisAngle[3],position[0],position[1],position[2]);
}

void iCubIKinCtrl::move(bool withOrientation, double angle, double oriX, double oriY, double oriZ, double x, double y, double z){
	static double _lastTime = Time::now();
	double currTime = Time::now();
	double timeDelta = currTime - _lastTime;
	if(timeDelta==0||timeDelta<0.00001) return;

	double minCartTrajTime = _minCartTrajTime;

	_icub->setCartTrajTime(minCartTrajTime);
	if(withOrientation)
		_icub->cartesianMove(x,y,z,angle,oriX,oriY,oriZ);
	else
		_icub->cartesianMove(x,y,z);

	_lastTime = currTime;
}

void iCubIKinCtrl::setMinCartTrajTime(double seconds){
	_minCartTrajTime = seconds;
}

void iCubIKinCtrl::stop(){
	_icub->stopKinematics();
}

void iCubIKinCtrl::setPartsOn(){
	//_icub->setCartDOF(10,1,1,1,1,1,1,1,1,1,1);//torso
	_icub->setCartDOF(10,0,0,0,1,1,1,1,1,1,1);//no torso
}