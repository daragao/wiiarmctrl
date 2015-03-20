#pragma once

#include <stdio.h>
#include "WiiPortCtrl.h"
#include "GLWindow.h"

int main(int argc, char *argv[]) {
	printf("Press any key to start OpenGL test...\n");
	getchar();
	Network yarp;
	if(!yarp.checkNetwork()){
		printf("YARP Network off!\n");
		return 0;
	}
	GLWindow *glWin = new GLWindow(argc, argv);
	WiiPortCtrl *wiiPortCtrl = new WiiPortCtrl(1,glWin);
	glWin->mainLoop();
	return 0;
}