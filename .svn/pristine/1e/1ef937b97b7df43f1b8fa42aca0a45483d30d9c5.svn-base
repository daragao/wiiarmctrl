#include "KinectSkeletonTracker.h"

KinectSkeletonTracker::KinectStatus *KinectSkeletonTracker::_kinectStatus;

#define CHECK_RC(rc, what)											\
	if (rc != XN_STATUS_OK)											\
	{																\
	printf("%s failed: %s\n", what, xnGetStatusString(rc));		\
	return rc;													\
	}

#define CHECK_ERRORS(rc, errors, what)		\
	if (rc == XN_STATUS_NO_NODE_PRESENT)	\
	{										\
	XnChar strError[1024];				\
	errors.ToString(strError, 1024);	\
	printf("%s\n", strError);			\
	return (rc);						\
	}

KinectSkeletonTracker::KinectSkeletonTracker(void)
{
	initVars();
	init();
}

KinectSkeletonTracker::~KinectSkeletonTracker(void)
{
}

void KinectSkeletonTracker::close(){
	_context->Shutdown();
}

void KinectSkeletonTracker::initVars(){
	_kinectStatus = new KinectStatus();
	for(int i = 0; i < MAX_USERS; i++){
		_kinectStatus->userSkeleton[i].skeletonState = NO_USER;
	}
}

KinectSkeletonTracker::KinectStatus *KinectSkeletonTracker::getKinect(){
	return _kinectStatus;
}

int KinectSkeletonTracker::init(){

	// xml to initialize OpenNI
	string sample_xml_path = "C:/Program Files/PrimeSense/NITE/Data/Sample-User.xml";

	XnStatus rc = XN_STATUS_OK;
	xn::EnumerationErrors errors;
	XnCallbackHandle h_user,h_calib,h_pose;

	// Initialize OpenNI
	_context = new Context();
	rc = _context->InitFromXmlFile(sample_xml_path.c_str(), &errors);
	CHECK_ERRORS(rc, errors, "InitFromXmlFile");
	CHECK_RC(rc, "InitFromXmlFile");

	_userGenerator = new UserGenerator();
	rc = _context->FindExistingNode(XN_NODE_TYPE_USER, *_userGenerator);
	CHECK_RC(rc, "Find user generator");
	_userGenerator->RegisterUserCallbacks(UserCreated,UserDestroyed,NULL,h_user);

	//create skeleton
	_userGenerator->GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
	_userGenerator->GetSkeletonCap().SetSmoothing(0.6f);
	_userGenerator->GetSkeletonCap().RegisterCalibrationCallbacks(&CalibrationStart,&CalibrationEnd,_userGenerator,h_calib);
	_userGenerator->GetPoseDetectionCap().RegisterToPoseCallbacks(&PoseDetected,/*&PoseEnded*/NULL,_userGenerator,h_pose);

	// Initialization done. Start generating
	rc = _context->StartGeneratingAll();
	CHECK_RC(rc, "StartGenerating");

	return rc;
}

