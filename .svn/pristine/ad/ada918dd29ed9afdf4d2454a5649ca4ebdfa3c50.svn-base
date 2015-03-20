#pragma once

#include <string>
#include <time.h>

// Headers for OpenNI
#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <XnHash.h>
#include <XnLog.h>

using namespace std;
using namespace xn;

typedef enum
{
	NO_USER,
	USER_DETECTED,
	CALIBRATING,
	SKELETON_TRACKING
} SessionState;

void XN_CALLBACK_TYPE UserCreated(UserGenerator& generator, XnUserID userID, void* pCookie);
void XN_CALLBACK_TYPE UserDestroyed(UserGenerator& generator, XnUserID userID, void* pCookie);
void XN_CALLBACK_TYPE CalibrationStart(SkeletonCapability &skeleton, XnUserID userID, void *pCookie);
void XN_CALLBACK_TYPE CalibrationEnd(SkeletonCapability &skeleton, XnUserID userID, XnBool bSuccess, void *pCookie);
void XN_CALLBACK_TYPE PoseDetected(PoseDetectionCapability &poseDetection, const XnChar* strPose, XnUserID userID, void *pCookie);

#define MAX_USERS 10
#define TOTAL_JOINTS 24

class KinectSkeletonTracker
{
public:
	typedef struct USER_SKELETON {
		int skeletonState;
		double skeletonPointsPos[TOTAL_JOINTS][3];
		double skeletonPointsOri[TOTAL_JOINTS][9];
	}UserSkeleton;
	typedef struct KINECT_STATUS {
		UserSkeleton userSkeleton[MAX_USERS+1];
	}KinectStatus;
	KinectSkeletonTracker(void);
	~KinectSkeletonTracker(void);
	void close();
	void updateKinect();
	static KinectStatus *getKinect();
private:
	static KinectStatus *_kinectStatus;
	// OpenNI objects
	Context *_context;
	UserGenerator *_userGenerator;
	int init();
	void initVars();
};
