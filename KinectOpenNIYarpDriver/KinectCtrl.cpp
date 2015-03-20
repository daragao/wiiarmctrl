#include "KinectCtrl.h"

#define SAMPLE_XML_PATH "C:\\Users\\doart3\\Documents\\Thesis\\Technical\\YarpDeviceModule\\wiiarmctrl\\KinectOpenNIYarpDriver/SamplesConfig.xml"

KinectCtrl::KinectCtrl(void)
{
	initKinectStatus();
	init();
}

KinectCtrl::~KinectCtrl(void)
{
}

void KinectCtrl::initKinectStatus(){
	_kinectStatus = new KinectStatus();
	_kinectStatus->isNITESession = false;
	_kinectStatus->anyHandDetected = false;
	_kinectStatus->isTracking = false;
	_kinectStatus->anyHandPos[0] = 0;
	_kinectStatus->anyHandPos[1] = 0;
	_kinectStatus->anyHandPos[2] = 0;
	_kinectStatus->fps = -1;
	_kinectStatus->frameID = -1;
}

bool KinectCtrl::checkRC(XnStatus rc, string what){
	if (rc != XN_STATUS_OK)
	{					
		printf("%s failed: %s\n", what.c_str(), xnGetStatusString(rc));		
		return false;
	}
	return true;
}

void KinectCtrl::init(){
	XnStatus nRetVal = XN_STATUS_OK;

	Context *context = new Context();
	EnumerationErrors errors;

	//init context
	nRetVal = context->InitFromXmlFile(SAMPLE_XML_PATH, &errors);

	if (nRetVal == XN_STATUS_NO_NODE_PRESENT)
	{
		XnChar strError[1024];
		errors.ToString(strError, 1024);
		printf("%s\n", strError);
		return;// (nRetVal);
	}
	else if (nRetVal != XN_STATUS_OK)
	{
		printf("Open failed: %s\n", xnGetStatusString(nRetVal));
		return;// (nRetVal);
	}
	_context = context;

	XnCallbackHandle h_hand,h_user,h_pose,h_calib; 

	//init NITE session manager
	XnVSessionManager *sessionMng = new XnVSessionManager();
	nRetVal = sessionMng->Initialize(context, "Wave,Click", "RaiseHand");
	checkRC(nRetVal, "Init SessionManager");
	sessionMng->RegisterSession(NULL,SessionStart,SessionEnd,FocusProgress);
	_sessionMng = sessionMng;

	//init depth
	DepthGenerator *depth = new DepthGenerator();
	nRetVal = context->FindExistingNode(XN_NODE_TYPE_DEPTH, *depth);
	checkRC(nRetVal, "Find depth generator");
	_depth = depth;

	//init hands
	HandsGenerator *hands = new HandsGenerator();
	nRetVal = context->FindExistingNode(XN_NODE_TYPE_HANDS, *hands);
	checkRC(nRetVal, "Find hands generator");
	hands->RegisterHandCallbacks(Hand_Created, Hand_Updated, Hand_Destroyed, getKinectStatus(), h_hand); 
	_hands = hands;

	//init user
	UserGenerator *user = new UserGenerator();
	nRetVal = context->FindExistingNode(XN_NODE_TYPE_USER, *user);
	user->RegisterUserCallbacks(User_Created,User_Destroyed,NULL,h_user);
	checkRC(nRetVal, "Find user generator");
	_user = user;

	//skeleton
	user->GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
	user->GetSkeletonCap().SetSmoothing(0.6);
	user->GetSkeletonCap().RegisterCalibrationCallbacks(&CalibrationStart, &CalibrationEnd, user, h_calib);
	user->GetPoseDetectionCap().RegisterToPoseCallbacks(&PoseDetected, NULL, user, h_pose);

	//FPS data
	XnFPSData *xnFPS = new XnFPSData();
	nRetVal = xnFPSInit(xnFPS, 180);
	checkRC(nRetVal, "FPS Init");
	_xnFPS = xnFPS;

	// Initialization done. Start generating
	nRetVal = context->StartGeneratingAll();
	checkRC(nRetVal, "StartGenerating");

}

bool KinectCtrl::updateFrame(){
	XnStatus nRetVal = XN_STATUS_OK;

	Context *context = _context;
	DepthGenerator *depth = _depth;
	XnFPSData *xnFPS = _xnFPS;
	DepthMetaData depthMD;

	nRetVal = context->WaitOneUpdateAll(*depth);
	if (nRetVal != XN_STATUS_OK)
	{
		printf("UpdateData failed: %s\n", xnGetStatusString(nRetVal));
		return false;
	}

	//update session manager
	_sessionMng->Update(context);

	//update to current frame
	xnFPSMarkFrame(xnFPS);

	depth->GetMetaData(depthMD);
	const XnDepthPixel* pDepthMap = depthMD.Data();

	_kinectStatus->frameID = depthMD.FrameID();
	_kinectStatus->fps = xnFPSCalc(xnFPS);

	updateSkeleton();

	return true;
}

KinectCtrl::KinectStatus* KinectCtrl::getKinectStatus(){
	return _kinectStatus;
}

void KinectCtrl::close(){
	if (_context==NULL) return;
	_context->Shutdown();
}

