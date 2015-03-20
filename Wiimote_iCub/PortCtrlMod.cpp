#include "PortCtrlMod.h"

double PortCtrlMod::getPeriod(){
	return 0.05;
}

bool PortCtrlMod::updateModule(){
	for(int i = 0; i != _niCubgen; i++){
		_icubGen[i]->updatePosition(_isTracking);
	}
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
	for(int i = 0; i < command.size(); i++){
			switch(command.get(i).asVocab()){
				case VOCAB3('B','O','T'):
					parseBotBottle(command);
					break;
				case VOCAB2('M','P'):
					parseMPBottle(command);
					break;
				case VOCAB3('S','O','L'):
					parseSolverBottle(command);
					break;
			}
	}
}

bool PortCtrlMod::configure(ResourceFinder &rf){
	_isTracking = false;
	_kinematicControl = true;
	if(rf.check("controlType") && rf.find("controlType").asString() == "motor") _kinematicControl = false;
	string wiiRemotePort = "wiimote0";
	if(rf.check("wiiRemotePort")) wiiRemotePort = rf.find("wiiRemotePort").asString();
	string wiiSolverRemotePort = "wiimoteSolver0";
	if(rf.check("wiiSolverRemotePort")) wiiSolverRemotePort = rf.find("wiiSolverRemotePort").asString();
	string localPortPrefix = "icubwiimotectrl";
	if(rf.check("localPortPrefix")) localPortPrefix = rf.find("localPortPrefix").asString();
	string robotName = "icubSim";
	if(rf.check("robot")) robotName = rf.find("robot").asString();
	string partName = "all";
	if(rf.check("part")) partName = rf.find("part").asString();
	double speed = 10;
	if(rf.check("motorSpeed")) speed = rf.find("motorSpeed").asDouble();
	double acc = 50;
	if(rf.check("motorAcceleration")) acc = rf.find("motorAcceleration").asDouble();
	double trajTime = 10;
	if(rf.check("trajTime")) trajTime = rf.find("trajTime").asDouble();
	string remotePort = "/";

	_niCubgen = 1;
	if(_kinematicControl){
		_icubGen[0] = new iCubiKinCtrl(localPortPrefix,robotName,partName,trajTime);
	}else{
		_icubGen[0] = new iCubMotorCtrl(localPortPrefix,robotName,partName);
		((iCubMotorCtrl*)_icubGen[0])->setSpeed(acc,speed);
	}

	setupPorts(localPortPrefix,wiiSolverRemotePort, wiiRemotePort);

	attachTerminal();
	return true;
}

bool PortCtrlMod::interruptModule(){
	return true;
}

bool PortCtrlMod::close(){
	for(int i = 0; i != TOTAL_WIIMOTE_PARTS; i++)
		if(_handlerPort[i]!=NULL)_handlerPort[i]->close();
	for(int i = 0; i != _niCubgen; i++)
		if(_icubGen[i]!=NULL)_icubGen[i]->close();
	return true;
}

//PORT CONNECTION
void PortCtrlMod::setupPorts(string localPortPrefix, string wiiSolverRemotePort, string wiimoteRemotePort){
	string localPort,remotePort;
	//Readers
	//BOTS
	localPort = "/"+localPortPrefix+"/"+wiimoteRemotePort+"/bots";
	_handlerPort[BOTS] = new BufferedPort<Bottle>();
	_handlerPort[BOTS]->open(localPort.c_str());
	_handlerPort[BOTS]->useCallback(*this);
	remotePort = "/"+wiimoteRemotePort+"/bots";
	Network::connect(remotePort.c_str(),localPort.c_str());
	//MOTION PLUS
	localPort = "/"+localPortPrefix+"/"+wiimoteRemotePort+"/ext/motionplus";
	_handlerPort[MP] = new BufferedPort<Bottle>();
	_handlerPort[MP]->open(localPort.c_str());
	_handlerPort[MP]->useCallback(*this);
	remotePort = "/"+wiimoteRemotePort+"/ext/motionplus";
	Network::connect(remotePort.c_str(),localPort.c_str());
	//SOLVER
	localPort = "/"+localPortPrefix+"/"+wiiSolverRemotePort+"/solverReader";
	_handlerPort[SOLVERREADER] = new BufferedPort<Bottle>();
	_handlerPort[SOLVERREADER]->open(localPort.c_str());
	_handlerPort[SOLVERREADER]->useCallback(*this);
	remotePort = "/"+wiiSolverRemotePort+"/solverWriter";
	Network::connect(remotePort.c_str(),localPort.c_str());
	//Writers
	//WIIMOTE WRITER
	localPort = localPortPrefix+"/wiiwriter";
	_handlerPort[WIIWRITER] = new BufferedPort<Bottle>();
	_handlerPort[WIIWRITER]->open(localPort.c_str());
	remotePort = "/"+wiimoteRemotePort+"/reader";
	Network::connect(localPort.c_str(),remotePort.c_str());
	//SOLVER WRITER
	localPort = localPortPrefix+"/solverwriter";
	_handlerPort[SOLVERWRITER] = new BufferedPort<Bottle>();
	_handlerPort[SOLVERWRITER]->open(localPort.c_str());
	remotePort = "/"+wiiSolverRemotePort+"/solverReader";
	Network::connect(localPort.c_str(),remotePort.c_str());
}

