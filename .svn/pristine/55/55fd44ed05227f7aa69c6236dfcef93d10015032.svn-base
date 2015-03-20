#pragma once

#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/IKinectDeviceDriver.h>

#include <iostream>
#include <string>

using namespace std;

using namespace yarp::dev;
using namespace yarp::os;
using namespace yarp::sig;

class KinectCtrl
{
public:
	KinectCtrl(string portPrefix, bool userDetection);
	~KinectCtrl(void);
	bool update();
	void close();
	bool isTracking(int userID);
	// ORIENTATION MATRICES
	bool getOrientationHead(int userID, Matrix &oriMatrix, double minConfidence = 0.5);
	bool getOrientationTorso(int userID, Matrix &oriMatrix, double minConfidence = 0.5);
	bool getOrientationWaist(int userID, Matrix &oriMatrix, double minConfidence = 0.5);
	bool getOrientationRightShoulder(int userID, Matrix &oriMatrix, double minConfidence = 0.5);
	bool getOrientationLeftShoulder(int userID, Matrix &oriMatrix, double minConfidence = 0.5);
	bool getOrientationRightElbow(int userID, Matrix &oriMatrix, double minConfidence = 0.5);
	bool getOrientationLeftElbow(int userID, Matrix &oriMatrix, double minConfidence = 0.5);
private:
	PolyDriver *_kinectPolyDriver;
	IKinectDeviceDriver *_kinect;
	struct UserJoints_t {
		int status;
		Matrix orientation[TOTAL_JOINTS];
		double orientationConf[TOTAL_JOINTS];
	} _userJoints[MAX_USERS];
	void initVars();
	bool initKinectInterface(string portPrefix, bool userDetection);
	bool updateKinectInterface();
	void stateVerbose(int userID, int oldState, int newState);
	string stateToStr(int state);
	bool getOrientation(int userID, int partIndex, Matrix &oriMatrix, double minConfidence);
};
