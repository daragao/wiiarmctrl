#pragma once
#include <gtk/gtk.h>
#include <string>
#include <math.h>

#define CTRL_THREAD_PER     0.02   // [s]

#define TWO_PI (2*3.14159265)
#define DEG_TO_RAD (TWO_PI/360)

using namespace std;

struct widgetData {
	GtkWidget *widget;
	char *name;
};

struct guiDataStruct{
	bool isGuiON;
	bool wiiRebootBot;
	bool mpBot;
};

struct wiimoteData{
	bool isConnected;
	bool botA;
	bool botB;
	double accX;
	double accY;
	double accZ;
	double yaw, pitch, roll;
	double motionPlusX;
	double motionPlusY;
	double motionPlusZ;
	double motionPlusAngleX;
	double motionPlusAngleY;
	double motionPlusAngleZ;
	float battery;
	int isMPLowSpeed;
	double temp;
};

struct icubRobotData{
	bool isConnected;
	double oriRightArm[4];
	double posRightArm[4];
};

class Util
{
public:
	static void initUtil();
	static void initWiiData();
	static void initGuiData();
	static void initRobotData();
	static wiimoteData *wiiData;
	static icubRobotData *icubData;
	static guiDataStruct *guiData;
	static double *rotate(double *angleArray, double heading, double attitude, double bank);
};
