#include <stdio.h>
#include "GuiClass.h"
#include "Wiimote.h"
#include "iCubCtrl.h"

YARP_DECLARE_DEVICES(icubmod)

using namespace yarp;
using namespace yarp::os;
using namespace yarp::dev;

//MAIN
int main(int argc, char *argv[]) {

	YARP_REGISTER_DEVICES(icubmod)

	//devices MUST be started here!
	Network yarp;
	if(!yarp.checkNetwork()){
		printf("\n********************\n* No YARP network! *\n********************\n\n");
		//return false;
	}

	//---- UTIL INIT
	Util::initUtil();

	//--------
	printf("\n\n--------- iCubCtrl :D ---------\n\n");
	iCubCtrl *icub = new iCubCtrl();

	//--------
	printf("\n\n--------- Wiimote :D ---------\n\n");
	Wiimote *wiimote = new Wiimote();

	//--------
	printf("\n\n--------- GTK Gui :D ---------\n\n");
	GuiClass *gui = new GuiClass("../gui/interfaceTestGui.glade");


	return 0;
}
