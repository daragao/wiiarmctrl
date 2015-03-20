#include "GraspPortCtrl.h"

GraspPortCtrl::GraspPortCtrl(void)
{
	_graspPort = NULL;
}

GraspPortCtrl::~GraspPortCtrl(void)
{
}

void GraspPortCtrl::initPorts(string graspPortName){
	string portName = "/WiiYarpPortsCtrl"+graspPortName;
	_graspPort = new BufferedPort<Bottle>();
	_graspPort->open(portName.c_str());
	bool connected = Network::connect(portName.c_str(),graspPortName.c_str());
}

void GraspPortCtrl::closePort(){
	_graspPort->close();
}

void GraspPortCtrl::sendCommand(string command){
	BufferedPort<Bottle> *graspPort = _graspPort;
	Bottle& output = graspPort->prepare();
	output.clear();
	output.fromString(command.c_str());
	graspPort->write();
}
