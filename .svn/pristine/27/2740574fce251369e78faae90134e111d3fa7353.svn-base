/*
 *
 * Author: Duarte Aragao
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "Wiimote.h"
#include "WiiThread.h"
#include "WiiPortCtrl.h"
#include <yarp/os/all.h>
#include<stdio.h>

using namespace yarp::os;

WiiPortCtrl** initWiiPorts(int totalWiiPorts, string wiimotePortPrefix){
	WiiPortCtrl **wiiPorts = new WiiPortCtrl*[totalWiiPorts];
	for(int i = 0; i < totalWiiPorts; i++){
		wiiPorts[i] = new WiiPortCtrl(i, wiimotePortPrefix);
	}
	return wiiPorts;
}


int main(int argc, char *argv[]) {

	ResourceFinder rf;
	rf.setVerbose();
	rf.setDefaultContext("../../YarpDeviceModule/wiiarmctrl/WiiYARP");
	rf.setDefaultConfigFile("WiiYARP.ini");
	rf.configure("ICUB_ROOT", argc, argv);

	Time::turboBoost();

	//variable data
	string wiimotePrefix = (string)rf.find("portprefix").asString();//"wiimote";
	double miliperiod = rf.find("threadperiod").asDouble();//0.02;
	//check if YARP Server is ON
	Network yarp;
	if(!yarp.checkNetwork()){
		printf("YARP Network off!\n");
		return 0;
	}
	//connect to the detected wiimotes
	Wiimote *wii = new Wiimote();
	if(!wii->connect(true)){
		printf("WiiError: Couldn't connect to Wiimote\n");
		return 0;
	}
	int totalWiimotes = wii->getTotalConnectWiimotes();
	//open ports for each wiimote
	// /wiimote[NUMBER]/sensor
	// /wiimote[NUMBER]/ext/mp
	// /wiimote[NUMBER]/status
	// /wiimote[NUMBER]/reader
	// /wiimote[NUMBER]/bots
	WiiPortCtrl **wiiPorts = initWiiPorts(wii->getTotalConnectWiimotes(),wiimotePrefix);
	//start a unique thread that wii cycle all the wiimotes
	WiiThread *thread = new WiiThread(wii,wiiPorts,miliperiod);
	thread->start();
	printf("\nPress any key to exit...\n");
	getchar();
	//finishes the program
	thread->stop();
	wii->disconnect();
	for(int i = 0; i < totalWiimotes; i++)
		wiiPorts[i]->closePorts();
    return 0;
}