void KinectCtrl::updateSkeleton(){
	UserGenerator *user = _user;


	XnUserID users[10];
	XnUInt16 nUsers = 10;
	user->GetUsers(users, nUsers);


	KinectStatus *kinectStatus = getKinectStatus();
	XnSkeletonJointPosition jointPos;
	XnSkeletonJointOrientation jointOri;
	int jointIndex;
	if (user->GetSkeletonCap().IsTracking(users[0]))
	{
		kinectStatus->isTracking = true;
		for(int totalJoints = 24; totalJoints != 0; totalJoints--){
			_user->GetSkeletonCap().GetSkeletonJointPosition(users[0],(XnSkeletonJoint)totalJoints,jointPos);
			_user->GetSkeletonCap().GetSkeletonJointOrientation(users[0],(XnSkeletonJoint)totalJoints,jointOri);
			//position
			jointIndex = (totalJoints-1)*3;
			kinectStatus->skeletonPointsPos[jointIndex+0] = jointPos.position.X;
			kinectStatus->skeletonPointsPos[jointIndex+1] = jointPos.position.Y;
			kinectStatus->skeletonPointsPos[jointIndex+2] = jointPos.position.Z;
			//orientation
			jointIndex = (totalJoints-1)*9;
			//X column
			kinectStatus->skeletonPointsOri[jointIndex+0] = jointOri.orientation.elements[0];
			kinectStatus->skeletonPointsOri[jointIndex+1] = jointOri.orientation.elements[3];
			kinectStatus->skeletonPointsOri[jointIndex+2] = jointOri.orientation.elements[6];
			//Y column
			kinectStatus->skeletonPointsOri[jointIndex+3] = jointOri.orientation.elements[1];
			kinectStatus->skeletonPointsOri[jointIndex+4] = jointOri.orientation.elements[4];
			kinectStatus->skeletonPointsOri[jointIndex+5] = jointOri.orientation.elements[7];
			//Z column
			kinectStatus->skeletonPointsOri[jointIndex+6] = jointOri.orientation.elements[2];
			kinectStatus->skeletonPointsOri[jointIndex+7] = jointOri.orientation.elements[5];
			kinectStatus->skeletonPointsOri[jointIndex+8] = jointOri.orientation.elements[8];
		}
	}else kinectStatus->isTracking = false;
}





//NITE CALLBACKS
void XN_CALLBACK_TYPE Push_Pushed(XnFloat fVelocity, XnFloat fAngle, void *cxt){
	printf("Push detected: vel(%f) angle(%f)\n",fVelocity,fAngle);
}

void XN_CALLBACK_TYPE FocusProgress(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt)
{
	printf("Focus progress: %s @(%f,%f,%f): %f\n", strFocus, ptPosition.X, ptPosition.Y, ptPosition.Z, fProgress);
}

void XN_CALLBACK_TYPE SessionStart(const XnPoint3D& PFOCUS_EVENT_RECORD, void* UserCxt){
	printf("SessionManager: start\n");
}

void XN_CALLBACK_TYPE SessionEnd(void* UserCxt){
	printf("SessionManager: end\n");
}

void XN_CALLBACK_TYPE Hand_Created(HandsGenerator &hNode, XnUserID user, const XnPoint3D *pPosition, XnFloat fTime, void *pCookie){
	printf("HandCreated: user(%d) pos(%f,%f,%f) time(%f)\n",user,pPosition->X,pPosition->Y,pPosition->Z,fTime);
	KinectCtrl::KinectStatus *status = (KinectCtrl::KinectStatus*) pCookie;
	status->anyHandDetected = true;
}

void XN_CALLBACK_TYPE Hand_Updated(HandsGenerator &generator, XnUserID user, const XnPoint3D *pPosition, XnFloat fTime, void *pCookie){
	//printf("HandUpdated: user(%d) pos(%f,%f,%f) time(%f)\n",user,pPosition->X,pPosition->Y,pPosition->Z,fTime);
	KinectCtrl::KinectStatus *status = (KinectCtrl::KinectStatus*) pCookie;
	status->anyHandPos[0] = pPosition->X;
	status->anyHandPos[1] = pPosition->Y;
	status->anyHandPos[2] = pPosition->Z;
}

void XN_CALLBACK_TYPE Hand_Destroyed(HandsGenerator &generator, XnUserID user, XnFloat fTime, void *pCookie){
	printf("HandDestroyed: user(%d) time(%f)\n",user,fTime);
	KinectCtrl::KinectStatus *status = (KinectCtrl::KinectStatus*) pCookie;
	status->anyHandDetected = false;
}

void XN_CALLBACK_TYPE User_Created(UserGenerator &generator, XnUserID user, void *pCookie){
	printf("New user identified: %d\n", user);
	generator.GetPoseDetectionCap().StartPoseDetection("Psi", user);
}

void XN_CALLBACK_TYPE User_Destroyed(UserGenerator &generator, XnUserID user, void *pCookie){
	printf("User lost: %d\n", user);
}

void XN_CALLBACK_TYPE CalibrationStart(xn::SkeletonCapability& skeleton, XnUserID user, void* pCookie)
{
	printf("Calibration start for user %d\n", user);
}
void XN_CALLBACK_TYPE CalibrationEnd(xn::SkeletonCapability& skeleton, XnUserID userID, XnBool bSuccess, void* pCookie)
{
	printf("Calibration complete for user %d: %s\n", userID, bSuccess?"Success":"Failure");
	if (bSuccess)
	{
		skeleton.StartTracking(userID);
	}
	else
	{
		UserGenerator *user = (UserGenerator*)pCookie;
		user->GetPoseDetectionCap().StartPoseDetection("Psi", userID);
	}
}
void XN_CALLBACK_TYPE PoseDetected(xn::PoseDetectionCapability& poseDetection, const XnChar* strPose, XnUserID nId, void* pCookie)
{
	printf("Pose '%s' detected for user %d\n", strPose, nId);
	UserGenerator *user = (UserGenerator*)pCookie;
	user->GetSkeletonCap().RequestCalibration(nId, false);
	user->GetPoseDetectionCap().StopPoseDetection(nId);
}