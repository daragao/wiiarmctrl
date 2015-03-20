#include "KinectCtrl.h"

KinectCtrl::KinectCtrl(string portPrefix, bool userDetection)
{	
	initVars();
	initKinectInterface(portPrefix,userDetection);
}

KinectCtrl::~KinectCtrl(void)
{
}

void KinectCtrl::close(){
	_kinectPolyDriver->close();
}

void KinectCtrl::initVars(){
	_kinect = NULL;
	_kinectPolyDriver = NULL;
	for(int i = 0; i < MAX_USERS; i++){
		for(int j = 0; j < TOTAL_JOINTS; j++){
			_userJoints[i].orientation[j].resize(3,3);
			_userJoints[i].orientation[j].zero();
			_userJoints[i].orientationConf[j] = -1;
		}
		_userJoints[i].status = NO_USER;
	}
}

bool KinectCtrl::initKinectInterface(string portPrefix, bool userDetection){
	string arguments = "";
	if(portPrefix.size()>1) arguments += " (portPrefix " + portPrefix + ")";
	if(userDetection) arguments += " (userDetection)";
	string kinectPropertiesStr = "(device KinectDeviceLocal)" + arguments + " (noCameras)";
	Property kinectProperties(kinectPropertiesStr.c_str());
	_kinectPolyDriver = new PolyDriver(kinectProperties);
	_kinectPolyDriver->view(_kinect);
	if (_kinect==NULL) return false;
	return true;
}

bool KinectCtrl::updateKinectInterface(){
	bool success = true;
	int newStatus;
	for(int i = 0; i <MAX_USERS; i++){
		newStatus = _kinect->getSkeletonState(i);
		stateVerbose(i,_userJoints[i].status,newStatus);
		_userJoints[i].status = newStatus;
		if(_userJoints[i].status==SKELETON_TRACKING)
			_kinect->getSkeletonOrientation(_userJoints[i].orientation,_userJoints[i].orientationConf,i);
	}
	return success;
}

bool KinectCtrl::update(){
	bool success = updateKinectInterface();
	return success;
}

bool KinectCtrl::getOrientation(int userID, int partIndex, Matrix &oriMatrix, double minConfidence){
	bool success = true;
	if( _userJoints[userID].status != SKELETON_TRACKING) return false;
	if( _userJoints[userID].orientationConf[partIndex] < minConfidence )
		success = false;
	oriMatrix = _userJoints[userID].orientation[partIndex];
	return success;
}

/****************************************************************************
*****************************ORIENTATION_MATRICES****************************
****************************************************************************/

bool KinectCtrl::isTracking(int userID){
	if( _userJoints[userID].status != SKELETON_TRACKING) return false;
	else return true;
}

bool KinectCtrl::getOrientationHead(int userID, Matrix &oriMatrix, double minConfidence){
	return getOrientation(userID, SKEL_HEAD, oriMatrix, minConfidence);
}

bool KinectCtrl::getOrientationTorso(int userID, Matrix &oriMatrix, double minConfidence){
	return getOrientation(userID, SKEL_TORSO, oriMatrix, minConfidence);
}

bool KinectCtrl::getOrientationWaist(int userID, Matrix &oriMatrix, double minConfidence){
	return getOrientation(userID, SKEL_WAIST, oriMatrix, minConfidence);
}

bool KinectCtrl::getOrientationRightShoulder(int userID, Matrix &oriMatrix, double minConfidence){
	return getOrientation(userID, SKEL_RIGHT_SHOULDER, oriMatrix, minConfidence);
}

bool KinectCtrl::getOrientationLeftShoulder(int userID, Matrix &oriMatrix, double minConfidence){
	return getOrientation(userID, SKEL_LEFT_SHOULDER, oriMatrix, minConfidence);
}

bool KinectCtrl::getOrientationRightElbow(int userID, Matrix &oriMatrix, double minConfidence){
	return getOrientation(userID, SKEL_RIGHT_ELBOW, oriMatrix, minConfidence);
}

bool KinectCtrl::getOrientationLeftElbow(int userID, Matrix &oriMatrix, double minConfidence){
	return getOrientation(userID, SKEL_LEFT_ELBOW, oriMatrix, minConfidence);
}


/****************************************************************************
 ***********************************HELPERS**********************************
****************************************************************************/

void KinectCtrl::stateVerbose(int userID, int oldStateInt, int newStateInt){
	string oldStateStr, newStateStr;
	if(oldStateInt!=newStateInt){
		oldStateStr = stateToStr(oldStateInt);
		newStateStr = stateToStr(newStateInt);
		cout << "User " << userID << " changed from " << oldStateStr;
		cout << " to " << newStateStr << endl;
	}
}

string KinectCtrl::stateToStr(int state){
	string result;
	switch(state){
		case USER_DETECTED:
			result = "USER DETECTED";
			break;
		case USER_LOST:
			result = "USER LOST";
			break;
		case NO_USER:
			result = "NO USER";
			break;
		case CALIBRATING:
			result = "CALIBRATING";
			break;
		case SKELETON_TRACKING:
			result = "SKELETON TRACKING";
			break;
	}
	return result;
}