#include "iCubGeneric.h"

iCubGeneric::iCubData *iCubGeneric::_icubData;

iCubGeneric::iCubGeneric(void)
{
	initVars();
}

iCubGeneric::~iCubGeneric(void)
{
}

void iCubGeneric::initVars(){
	_icubData = new iCubData();
	updateTimestamp();
	_icubData->controlID = -1;
	_icubData->solverVector.resize(3);
	_icubData->solverRotMatrix.resize(3,3);
	_icubData->mpRawVector.resize(3);
	_icubData->mpAngleVector.resize(3);
}

void iCubGeneric::updateTimestamp(){
	_icubData->lastBottleTimestamp = clock();
}

void iCubGeneric::setControlID(int id){
	_icubData->controlID = id;
}

void iCubGeneric::setMPRawVector(double x, double y, double z){
	_icubData->mpRawVector[0] = x;
	_icubData->mpRawVector[1] = y;
	_icubData->mpRawVector[2] = z;
}

Vector iCubGeneric::getMPRawVector(){
	return _icubData->mpRawVector;
}

void iCubGeneric::setMPAngleVector(double x, double y, double z){
	_icubData->mpAngleVector[0] = x;
	_icubData->mpAngleVector[1] = y;
	_icubData->mpAngleVector[2] = z;
}

Vector iCubGeneric::getMPAngleVector(){
	return _icubData->mpAngleVector;
}

void iCubGeneric::setSolverVector(double x, double y, double z){
	_icubData->solverVector[0] = x;
	_icubData->solverVector[1] = y;
	_icubData->solverVector[2] = z;
}

void iCubGeneric::setSolverMatrix(double r00, double r01, double r02, double r10, double r11, double r12, double r20, double r21, double r22){
	_icubData->solverRotMatrix[0][0] = r00;
	_icubData->solverRotMatrix[0][1] = r01;
	_icubData->solverRotMatrix[0][2] = r02;
	_icubData->solverRotMatrix[1][0] = r10;
	_icubData->solverRotMatrix[1][1] = r11;
	_icubData->solverRotMatrix[1][2] = r12;
	_icubData->solverRotMatrix[2][0] = r20;
	_icubData->solverRotMatrix[2][1] = r21;
	_icubData->solverRotMatrix[2][2] = r22;
}
