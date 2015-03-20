#include "WiiSphericalCoord.h"

WiiSphericalCoord::WiiSphericalCoord(double firstPoint[3])
{
	initVars();
	_firstPoint[0] = firstPoint[0];
	_firstPoint[1] = firstPoint[1];
	_firstPoint[2] = firstPoint[2];
}

WiiSphericalCoord::~WiiSphericalCoord(void)
{
}

void WiiSphericalCoord::initVars(){
	_calibStatus = -1;
	_radius = 1;
	_mpAngle.resize(3);
	_mpAngle.zero();
	_mpAngleRate.resize(3);
	_mpAngleRate.zero();
	_pointCoord.resize(3);
	_pointCoord.zero();
	_firstPoint[0] = 0;
	_firstPoint[1] = 1;
	_firstPoint[2] = 0;
	Utils::euler2Matrix(_lastRotMatrix,0,0,0);
}

void WiiSphericalCoord::updateMotionPlusData(double status, double xRate, double yRate, double zRate, double xAngle, double yAngle, double zAngle){
	Vector angle = _mpAngle;
	_calibStatus = status;
	if(status == 1){
		angle[0] += _mpAngleRate[0] = xAngle;
		angle[1] += _mpAngleRate[1] = yAngle;
		angle[2] += _mpAngleRate[2] = zAngle;
	}else{
		_mpAngleRate.zero();
	}
	_mpAngle = angle;
	updateSphericalCoordYarpMatrix();
}

//gets a new rotation matrix from a vector
void WiiSphericalCoord::setNewPosition(double xPos, double yPos, double zPos){
	double newVector[3] = {xPos,yPos,zPos};

	Utils::normalizeVector(newVector,newVector);
	Utils::crossProduct(newVector,_firstPoint,newVector);
	double angle = asin(Utils::normVector(newVector));
	angle *= RAD_TO_DEG;
	Utils::normalizeVector(newVector,newVector);

	setNewPosition(angle,newVector[0],newVector[1],newVector[2]);
}

//gets a new rotation matrix from a axis angle
void WiiSphericalCoord::setNewPosition(double angle, double xPos, double yPos, double zPos){
	if(xPos!=0||yPos!=0||zPos!=0)
		Utils::axisAngle2Matrix(_lastRotMatrix,angle*DEG_TO_RAD,xPos,yPos,zPos);
	else Utils::euler2Matrix(_lastRotMatrix,0,0,0);
}

void WiiSphericalCoord::setNewRotationMatrix(double rotMatrix[3][3]){
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			_lastRotMatrix[i][j] = rotMatrix[i][j];
}

void WiiSphericalCoord::updateSphericalCoordYarpMatrix(){

	Vector newPoint(3);
	Vector firstPoint(3,_firstPoint);

	double xAngle = _mpAngleRate[0]*DEG_TO_RAD;
	double yAngle = _mpAngleRate[1]*DEG_TO_RAD;
	double zAngle = _mpAngleRate[2]*DEG_TO_RAD;
	_mpAngleRate[0] = 0;
	_mpAngleRate[1] = 0;
	_mpAngleRate[2] = 0;

	Matrix lastRotMatrix(3,3);
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			lastRotMatrix[i][j] = _lastRotMatrix[i][j];

	Matrix rotMatrixX(3,3);
	rotMatrixX.zero();
	rotMatrixX[0][0] = 1;
	rotMatrixX[1][1] = cos(xAngle);
	rotMatrixX[1][2] = -sin(xAngle);
	rotMatrixX[2][1] = sin(xAngle);
	rotMatrixX[2][2] = cos(xAngle);
	Matrix rotMatrixY(3,3);
	rotMatrixY.zero();
	rotMatrixY[0][0] = cos(yAngle);
	rotMatrixY[0][2] = sin(yAngle);
	rotMatrixY[1][1] = 1;
	rotMatrixY[2][0] = -sin(yAngle);
	rotMatrixY[2][2] = cos(yAngle);
	Matrix rotMatrixZ(3,3);
	rotMatrixZ.zero();
	rotMatrixZ[0][0] = cos(zAngle);
	rotMatrixZ[0][1] = -sin(zAngle);
	rotMatrixZ[1][0] = sin(zAngle);
	rotMatrixZ[1][1] = cos(zAngle);
	rotMatrixZ[2][2] = 1;

	Matrix rotMatrixZXY(3,3);
	rotMatrixZXY = (rotMatrixX*rotMatrixZ)*rotMatrixY;

	lastRotMatrix = lastRotMatrix*rotMatrixZXY;

	/*Matrix tempMatrix = lastRotMatrix*lastRotMatrix.transposed();
	printf("\n%s\n",getMatrixStr(tempMatrix).c_str());*/


	newPoint = lastRotMatrix*firstPoint;

	_pointCoord = newPoint;

	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			_lastRotMatrix[i][j] = lastRotMatrix[i][j];
}

Vector WiiSphericalCoord::getSphericalCoord(double radius, double phi, double theta){
	Vector point;
	point.resize(3);
	point[0] = radius * cos (theta * DEG_TO_RAD) * sin (phi * DEG_TO_RAD);
	point[1] = radius * sin (theta * DEG_TO_RAD) * sin (phi * DEG_TO_RAD);
	point[2] = radius * cos (phi * DEG_TO_RAD);
	return point;
}


Vector WiiSphericalCoord::getPointCoord(){
	return _pointCoord;
}

Vector WiiSphericalCoord::getAxisAngle(){
	Vector axisAngle;
	axisAngle.resize(4);
	axisAngle[0] = _mpAngle[AXISANGLE_INDEX];
	axisAngle[1] = _pointCoord[0];
	axisAngle[2] = _pointCoord[1];
	axisAngle[3] = _pointCoord[2];
	return axisAngle;
}

Vector WiiSphericalCoord::getLastRotationMatrix(){
	Vector rotMatrix;
	rotMatrix.resize(9);
	rotMatrix[0] = _lastRotMatrix[0][0];
	rotMatrix[1] = _lastRotMatrix[0][1];
	rotMatrix[2] = _lastRotMatrix[0][2];
	rotMatrix[3] = _lastRotMatrix[1][0];
	rotMatrix[4] = _lastRotMatrix[1][1];
	rotMatrix[5] = _lastRotMatrix[1][2];
	rotMatrix[6] = _lastRotMatrix[2][0];
	rotMatrix[7] = _lastRotMatrix[2][1];
	rotMatrix[8] = _lastRotMatrix[2][2];
	return rotMatrix;
}

//UTILS!!


string WiiSphericalCoord::getMatrixStr(Matrix tempMatrix){
	string str = "[";
	str += tempMatrix.toString()+"]";
	string key (";");
	size_t found = str.rfind(key);
	if (found!=string::npos)
		str.replace (found,key.length(),"\n");
	found = str.rfind(key);
	if (found!=string::npos)
		str.replace (found,key.length(),"\n");
	found = str.rfind(key);
	if (found!=string::npos)
		str.replace (found,key.length(),"");
	return str;
}