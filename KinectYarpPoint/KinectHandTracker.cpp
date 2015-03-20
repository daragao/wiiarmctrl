#include "KinectHandTracker.h"

KinectHandTracker::KinectStatus *KinectHandTracker::_kinectStatus;

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

KinectHandTracker::KinectHandTracker(void)
{
	initVars();
	init();
}

KinectHandTracker::~KinectHandTracker(void)
{
}

void KinectHandTracker::close(){
	_context->Shutdown();
}

void KinectHandTracker::initVars(){
	_kinectStatus = new KinectStatus();
	_kinectStatus->sessionState = NOT_IN_SESSION;
	_kinectStatus->pointLastUpdate = -1;
	_kinectStatus->pointProjCoords[0] = 0;
	_kinectStatus->pointProjCoords[1] = 0;
	_kinectStatus->pointProjCoords[2] = 0;
	_kinectStatus->pointRealCoords[0] = 0;
	_kinectStatus->pointRealCoords[1] = 0;
	_kinectStatus->pointRealCoords[2] = 0;
}

KinectHandTracker::KinectStatus *KinectHandTracker::getKinect(){
	return _kinectStatus;
}

int KinectHandTracker::init(){

	// xml to initialize OpenNI
	string sample_xml_path = "C:/Program Files/PrimeSense/NITE/Data/Sample-Tracking.xml";

	XnStatus rc = XN_STATUS_OK;
	xn::EnumerationErrors errors;

	// Initialize OpenNI
	_context = new Context();
	rc = _context->InitFromXmlFile(sample_xml_path.c_str(), &errors);
	CHECK_ERRORS(rc, errors, "InitFromXmlFile");
	CHECK_RC(rc, "InitFromXmlFile");

	_depthGenerator = new DepthGenerator();
	rc = _context->FindExistingNode(XN_NODE_TYPE_DEPTH, *_depthGenerator);
	CHECK_RC(rc, "Find depth generator");
	_handsGenerator = new HandsGenerator();
	rc = _context->FindExistingNode(XN_NODE_TYPE_HANDS, *_handsGenerator);
	_handsGenerator->SetSmoothing(0.8);
	CHECK_RC(rc, "Find hands generator");

	// Create NITE objects
	_sessionManager = new XnVSessionManager;
	rc = _sessionManager->Initialize(_context, "Click,Wave", "RaiseHand");
	CHECK_RC(rc, "SessionManager::Initialize");

	_sessionManager->RegisterSession(NULL,SessionStarting, SessionEnding, FocusProgress);


	_pointTracker = new PointTracker(_depthGenerator);//(20, g_DepthGenerator); 
	_flowRouter = new XnVFlowRouter;
	_flowRouter->SetActive(_pointTracker);

	_sessionManager->AddListener(_flowRouter);

	_pointTracker->RegisterNoPoints(NULL, NoHands);

	// Initialization done. Start generating
	rc = _context->StartGeneratingAll();
	CHECK_RC(rc, "StartGenerating");

	return rc;
}

void KinectHandTracker::updateKinect(){
	// Read next available data
	_context->WaitAndUpdateAll();
	// Update NITE tree
	_sessionManager->Update(_context);
	//update kinect status struct

	if(_kinectStatus->pointLastUpdate != _pointTracker->getPointTracker()->pointLastUpdate){
		_kinectStatus->pointLastUpdate = _pointTracker->getPointTracker()->pointLastUpdate;
		_kinectStatus->pointProjCoords[0] = _pointTracker->getPointTracker()->pointProjCoords[0];
		_kinectStatus->pointProjCoords[1] = _pointTracker->getPointTracker()->pointProjCoords[1];
		_kinectStatus->pointProjCoords[2] = _pointTracker->getPointTracker()->pointProjCoords[2];
		_kinectStatus->pointRealCoords[0] = _pointTracker->getPointTracker()->pointRealCoords[0];
		_kinectStatus->pointRealCoords[1] = _pointTracker->getPointTracker()->pointRealCoords[1];
		_kinectStatus->pointRealCoords[2] = _pointTracker->getPointTracker()->pointRealCoords[2];
	}
}


void XN_CALLBACK_TYPE SessionStarting(const XnPoint3D& ptPosition, void* UserCxt){
	printf("Session started!\n");
	KinectHandTracker::getKinect()->sessionState = IN_SESSION;
}
void XN_CALLBACK_TYPE SessionEnding(void* UserCxt){
	printf("Session Ended!\n");
	KinectHandTracker::getKinect()->sessionState = NOT_IN_SESSION;
}
void XN_CALLBACK_TYPE FocusProgress(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt){
	printf("Focus!\n");
	KinectHandTracker::getKinect()->sessionState = QUICK_REFOCUS;
}
void XN_CALLBACK_TYPE NoHands(void* UserCxt){
	printf("No hands!\n");
	if (KinectHandTracker::getKinect()->sessionState != NOT_IN_SESSION)
	{
		printf("Quick refocus\n");
		KinectHandTracker::getKinect()->sessionState = QUICK_REFOCUS;
	}
}
