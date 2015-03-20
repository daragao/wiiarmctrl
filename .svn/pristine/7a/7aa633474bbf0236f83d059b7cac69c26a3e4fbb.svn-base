#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>	// OpenGL Graphics Utility Library
#include <math.h>
#include <string>
#include <sstream>
#include <time.h>
#include "Utils.h"
#include <queue>
//const double PI = 3.14159265;
const double MAX_R = 1;

using namespace std;


class SimpleTest
{
public:
	SimpleTest();
	~SimpleTest(void);
	int initSimpleTest( int argc, char** argv );
	static void addThreadData(double timeDiff, bool aBot, bool bBot, double calibration, double mp[3], double gForce[3], double acc[3]);//(bool aBot, bool bBot, double calibration, double rx, double ry, double rz, double accX, double accY, double accZ, double gForce[3]);
private:
	static queue<double> *queue1,*queue2;
	static int win1,win2;
	static bool isStarted;
	static int CurrentMode;
	static int NumModes;
	static double Xmin, Xmax;
	static double Ymin, Ymax;
	static double ratioWin2;
	static double r, theta, phi;
	static double forceVector[4];
	static double gForce[3],acc[3],mp[3];
	static double calibStatus;
	static void drawVector(double x, double y, double z, double r, double g, double b);
	static void drawAxes(void);
	static void renderString(float x, float y, void *font, const char* str, float r, float g, float b);
	static void myKeyboardFunc( unsigned char key, int x, int y );
	static void drawScene(void);
	static void drawScene2(void);
	void initRendering();
	static void resizeWindow(int w, int h);
	static void resizeWindow2(int w, int h);
	static double acc2Size(double timeDiff, double accX, double accY, double accZ, double gyro[3]);
	static void printStatus(double x,double y,double z);
	static void sphericalCoords2Vector(double vector[3], double r, double theta, double phi);
	static void drawSize(double size, double maxSize, double r, double g, double b);
	static void drawMotionSenseBars(string title, double maxSize, double motionData[3]);
	static void plotLine(double totalPoints, queue<double> *points, double minValue, double maxValue, double r, double g, double b);
};
