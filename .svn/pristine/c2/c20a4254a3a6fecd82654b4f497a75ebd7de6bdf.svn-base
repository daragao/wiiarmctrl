#include "PortCtrlMod.h"

double PortCtrlMod::getPeriod(){
	return 0.00;
}

bool PortCtrlMod::updateModule(){
	for(int i = 0; i != _niCubgen; i++)
		_icubGen[i]->updatePosition(_isTracking);
	return true;
}

bool PortCtrlMod::respond(const Bottle& command, Bottle& reply){
	if(command.get(0).asString()=="quit"){
		return false;
	}else printf("echo: %s",command.toString().c_str());
	return true;
}

void PortCtrlMod::onRead(Bottle &command){
	iCubGeneric::updateTimestamp();
	if(_useSkeleton){//with skeleton move through motors
		//_isTracking = false;
		int posIndex = 0;
		int oriIndex = 0;
		Bottle *bottleList;
		for(int i = 0; i < command.size(); i++){
			switch(command.get(i).asVocab()){
			case VOCAB3('B','O','T'):
				if(command.get(++i).asVocab() == VOCAB1('B')) _isTracking = true;
				else _isTracking = false;
				break;
			case VOCAB3('P','O','S'):
				bottleList = command.get(++i).asList();
				iCubGeneric::setJointPos(posIndex++,bottleList->get(0).asDouble(),bottleList->get(1).asDouble(),bottleList->get(2).asDouble());
				break;
			case VOCAB3('O','R','I'):
				bottleList = command.get(++i).asList();
				iCubGeneric::setJointOri(oriIndex++,bottleList->get(0).asDouble(),bottleList->get(1).asDouble(),bottleList->get(2).asDouble()
					,bottleList->get(3).asDouble(),bottleList->get(4).asDouble(),bottleList->get(5).asDouble()
					,bottleList->get(6).asDouble(),bottleList->get(7).asDouble(),bottleList->get(8).asDouble());
				break;
			}
		}
	}else{//without skeleton use iKin
		for(int i = 0; i < command.size(); i++){
			switch(command.get(i).asVocab()){
				case VOCAB3('P','O','S'):
					Bottle *realBottle = command.get(++i).asList();
					Bottle *projBottle = command.get(++i).asList();
					iCubGeneric::setHandRealPos(realBottle->get(0).asDouble(),realBottle->get(1).asDouble(),realBottle->get(2).asDouble());
					iCubGeneric::setHandProjPos(projBottle->get(0).asDouble(),projBottle->get(1).asDouble(),projBottle->get(2).asDouble());
					break;
			}
		}
	}
}

bool PortCtrlMod::configure(ResourceFinder &rf){
	_isTracking = false;
	_useSkeleton = true;
	if(rf.check("controlType") && rf.find("controlType").asString() == "hand") _useSkeleton = false;
	string localPortPrefix = "icubKinect";
	if(rf.check("localPortPrefix")) localPortPrefix = rf.find("localPortPrefix").asString();
	string robotName = "icubSim";
	if(rf.check("robotName")) robotName = rf.find("robotName").asString();
	string partName = "all";
	if(rf.check("partName")) partName = rf.find("partName").asString();
	double speed = 10;
	if(rf.check("motorSpeed")) speed = rf.find("motorSpeed").asDouble();
	double acc = 50;
	if(rf.check("motorAcceleration")) acc = rf.find("motorAcceleration").asDouble();
	string wiiPort = "/";
	if(rf.check("wiiRemotePort")) wiiPort += rf.find("wiiRemotePort").asString();
	else wiiPort += "wiimote0";
	string remotePort = "/";
	if(rf.check("kinectRemotePort")){
		remotePort += rf.find("kinectRemotePort").asString();
	}else{
		if(_useSkeleton)
			remotePort += "kinectPort";
		else
			remotePort += "kinectHandTracker";
	}

	_niCubgen = 1;
	if(_useSkeleton){
		if(partName=="all"){
			_niCubgen = 4;
			_icubGen[0] = new iCubMotorCtrl(localPortPrefix,robotName,"left_arm");
			((iCubMotorCtrl*)_icubGen[0])->setSpeed(acc,speed);
			_icubGen[1] = new iCubMotorCtrl(localPortPrefix,robotName,"right_arm");
			((iCubMotorCtrl*)_icubGen[1])->setSpeed(acc,speed);
			_icubGen[2] = new iCubMotorCtrl(localPortPrefix,robotName,"torso");
			((iCubMotorCtrl*)_icubGen[2])->setSpeed(acc,speed);
			_icubGen[3] = new iCubMotorCtrl(localPortPrefix,robotName,"shoulders");
			((iCubMotorCtrl*)_icubGen[3])->setSpeed(acc,speed);

		}else _icubGen[0] = new iCubMotorCtrl(localPortPrefix,robotName,partName);
	}

	string localPort = "/"+localPortPrefix+"/inputPosition";
	_handlerPort = new BufferedPort<Bottle>();
	_handlerPort->open(localPort.c_str());
	_handlerPort->useCallback(*this);

	Network::connect(remotePort.c_str(),localPort.c_str());
	Network::connect(wiiPort.c_str(),localPort.c_str());

	attachTerminal();
	return true;
}

bool PortCtrlMod::interruptModule(){
	return true;
}

bool PortCtrlMod::close(){
	_handlerPort->close();
	for(int i = 0; i != _niCubgen; i++)
		_icubGen[i]->close();
	return true;
}