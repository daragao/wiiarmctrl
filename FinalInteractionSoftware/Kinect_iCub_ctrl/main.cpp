// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
* Copyright (C) 2011 Duarte Aragao
* Author: Duarte Aragao
* CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
*
*/

#include <yarp/os/Network.h>

#include "ModuleCtrl.h"

using namespace std;
using namespace yarp::os;

int main(int argc, char *argv[]){

	Network yarp;
	if(!yarp.checkNetwork()){
		printf("Yarp network failed!\n");
		return -1;
	}

	ModuleCtrl modCtrl;
	modCtrl.runModule(argc,argv);
	//when the module finnishes the program ends
	return 1;//this will never happen
}

