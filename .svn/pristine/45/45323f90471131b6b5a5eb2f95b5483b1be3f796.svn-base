#include <stdio.h>
#include <yarp/os/all.h>
#include <yarp/dev/all.h>
#include "iCubCtrl.h"
#include "Wiimote.h"
#include "glWindow.h"
#include "ThreadCtrl.h"
#include "SimpleTest.h"

using namespace yarp;
using namespace yarp::os;
using namespace yarp::dev;

YARP_DECLARE_DEVICES(icubmod)

//MAIN
int main(int argc, char *argv[]) {

	YARP_REGISTER_DEVICES(icubmod)

	//devices MUST be started here!
	Network yarp;
	if(!yarp.checkNetwork()){
		printf("\n********************\n* No YARP network! *\n********************\n\n");
		return 1;
	}

	//--------
	printf("\n\n--------- iCubCtrl :D ---------\n\n");
	iCubCtrl *icub = new iCubCtrl();
	
	//--------
	printf("\n\n--------- Wiimote :D ---------\n\n");
	Wiimote *wiimote = new Wiimote();

	SimpleTest *testClass = new SimpleTest();//TESTCLASS

	//--------
	printf("\n\n--------- ThreadCtrl :D ---------\n\n");
	ThreadCtrl *thr = new ThreadCtrl(0.02,wiimote,icub);
	thr->initThread(thr);

	//--------
	printf("\n\n--------- OpenGL :D ---------\n\n");
	//glWindow *glWin = new glWindow(argc, argv);
	testClass->initSimpleTest(argc, argv);//TESTCLASS

	return 0;
}
