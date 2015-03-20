#pragma once

//OpenNI includes
#include <XnOpenNI.h>
#include <XnLog.h>
#include <XnCppWrapper.h>
#include <XnFPSCalculator.h>

// Header for NITE
#include <XnVNite.h>

#include <string>

using namespace std;
using namespace xn;

class KinectCtrl
{
public:
	typedef struct KINECT_STATUS {
		bool anyHandDetected;
		double anyHandPos[3];
		bool isTracking;
		bool isNITESession;
		UINT32 frameID;
		double fps;
		double skeletonPointsPos[24*3];
		double skeletonPointsOri[24*9];
	} KinectStatus;
	KinectCtrl(void);
	~KinectCtrl(void);
	bool updateFrame();
	void updateSkeleton();
	KinectStatus* getKinectStatus();
	void close();
private:
	KinectStatus *_kinectStatus;
	Context *_context;
	XnVSessionManager *_sessionMng;
	DepthGenerator *_depth;
	UserGenerator *_user;
	HandsGenerator *_hands;
	XnFPSData *_xnFPS;
	void init();
	void initKinectStatus();
	bool checkRC(XnStatus rc, string what);
};

//NITE callbacks
void XN_CALLBACK_TYPE FocusProgress(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt);
void XN_CALLBACK_TYPE SessionStart(const XnPoint3D& PFOCUS_EVENT_RECORD, void* UserCxt);
void XN_CALLBACK_TYPE SessionEnd(void* UserCxt);
void XN_CALLBACK_TYPE Push_Pushed(XnFloat fVelocity, XnFloat fAngle, void *cxt);
void XN_CALLBACK_TYPE Hand_Created(HandsGenerator &hNode, XnUserID user, const XnPoint3D *pPosition, XnFloat fTime, void *pCookie);
void XN_CALLBACK_TYPE Hand_Updated(HandsGenerator &generator, XnUserID user, const XnPoint3D *pPosition, XnFloat fTime, void *pCookie);
void XN_CALLBACK_TYPE Hand_Destroyed(HandsGenerator &generator, XnUserID user, XnFloat fTime, void *pCookie);
void XN_CALLBACK_TYPE User_Created(UserGenerator &generator, XnUserID user, void *pCookie);
void XN_CALLBACK_TYPE User_Destroyed(UserGenerator &generator, XnUserID user, void *pCookie);
void XN_CALLBACK_TYPE CalibrationStart(SkeletonCapability& skeleton, XnUserID user, void* pCookie);
void XN_CALLBACK_TYPE CalibrationEnd(SkeletonCapability& skeleton, XnUserID userID, XnBool bSuccess, void* pCookie);
void XN_CALLBACK_TYPE PoseDetected(PoseDetectionCapability& poseDetection, const XnChar* strPose, XnUserID nId, void* pCookie);