//BOTTLE PARSING
void PortCtrlMod::parseBotBottle(Bottle botBottle){
	_isTracking = false;
	for(int i = 1; i < botBottle.size(); i++){
			switch(botBottle.get(i).asVocab()){
				case VOCAB1('B'):
					_isTracking = true;
					break;
				case VOCAB1('H'):
					iCubGeneric::setControlID(0);
					break;
				case VOCAB1('1'):
					iCubGeneric::setControlID(1);
					break;
				case VOCAB1('2'):
					iCubGeneric::setControlID(2);
					break;
			}
	}
}

void PortCtrlMod::parseMPBottle(Bottle mpBottle){
	double calibStatus = mpBottle.get(1).asDouble();
	Vector mpRaw(3),mpAngle(3);
	mpRaw[0] = mpBottle.get(2).asDouble();
	mpRaw[1] = mpBottle.get(3).asDouble();
	mpRaw[2] = mpBottle.get(4).asDouble();
	mpAngle[0] = (abs(mpBottle.get(5).asDouble()) > VERY_LITTLE) ? mpBottle.get(5).asDouble() : 0;
	mpAngle[1] = (abs(mpBottle.get(6).asDouble()) > VERY_LITTLE) ? mpBottle.get(6).asDouble() : 0;
	mpAngle[2] = (abs(mpBottle.get(7).asDouble()) > VERY_LITTLE) ? mpBottle.get(7).asDouble() : 0;
	
	if(_isTracking){
		mpAngle = mpAngle + iCubGeneric::getMPAngleVector();
		mpRaw = mpRaw + iCubGeneric::getMPRawVector();
	}

	iCubGeneric::setMPAngleVector(mpAngle[0],mpAngle[1],mpAngle[2]);
	iCubGeneric::setMPRawVector(mpRaw[0],mpRaw[1],mpRaw[2]);
}

void PortCtrlMod::parseSolverBottle(Bottle solverBottle){
	int i = 1;
	//solverBottle.get(1).asVocab() == PNT (Point)
	i++;
	double point[3];
	point[0] = solverBottle.get(i++).asDouble();
	point[1] = solverBottle.get(i++).asDouble();
	point[2] = solverBottle.get(i++).asDouble();
	//solverBottle.get(5).asVocab() == ROTM (RotationMatrix)
	i++;
	double rotMat[3][3];
	rotMat[0][0] = solverBottle.get(i++).asDouble();
	rotMat[0][1] = solverBottle.get(i++).asDouble();
	rotMat[0][2] = solverBottle.get(i++).asDouble();
	rotMat[1][0] = solverBottle.get(i++).asDouble();
	rotMat[1][1] = solverBottle.get(i++).asDouble();
	rotMat[1][2] = solverBottle.get(i++).asDouble();
	rotMat[2][0] = solverBottle.get(i++).asDouble();
	rotMat[2][1] = solverBottle.get(i++).asDouble();
	rotMat[2][2] = solverBottle.get(i++).asDouble();

	iCubGeneric::setSolverVector(point[0],point[1],point[2]);
	iCubGeneric::setSolverMatrix( rotMat[0][0],rotMat[0][1],rotMat[0][2],
		rotMat[1][0],rotMat[1][1],rotMat[1][2],
		rotMat[2][0],rotMat[2][1],rotMat[2][2]);
}

//SEND DATA
void PortCtrlMod::setRotationMatrix(double rotMAtrix[3][3]){
	BufferedPort<Bottle> *writerPort = _solverWriter;
	Bottle& output = writerPort->prepare();
	output.clear();
	output.addVocab(WIISOLVER_ROTMAT_VOCAB);
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			output.addDouble(rotMAtrix[i][j]);
	writerPort->write();
}