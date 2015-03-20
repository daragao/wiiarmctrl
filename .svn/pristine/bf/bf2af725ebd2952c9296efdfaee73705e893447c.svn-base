#include "PortCtrlMod.h"

double PortCtrlMod::getPeriod(){
	return 0.05;
}

bool PortCtrlMod::updateModule(){
	_handTracker->updateKinect();
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

	_handTracker = new KinectHandTracker();

	string localPort = "/kinectHandTracker";
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
	_handTracker->close();
	_handlerPort->close();
	return true;
}

void PortCtrlMod::sendKinectData(){
	static long pointLastUpdate = -1;
	KinectHandTracker::KinectStatus *kinectStatus = KinectHandTracker::getKinect();
	if(kinectStatus->sessionState == IN_SESSION){
		if(kinectStatus->pointLastUpdate != -1 && kinectStatus->pointLastUpdate != pointLastUpdate){
			pointLastUpdate = kinectStatus->pointLastUpdate;
			Bottle &bot = _handlerPort->prepare();
			bot.clear();
			bot.addVocab(VOCAB3('P','O','S'));
			Bottle &realList = bot.addList();
			realList.addDouble(kinectStatus->pointRealCoords[0]);//in milimeters (I think!)
			realList.addDouble(kinectStatus->pointRealCoords[1]);
			realList.addDouble(kinectStatus->pointRealCoords[2]);
			Bottle &projList = bot.addList();
			projList.addDouble(kinectStatus->pointProjCoords[0]);
			projList.addDouble(kinectStatus->pointProjCoords[1]);
			projList.addDouble(kinectStatus->pointProjCoords[2]);
			_handlerPort->write();
		}
	}
	pointLastUpdate = kinectStatus->pointLastUpdate;
}
