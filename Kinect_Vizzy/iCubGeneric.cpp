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
	_icubData->handProjPos.resize(3);
	_icubData->handProjPos.zero();
	_icubData->handRealPos.resize(3);
	_icubData->handRealPos.zero();
	_icubData->lastBottleTimestamp = -1;
	for(int i = 0; i < 24; i++){
		_icubData->jointsPos[i].resize(3);
		_icubData->jointsOri[i].resize(3,3);
		_icubData->jointsPos[i].zero();
		_icubData->jointsOri[i].zero();
	}
}

void iCubGeneric::updateTimestamp(){
	_icubData->lastBottleTimestamp = clock();
}

void iCubGeneric::setHandRealPos(double x, double y, double z){
	_icubData->handRealPos[0] = x;
	_icubData->handRealPos[1] = y;
	_icubData->handRealPos[2] = z;
}

void iCubGeneric::setHandProjPos(double x, double y, double z){
	_icubData->handProjPos[0] = x;
	_icubData->handProjPos[1] = y;
	_icubData->handProjPos[2] = z;
}

void iCubGeneric::setJointPos(int index, double x, double y, double z){
	_icubData->jointsPos[index][0] = x;
	_icubData->jointsPos[index][1] = y;
	_icubData->jointsPos[index][2] = z;
}

void iCubGeneric::setJointOri(int index, double m00, double m01, double m02, double m10, double m11, double m12,double m20, double m21, double m22){
	_icubData->jointsOri[index][0][0] = m00;
	_icubData->jointsOri[index][0][1] = m01;
	_icubData->jointsOri[index][0][2] = m02;
	_icubData->jointsOri[index][1][0] = m10;
	_icubData->jointsOri[index][1][1] = m11;
	_icubData->jointsOri[index][1][2] = m12;
	_icubData->jointsOri[index][2][0] = m20;
	_icubData->jointsOri[index][2][1] = m21;
	_icubData->jointsOri[index][2][2] = m22;
}