#include "PortCtrlMod.h"

double PortCtrlMod::getPeriod(){
	return 0.05;
}

bool PortCtrlMod::updateModule(){
	_skeletonTracker->updateKinect();
	sendKinectData();
	return true;
}

bool PortCtrlMod::respond(const Bottle& command, Bottle& reply){
	if(command.get(0).asString()=="quit"){
		return false;
	}else printf("echo: %s",command.toString().c_str());
	return true;
}

void PortCtrlMod::onRead(Bottle &command){
}

bool PortCtrlMod::configure(ResourceFinder &rf){

	_skeletonTracker = new KinectSkeletonTracker();

	string localPort = "/kinectPort";
	_handlerPort = new BufferedPort<Bottle>();
	_handlerPort->open(localPort.c_str());
//	_handlerPort->useCallback(*this);

	attachTerminal();
	return true;
}

bool PortCtrlMod::interruptModule(){
	return true;
}

bool PortCtrlMod::close(){
	_skeletonTracker->close();
	_handlerPort->close();
	return true;
}

void PortCtrlMod::sendKinectData(){
	KinectSkeletonTracker::UserSkeleton *userSkeleton = KinectSkeletonTracker::getKinect()->userSkeleton;
	double *joint;
	for(int i = 0; i < MAX_USERS; i++){
		if(userSkeleton[i].skeletonState == SKELETON_TRACKING){
			Bottle &botSkeleton = _handlerPort->prepare();
			botSkeleton.clear();
			for(int jointIndex = 0; jointIndex < TOTAL_JOINTS; jointIndex++){
				//position
				botSkeleton.addVocab(VOCAB3('P','O','S'));
				joint = userSkeleton[i].skeletonPointsPos[jointIndex];
				Bottle &botList = botSkeleton.addList();
				botList.addDouble(joint[0]);
				botList.addDouble(joint[1]);
				botList.addDouble(joint[2]);
				//orientation
				joint = userSkeleton[i].skeletonPointsOri[jointIndex];
				botSkeleton.addVocab(VOCAB3('O','R','I'));
				Bottle &botList2 = botSkeleton.addList();
				botList2.addDouble(joint[0]);
				botList2.addDouble(joint[1]);
				botList2.addDouble(joint[2]);
				botList2.addDouble(joint[3]);
				botList2.addDouble(joint[4]);
				botList2.addDouble(joint[5]);
				botList2.addDouble(joint[6]);
				botList2.addDouble(joint[7]);
				botList2.addDouble(joint[8]);
			}
			_handlerPort->write();
		}
	}
}
