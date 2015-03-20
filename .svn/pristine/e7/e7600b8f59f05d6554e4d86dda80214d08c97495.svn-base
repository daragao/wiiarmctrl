#include <stdio.h>
#include "wiimoteController.h"

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;

int main(int argc, char *argv[]) {
	DriverCreator *fake_factory = 
		new DriverCreatorOf<wiimoteController>("wiimoteControllerName","wiimote","wiimoteController");
	Drivers::factory().add(fake_factory); // hand factory over to YARP


	PolyDriver dd("wiimoteControllerName");
	if (!dd.isValid()) {
		printf("wiimoteControllerName not available\n"); 
		return 1;
	}
	IWiimote *genericSensor;
	dd.view(genericSensor);
	if (genericSensor==NULL) {
		printf("*** Device failed to connect\n");
		return 1;
	}

	//WIIMOTE
	Vector out;
	double *outData;
	bool isMP_ON = false;

	genericSensor->initController();

	while(1){
		if(genericSensor->wiiRefresh()){
			if(genericSensor->wiiGetBotState(0,IWiimote::HOME)){
				genericSensor->close();
				Sleep(3*1000);
				genericSensor->initController();
			}
			if(genericSensor->wiiGetBotState(0,IWiimote::PLUS)){
				genericSensor->wiiSetParts(0,1,IWiimote::MOTION_PLUS);
				isMP_ON = true;
			}
			if(genericSensor->wiiGetBotState(0,IWiimote::MINUS)){
				genericSensor->wiiSetParts(0,0,IWiimote::MOTION_PLUS);
				isMP_ON = false;
			}
			// PRINT MESSAGES
			printf("-------- wiiRefresh - %d ------\n",genericSensor->getSequenceNumber(0));
			if(isMP_ON){
				genericSensor->readMotionPlus(0,out);
				outData = out.data();
				printf("MOTIONPLUS(%.0f,%.0f,%.0f);\n",outData[0],outData[1],outData[2]);
			}
		}
	}
	return 0;
}