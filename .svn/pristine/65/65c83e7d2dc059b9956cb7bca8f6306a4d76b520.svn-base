#include <stdio.h>
#include <yarp/os/all.h>
#include "WiiYarpPortsCtrl.h"

using namespace yarp::os;

int main(int argc, char *argv[]) {
	printf("Press a key to start...\n");
	getchar();

	Network yarp;
	if(!yarp.checkNetwork()){
		printf("YARP Network off!\n");
		return 0;
	}

	
	ResourceFinder rf;
	rf.setVerbose();
	rf.setDefaultContext("../../YarpDeviceModule/SVNwiiarmctrl/iCubWiiSolver");
	rf.setDefaultConfigFile("iCubWiiSolver.ini");
	rf.configure("ICUB_ROOT", argc, argv);

	int wiimoteID = rf.find("wiimote").asInt();
	double sensitivity = rf.find("sensitivity").asDouble();
	double accelerator = rf.find("accelerator").asDouble();
	Bottle *firstVectorList = rf.find("firstvector").asList();
	double firstPoint[3] = {firstVectorList->get(0).asDouble(),firstVectorList->get(1).asDouble(),firstVectorList->get(2).asDouble()};

	WiiSphericalCoord *sphericalCoord = new WiiSphericalCoord(firstPoint);
	WiiYarpPortsCtrl *wiiYarpPortsCtrl = new WiiYarpPortsCtrl(wiimoteID,sensitivity,sphericalCoord,accelerator);
	wiiYarpPortsCtrl->setMotionPlus(true);
	getchar();
	wiiYarpPortsCtrl->closePorts();
	return 0;
}