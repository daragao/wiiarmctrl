#include "WiiYarpPortsCtrl.h"

WiiYarpPortsCtrl::WiiYarpPortsCtrl(int wiimoteID, double sensitivity, WiiSphericalCoord *wiiSphericalCoord, double accelerator)
{
	stringstream out;
	out << wiimoteID;
	string classPortPrefix = "/WiiYarpPortsCtrl"+out.str();

	initVars();

	_veryLittle = sensitivity;
	_wiiSphericalCoord = wiiSphericalCoord;
	_portReader = new WiiYarpPortsReader(this);
	openPorts(wiimoteID,classPortPrefix);
	_accelerator = accelerator;
}

WiiYarpPortsCtrl::~WiiYarpPortsCtrl(void)
{
}

void WiiYarpPortsCtrl::initVars(){
	_veryLittle = 0;
	_wiiSphericalCoord	= NULL;
	_portReader			= NULL;
	_mp					= NULL;
	_wiiYarpWriter		= NULL;
	_icubSolverWriter	= NULL;
	_icubSolverReader	= NULL;
}

void WiiYarpPortsCtrl::openPorts(int wiimoteID, string classPortPrefix){
	setWriterPorts(wiimoteID,classPortPrefix);
	setReaderPorts(wiimoteID,classPortPrefix);
}

BufferedPort<Bottle> *WiiYarpPortsCtrl::openWiiYarpPortConnection(int wiimoteID, bool isWriter, string classPortPrefix, string part){
	stringstream out;
	out << wiimoteID;
	string yarpPortPrefix = "/wiimote"+out.str();
	string classPort;
	BufferedPort<Bottle> *port;
	port = new BufferedPort<Bottle>();
	classPort = classPortPrefix+part;
	string portName = yarpPortPrefix+part;
	port->open(classPort.c_str());
	if(isWriter)
		Network::connect(classPort.c_str(),portName.c_str());
	else 
		Network::connect(portName.c_str(),classPort.c_str());
	return port;
}

void WiiYarpPortsCtrl::setWriterPorts(int wiimoteID, string classPortPrefix){
	BufferedPort<Bottle> *port = NULL;
	port = openWiiYarpPortConnection(wiimoteID,true,classPortPrefix,"/reader");
	_wiiYarpWriter = port;
	string portName = classPortPrefix + "/solverWriter";
	port = new BufferedPort<Bottle>();
	port->open(portName.c_str());
	_icubSolverWriter = port;
}

void WiiYarpPortsCtrl::setReaderPorts(int wiimoteID, string classPortPrefix){
	BufferedPort<Bottle> *port = NULL;
	port = openWiiYarpPortConnection(wiimoteID,false,classPortPrefix,"/ext/motionplus");
	port->useCallback(*_portReader);
	_mp = port;
	string portName = classPortPrefix + "/solverReader";
	port = new BufferedPort<Bottle>();
	port->open(portName.c_str());
	port->useCallback(*_portReader);
	_icubSolverReader = port;
}

void WiiYarpPortsCtrl::setMotionPlus(bool setOn){
	int stateInt = (setOn?1:0);
	BufferedPort<Bottle> *writerPort = _wiiYarpWriter;
	Bottle& output = writerPort->prepare();
	output.clear();
	output.addVocab(WII_MOTIONPLUS_VOCAB);
	output.addInt(stateInt);
	writerPort->write();
}

void WiiYarpPortsCtrl::updateMotionPlusData(double status, double xRate, double yRate, double zRate, double xAngle, double yAngle, double zAngle){
	double angle[3] = {0,0,0};
	//filter movement
	double veryLittle = _veryLittle;
	if(abs(xAngle)>=veryLittle) angle[0] = xAngle*_accelerator;
	if(abs(yAngle)>=veryLittle) angle[1] = yAngle*_accelerator;
	if(abs(zAngle)>=veryLittle) angle[2] = -zAngle*_accelerator;
	_wiiSphericalCoord->updateMotionPlusData(status,xRate,yRate,zRate,angle[0],angle[1],angle[2]);
}

void WiiYarpPortsCtrl::setRotationMatrix(double rotMatrix[3][3]){
	_wiiSphericalCoord->setNewRotationMatrix(rotMatrix);
}

void WiiYarpPortsCtrl::setVectorPosition(double xPos, double yPos, double zPos){
	_wiiSphericalCoord->setNewPosition(xPos,yPos,zPos);
}