void KinectSkeletonTracker::updateKinect(){
	// Read next available data
	_context->WaitAndUpdateAll();

	//get data per user
	XnUserID users[MAX_USERS];
	XnUInt16 nUsers = MAX_USERS;
	_userGenerator->GetUsers(users,nUsers);

	//UserSkeleton *userSkeleton = getKinect()->userSkeleton;
	XnSkeletonJointTransformation joint;
	int jointIndex;
	for(int userIndex = 0; userIndex < MAX_USERS; userIndex++){
		if(users[userIndex]>MAX_USERS) continue;
		if(_userGenerator->GetSkeletonCap().IsTracking(users[userIndex])){
			getKinect()->userSkeleton[users[userIndex]].skeletonState = SKELETON_TRACKING;
			for(int totalJoints = TOTAL_JOINTS; totalJoints != 0; totalJoints--){
				jointIndex = (totalJoints-1);
				_userGenerator->GetSkeletonCap().GetSkeletonJoint(users[userIndex],(XnSkeletonJoint)totalJoints,joint);
				//position
				getKinect()->userSkeleton[users[userIndex]].skeletonPointsPos[jointIndex][0] = joint.position.position.X;
				getKinect()->userSkeleton[users[userIndex]].skeletonPointsPos[jointIndex][1] = joint.position.position.Y;
				getKinect()->userSkeleton[users[userIndex]].skeletonPointsPos[jointIndex][2] = joint.position.position.Z;
				//orientation
				getKinect()->userSkeleton[users[userIndex]].skeletonPointsOri[jointIndex][0] = joint.orientation.orientation.elements[0];
				getKinect()->userSkeleton[users[userIndex]].skeletonPointsOri[jointIndex][1] = joint.orientation.orientation.elements[3];
				getKinect()->userSkeleton[users[userIndex]].skeletonPointsOri[jointIndex][2] = joint.orientation.orientation.elements[6];
				getKinect()->userSkeleton[users[userIndex]].skeletonPointsOri[jointIndex][3] = joint.orientation.orientation.elements[1];
				getKinect()->userSkeleton[users[userIndex]].skeletonPointsOri[jointIndex][4] = joint.orientation.orientation.elements[4];
				getKinect()->userSkeleton[users[userIndex]].skeletonPointsOri[jointIndex][5] = joint.orientation.orientation.elements[7];
				getKinect()->userSkeleton[users[userIndex]].skeletonPointsOri[jointIndex][6] = joint.orientation.orientation.elements[2];
				getKinect()->userSkeleton[users[userIndex]].skeletonPointsOri[jointIndex][7] = joint.orientation.orientation.elements[5];
				getKinect()->userSkeleton[users[userIndex]].skeletonPointsOri[jointIndex][8] = joint.orientation.orientation.elements[8];
			}
		}else getKinect()->userSkeleton[users[userIndex]].skeletonState = NO_USER;
	}
}

void XN_CALLBACK_TYPE UserCreated(UserGenerator& generator, XnUserID userID, void* pCookie){
	printf("New user identified: %d\n",userID);
	generator.GetPoseDetectionCap().StartPoseDetection("Psi",userID);
	KinectSkeletonTracker::getKinect()->userSkeleton[userID].skeletonState = USER_DETECTED;
}

void XN_CALLBACK_TYPE UserDestroyed(UserGenerator& generator, XnUserID userID, void* pCookie){
	printf("User lost: %d\n",userID);
	KinectSkeletonTracker::getKinect()->userSkeleton[userID].skeletonState = NO_USER;
}

void XN_CALLBACK_TYPE PoseDetected(PoseDetectionCapability &poseDetection, const XnChar* strPose, XnUserID userID, void *pCookie){
	printf("Pose '%s' for user %d detected\n",strPose,userID);
	UserGenerator* userGenerator = (UserGenerator*)pCookie;
	userGenerator->GetSkeletonCap().RequestCalibration(userID,false);
	userGenerator->GetPoseDetectionCap().StopPoseDetection(userID);
	KinectSkeletonTracker::getKinect()->userSkeleton[userID].skeletonState = CALIBRATING;
}

void XN_CALLBACK_TYPE CalibrationStart(SkeletonCapability &skeleton, XnUserID userID, void *pCookie){
	printf("Calibration started for user %d\n",userID);
}

void XN_CALLBACK_TYPE CalibrationEnd(SkeletonCapability &skeleton, XnUserID userID, XnBool bSuccess, void *pCookie){
	printf("Calibration complete for user %d: %s\n",userID,(bSuccess?"Success!":"Failure..."));
	if(bSuccess){
		skeleton.StartTracking(userID);
		KinectSkeletonTracker::getKinect()->userSkeleton[userID].skeletonState = SKELETON_TRACKING;
	}else{
		UserGenerator *userGenerator = (UserGenerator*)pCookie;
		userGenerator->GetPoseDetectionCap().StartPoseDetection("Psi",userID);
		KinectSkeletonTracker::getKinect()->userSkeleton[userID].skeletonState = USER_DETECTED;
	}
}
