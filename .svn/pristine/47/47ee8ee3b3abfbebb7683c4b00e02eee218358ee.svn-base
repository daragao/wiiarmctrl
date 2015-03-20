#pragma once

#include <string>

// Headers for OpenNI
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnHash.h>
#include <XnLog.h>

// Header for NITE
#include "XnVNite.h"

#include "PointTracker.h"

using namespace std;
using namespace xn;

void XN_CALLBACK_TYPE SessionStarting(const XnPoint3D& ptPosition, void* UserCxt);
void XN_CALLBACK_TYPE SessionEnding(void* UserCxt);
void XN_CALLBACK_TYPE FocusProgress(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt);
void XN_CALLBACK_TYPE NoHands(void* UserCxt);

class KinectHandTracker
{
public:
	typedef struct KINECT_STATUS {
		int sessionState;
		long pointLastUpdate;
		double pointProjCoords[3];
		double pointRealCoords[3];
	}KinectStatus;
	KinectHandTracker(void);
	~KinectHandTracker(void);
	void close();
	void updateKinect();
	static KinectStatus *getKinect();
private:
	static KinectStatus *_kinectStatus;
	// OpenNI objects
	Context *_context;
	DepthGenerator *_depthGenerator;
	HandsGenerator *_handsGenerator;
	// NITE objects
	XnVSessionManager* _sessionManager;
	XnVFlowRouter* _flowRouter;
	PointTracker *_pointTracker;
	int init();
	void initVars();
};
