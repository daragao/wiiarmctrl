#pragma once

#include <iostream>

#include <yarp/os/Module.h>
#include <yarp/os/Searchable.h>
#include <yarp/math/Math.h>

#include <string>
#include <math.h>

#include "iCubCtrl.h"
#include "KinectCtrl.h"

using namespace std;

using namespace yarp::os;
using namespace yarp::math;

static const double PI = 4.0*atan(1.0);

class ModuleCtrl :
	public Module
{
public:
	ModuleCtrl(void);
	~ModuleCtrl(void);
protected:
	virtual double 	getPeriod ();
	virtual bool 	updateModule ();
	virtual bool 	open (Searchable &config);
	virtual bool 	respond (const Bottle &command, Bottle &reply);
	virtual bool 	close ();
private:
	iCubCtrl *_icubCtrl;
	KinectCtrl *_kinectCtrl;
	bool _testON;
	void initVars();
	//Kinect data to iCub movments
	bool kinect2iCub(int userID);
	Vector matrix2Angles(Matrix matrixParent, Matrix matrixChild);
	Vector matrix2Angles(Matrix matrix);
	Vector matrix2AnglesXYZ(Matrix matrix);
	Vector matrix2AnglesYZX(Matrix rMatrix);
	Vector matrix2AnglesZXY(Matrix rMatrix);
	Vector matrix2AnglesZYX(Matrix rMatrix);
	Vector matrix2AnglesXZY(Matrix rMatrix);
	//UTILS
	void printAngles(Vector anglesV);//DEBUG
	void printMatrix(Matrix matrix);//DEBUG
	bool doTest();//DEBUG
};
