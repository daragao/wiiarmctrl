#include "PortCtrlMod.h"

double PortCtrlMod::getPeriod(){
	return 0.02;
}

bool PortCtrlMod::updateModule(){
	return true;
}

bool PortCtrlMod::respond(const Bottle& command, Bottle& reply){
	if(command.get(0).asString()=="quit"){
		close();
		return false;
	}
	return true;
}

void PortCtrlMod::onRead(Bottle &command){
	int posIndex = 0;
	int oriIndex = 0;
	Bottle *bottleList;
	for(int i = 0; i < command.size(); i++){
		switch(command.get(i).asVocab()){
			case VOCAB3('P','O','S'):
				bottleList = command.get(++i).asList();
				_glWindow->setJointPos(posIndex++,bottleList->get(0).asDouble());
				_glWindow->setJointPos(posIndex++,bottleList->get(1).asDouble());
				_glWindow->setJointPos(posIndex++,bottleList->get(2).asDouble());
				break;
			case VOCAB3('O','R','I'):
				bottleList = command.get(++i).asList();
				_glWindow->setJointOri(oriIndex,bottleList->get(0).asDouble(),bottleList->get(1).asDouble(),bottleList->get(2).asDouble()
					,bottleList->get(3).asDouble(),bottleList->get(4).asDouble(),bottleList->get(5).asDouble()
					,bottleList->get(6).asDouble(),bottleList->get(7).asDouble(),bottleList->get(8).asDouble());
				oriIndex++;
				break;
		}
	}
	if(_glWindow!=NULL)
		_glWindow->redraw();
}

bool PortCtrlMod::configure(ResourceFinder &rf){
	_glWindow = NULL;
	_handlerPort = new BufferedPort<Bottle>();
	_handlerPort->open("/skeletonViewerPort");
	_handlerPort->useCallback(*this);

	Network::connect("/kinectPort","/skeletonViewerPort");
	return true;
}

bool PortCtrlMod::interruptModule(){
	return true;
}

bool PortCtrlMod::close(){
	exit(0);
	return true;
}


void PortCtrlMod::prepapreGLWindow(int argc, char *argv[]){
	_glWindow = new GLWindow(argc,argv);
	_glWindow->mainLoop();
}