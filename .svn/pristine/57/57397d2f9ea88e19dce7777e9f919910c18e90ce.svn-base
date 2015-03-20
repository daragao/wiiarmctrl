#include "PointTracker.h"

PointTracker::PointTracker(DepthGenerator *depthGen):XnVPointControl("PointTracker")
{
	_pointTrackerStatus = new PointTrackerStatus();
	initPointTrackerStatus();
	_depthGen = depthGen;
}

PointTracker::~PointTracker(void){
}

void PointTracker::initPointTrackerStatus(){
	_pointTrackerStatus->pointLastUpdate = -1;
	_pointTrackerStatus->pointProjCoords[0] = 0;
	_pointTrackerStatus->pointProjCoords[1] = 0;
	_pointTrackerStatus->pointProjCoords[2] = 0;
	_pointTrackerStatus->pointRealCoords[0] = 0;
	_pointTrackerStatus->pointRealCoords[1] = 0;
	_pointTrackerStatus->pointRealCoords[2] = 0;
}

PointTracker::PointTrackerStatus *PointTracker::getPointTracker(){
	return _pointTrackerStatus;
}


void PointTracker::Update(XnVMessage* pMessage){
	XnVPointControl::Update(pMessage);
}

void PointTracker::OnPointCreate(const XnVHandPointContext* cxt){
	OnPointUpdate(cxt);
}

void PointTracker::OnPointUpdate(const XnVHandPointContext* cxt){
	_pointTrackerStatus->pointLastUpdate = clock();

	XnPoint3D ptProjective(cxt->ptPosition);
	//printf("Point (%f,%f,%f)", ptProjective.X, ptProjective.Y, ptProjective.Z);//real
	_pointTrackerStatus->pointRealCoords[0] = ptProjective.X;
	_pointTrackerStatus->pointRealCoords[1] = ptProjective.Y;
	_pointTrackerStatus->pointRealCoords[2] = ptProjective.Z;
	_depthGen->ConvertRealWorldToProjective(1, &ptProjective, &ptProjective);
	_pointTrackerStatus->pointProjCoords[0] = ptProjective.X;
	_pointTrackerStatus->pointProjCoords[1] = ptProjective.Y;
	_pointTrackerStatus->pointProjCoords[2] = ptProjective.Z;
	//printf(" -> (%f,%f,%f)\n", ptProjective.X, ptProjective.Y, ptProjective.Z);//projective
}

void PointTracker::OnPointDestroy(XnUInt32 nID){
	initPointTrackerStatus();
}
