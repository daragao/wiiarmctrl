#include "Util.h"

wiimoteData *Util::wiiData;
icubRobotData *Util::icubData;
guiDataStruct *Util::guiData;

void Util::initWiiData(){
	wiiData = new wiimoteData();
	wiiData->isConnected = false;
	wiiData->botA = false;
	wiiData->botB = false;
	wiiData->accX = 0;
	wiiData->accY = 0;
	wiiData->accZ = 0;
	wiiData->motionPlusX = 0;
	wiiData->motionPlusY = 0;
	wiiData->motionPlusZ = 0;
	wiiData->motionPlusAngleX = 0;
	wiiData->motionPlusAngleY = 0;
	wiiData->motionPlusAngleZ = 0;
	wiiData->battery = 0;
	wiiData->isMPLowSpeed = -1;
	wiiData->temp = 0;
}

void Util::initGuiData(){
	guiData = new guiDataStruct();
	guiData->isGuiON = false;
	guiData->mpBot = false;
	guiData->wiiRebootBot = false;
}

void Util::initRobotData(){
	icubData = new icubRobotData();
	icubData->isConnected = false;
	icubData->oriRightArm[0] = 0;
	icubData->oriRightArm[1] = 0;
	icubData->oriRightArm[2] = 0;
	icubData->oriRightArm[3] = 0;
	icubData->posRightArm[0] = 0;
	icubData->posRightArm[1] = 0;
	icubData->posRightArm[2] = 0;
	icubData->posRightArm[3] = 0;
}

void Util::initUtil(){
	initWiiData();
	initGuiData();
	initRobotData();
}


double *Util::rotate(double *angleArray, double heading, double attitude, double bank) {
	// Assuming the angles are in radians.
	double c1 = cos(heading/2);
	double s1 = sin(heading/2);
	double c2 = cos(attitude/2);
	double s2 = sin(attitude/2);
	double c3 = cos(bank/2);
	double s3 = sin(bank/2);
	double c1c2 = c1*c2;
	double s1s2 = s1*s2;
	double w =c1c2*c3 - s1s2*s3;
	double x =c1c2*s3 + s1s2*c3;
	double y =s1*c2*c3 + c1*s2*s3;
	double z =c1*s2*c3 - s1*c2*s3;
	double angle = 2 * acos(w);
	double norm = x*x+y*y+z*z;
	if (norm < 0.001) { // when all euler angles are zero angle =0 so
		// we can set axis to anything to avoid divide by zero
		x=1;
		y=z=0;
	} else {
		norm = sqrt(norm);
    	x /= norm;
    	y /= norm;
    	z /= norm;
	}
	angleArray[0] = x;
	angleArray[1] = y;
	angleArray[2] = z;
	angleArray[3] = angle;
	return angleArray;
}