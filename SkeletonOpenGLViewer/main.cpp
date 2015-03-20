#include <stdio.h>

#include <yarp/os/Network.h>
#include <yarp/os/ResourceFinder.h>

#include "PortCtrlMod.h"

using namespace yarp::os;


int main(int argc, char *argv[]){

	Network yarp;
	if(!yarp.checkNetwork()){
		printf("Yarp network failed!\n");
		return -1;
	}

	PortCtrlMod portMod;

	ResourceFinder rf;
	rf.setVerbose();
	rf.setDefaultContext("../../SkeletonOpenGLViewer");
	rf.setDefaultConfigFile("SkeletonOpenGLViewer.ini");
	rf.configure("ICUB_ROOT", argc, argv);

	if(!portMod.configure(rf)){
		printf("ERROR: Bad configuration of the RFModule.");
		return -1;
	}

	//prepare glWindow
	portMod.prepapreGLWindow(argc,argv);
	//portMod.runModule();

	return 0;

}