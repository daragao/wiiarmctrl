#include <stdio.h>
#include <string>
#include <yarp/os/all.h>
#include <yarp/dev/Drivers.h>
#include "iCubCtrl.h"
#include "iCubMotorCtrl.h"
#include "WiiPortCtrl.h"
#include "GraspPortCtrl.h"

using namespace yarp::os;
using namespace yarp::dev;

YARP_DECLARE_DEVICES(icubmod)

int main(int argc, char *argv[]) {

	 YARP_REGISTER_DEVICES(icubmod)

	/*int wiimoteID = 0;
	string icubName = "icubSim";
	string partName = "right_arm";*/

	ResourceFinder rf;
	rf.setVerbose();
	rf.setDefaultContext("../../YarpDeviceModule/SVNwiiarmctrl/iCubWiiMotorCtrl");
	rf.setDefaultConfigFile("iCubWii.ini");
	rf.configure("ICUB_ROOT", argc, argv);

	int wiimoteID = rf.find("wiimote").asInt();
	ConstString icubName=rf.find("robot").asString();
	ConstString partName=rf.find("part").asString();
	double speed = rf.find("speed").asDouble();
	double acceleration = rf.find("acceleration").asDouble();
	double speedmultiplier = rf.find("speedmultiplier").asDouble();
	ConstString controlType = rf.find("controltype").asString();
	double mintrajectorytime = rf.find("mintrajectorytime").asDouble();
	bool isKinematic = false;
	if(controlType.find("kinematic")!=string::npos)
		isKinematic = true;
	else isKinematic = false;
	ConstString graspPortName = rf.find("graspingport").asString();


	Network yarp;
	if(!yarp.checkNetwork()){
		printf("YARP Network off!\n");
		return 0;
	}

	GraspPortCtrl *graspCtrl = new GraspPortCtrl();
	graspCtrl->initPorts(graspPortName.c_str());

	iCubCtrl *icub = new iCubCtrl(icubName.c_str(),partName.c_str());
	if(isKinematic){
		icub->initRobotKinematic();
	}else{
		if(!icub->initRobotMotor()){
			printf("ERROR: bad iCub initialization!\n");
			getchar();
			return 0;
		}
	}

	iCubMotorCtrl *icubMotorCtrl = new iCubMotorCtrl(icub);
	int foundLeftPart=partName.find("left");
	if(foundLeftPart!=string::npos)
		icubMotorCtrl->setAsLeft();
	icubMotorCtrl->setAcceleration(acceleration);
	icubMotorCtrl->setSpeed(speed);
	icubMotorCtrl->setSpeedMultiplier(speedmultiplier);

	iCubIKinCtrl *icubIKinCtrl = new iCubIKinCtrl(icub);
	if(isKinematic){
		icubIKinCtrl->setMinCartTrajTime(mintrajectorytime);
	}

	WiiPortCtrl *wiiPort = new WiiPortCtrl(wiimoteID,graspCtrl,icubMotorCtrl,icubIKinCtrl);
	wiiPort->setMotionPlus(true);
	if(isKinematic){
		wiiPort->setKinCtrlMode();
	}else{
		wiiPort->setMotorCtrlMode();
	}

	printf("\nPress ENTER to quit...\n");
	getchar();

	graspCtrl->closePort();
	wiiPort->closeMotorPorts();
	icub->closeRobot();
	
	return 0;
}