void WiiYarpPortsCtrl::setAxisAngle(double angle,double xPos, double yPos, double zPos){
	_wiiSphericalCoord->setNewPosition(angle,xPos,yPos,zPos);
}

void WiiYarpPortsCtrl::writeDataToPort(){
	Vector axisAngle = _wiiSphericalCoord->getAxisAngle();
	Vector point = _wiiSphericalCoord->getPointCoord();
	Vector rotMat = _wiiSphericalCoord->getLastRotationMatrix();
	//[SOL] [PNT] x y z [ROTM] (RotationMatrix)
	BufferedPort<Bottle> *writerPort = _icubSolverWriter;
	Bottle& output = writerPort->prepare();
	output.clear();
	output.addVocab(WII_ICUBSOLVER_VOCAB);
	output.addVocab(WIISOLVER_POINT_VOCAB);
	output.addDouble(point[0]);
	output.addDouble(point[1]);
	output.addDouble(point[2]);
	output.addVocab(WIISOLVER_ROTMAT_VOCAB);
	output.addDouble(rotMat[0]);
	output.addDouble(rotMat[1]);
	output.addDouble(rotMat[2]);
	output.addDouble(rotMat[3]);
	output.addDouble(rotMat[4]);
	output.addDouble(rotMat[5]);
	output.addDouble(rotMat[6]);
	output.addDouble(rotMat[7]);
	output.addDouble(rotMat[8]);
	/*output.addVocab(WII_ICUBSOLVER_VOCAB);
	output.addDouble(axisAngle[0]);
	output.addDouble(axisAngle[1]);
	output.addDouble(axisAngle[2]);
	output.addDouble(axisAngle[3]);*/
	writerPort->write();
}

void WiiYarpPortsCtrl::closePorts(){
	if(!_mp->isClosed())				_mp->close();
	if(!_wiiYarpWriter->isClosed())		_wiiYarpWriter->close();
	if(!_icubSolverWriter->isClosed())	_icubSolverWriter->close();
	if(!_icubSolverReader->isClosed())	_icubSolverReader->close();
}

//WiiYarpPortsReader||WiiYarpPortsReader||WiiYarpPortsReader||WiiYarpPortsReader||WiiYarpPortsReader||WiiYarpPortsReader
//WiiYarpPortsReader||WiiYarpPortsReader||WiiYarpPortsReader||WiiYarpPortsReader||WiiYarpPortsReader||WiiYarpPortsReader
//WiiYarpPortsReader||WiiYarpPortsReader||WiiYarpPortsReader||WiiYarpPortsReader||WiiYarpPortsReader||WiiYarpPortsReader

WiiYarpPortsReader::WiiYarpPortsReader(WiiYarpPortsCtrl *portsCtrl){
	_portsCtrl = portsCtrl;
}

WiiYarpPortsReader::~WiiYarpPortsReader(void){
}

void WiiYarpPortsReader::onRead(Bottle& b){
	WiiYarpPortsCtrl *portsCtrl = _portsCtrl;
	if(b.get(0).asVocab() == WII_MOTIONPLUS_VOCAB){
		portsCtrl->updateMotionPlusData(b.get(1).asDouble(),b.get(2).asDouble(),b.get(3).asDouble(),b.get(4).asDouble(),b.get(5).asDouble(),b.get(6).asDouble(),b.get(7).asDouble());
		portsCtrl->writeDataToPort();
	}
	if(b.get(0).asVocab() == WIISOLVER_ROTMAT_VOCAB){
		double rotMatrix[3][3] = {{b.get(1).asDouble(),b.get(2).asDouble(),b.get(3).asDouble()},
		{b.get(4).asDouble(),b.get(5).asDouble(),b.get(6).asDouble()},
		{b.get(7).asDouble(),b.get(8).asDouble(),b.get(9).asDouble()}};
		portsCtrl->setRotationMatrix(rotMatrix);
	}
	if(b.get(0).asVocab() == WIISOLVER_AXISANGLE_VOCAB){
		portsCtrl->setAxisAngle(b.get(1).asDouble(),b.get(2).asDouble(),b.get(3).asDouble(),b.get(4).asDouble());
	}
	if(b.get(0).asVocab() == WIISOLVER_POINT_VOCAB){
		portsCtrl->setVectorPosition(b.get(1).asDouble(),b.get(2).asDouble(),b.get(3).asDouble());
	}
}
