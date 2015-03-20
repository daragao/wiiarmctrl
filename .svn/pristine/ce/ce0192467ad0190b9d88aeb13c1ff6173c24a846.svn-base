#include "PortCtrlMod.h"

double PortCtrlMod::getPeriod(){
	return 0.02;
}

bool PortCtrlMod::updateModule(){
	KinectCtrl *kinect = _kinect;
	kinect->updateFrame();
	KinectCtrl::KinectStatus *kinectStatus = _kinect->getKinectStatus();
	if(kinectStatus->isTracking){
		double *skeletonPointsPos = kinectStatus->skeletonPointsPos; 
		double *skeletonPointsOri = kinectStatus->skeletonPointsOri; 
		Bottle &bot = _handlerPort->prepare();
		bot.clear();
		bot.addVocab(VOCAB3('P','O','S'));
		for(int i = 0; i < (24*3); i++)
			bot.addDouble(skeletonPointsPos[i]);
		_handlerPort->write();
		Bottle &botOri = _handlerPort->prepare();
		botOri.clear();
		botOri.addVocab(VOCAB3('O','R','I'));
		for(int i = 0; i < (24*9); i++)
			botOri.addDouble(skeletonPointsOri[i]);
		_handlerPort->write();
	}
	return true;
}

bool PortCtrlMod::respond(const Bottle& command, Bottle& reply){
	if(command.get(0).asString()=="quit")
		return false;
	else{
		printf("echo: %s",command.toString().c_str());//debug
	}
	return true;
}

bool PortCtrlMod::configure(ResourceFinder &rf){

	KinectCtrl *kinect = new KinectCtrl();

	_handlerPort = new BufferedPort<Bottle>;
	_handlerPort->open("/kinectPort");

	_kinect = kinect;
	return true;
}

bool PortCtrlMod::interruptModule(){
	return true;
}

bool PortCtrlMod::close(){
	if(_kinect!=NULL)
		_kinect->close();
	return true;
}
