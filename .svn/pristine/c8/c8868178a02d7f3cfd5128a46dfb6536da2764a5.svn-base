#pragma once

#include <stdio.h>
#include <stdlib.h>

#ifndef WIN32
	#include <unistd.h>
#endif

#include "wiiuse.h"
#include "WiiDataStruct.h"
#include "MotionPlusUtils.h"

#define MAX_WIIMOTES		2
#define CONNECTION_TIMEOUT	5

#define WIIMOTE_ACCELEROMETER	1
#define WIIMOTE_MOTIONPLUS		2
#define WIIMOTE_IR				4

#define PITCH_LOW_SPEED 0x1
#define YAW_LOW_SPEED	0x2
#define ROLL_LOW_SPEED	0x4
#define LOW_SPEED (PITCH_LOW_SPEED | YAW_LOW_SPEED | ROLL_LOW_SPEED)

class Wiimote
{
public:
	Wiimote(void);
	~Wiimote(void);
	void setBluetoothStack(bool isBluesoleil);
	bool connect(bool connect);
	bool disconnect();
	void poll();
	//turn on wiimote parts
	void turnOnPart(int id, int partEnum, bool partOn);
	//set (speaker not implemented)
	void setLEDs(int id, int binaryLEDs);
	void setRumble(int id, bool rumbleOn);
	WiiDataStruct *getWiiDataStruct(int id);
	int getTotalConnectWiimotes();
private:
	WiiDataStruct **_wiiData;
	MotionPlusUtils **_motionPlusUtils;
	wiimote** _wiimotes;
	int _connectedWiimotes;
	bool _isMotionPlusOn;
	void initVars();
	WiiDataStruct **initWiiDataVars(int totalWiimotes);
	MotionPlusUtils **initMotionPlusUtilsVars(int totalWiimotes);
	MotionPlusUtils *getMotionPlusUtils(int id);
	bool connect(int maxWiimotes, int timeout);
	WIIUSE_EVENT_TYPE eventSwitch(int id, wiimote *wm);
	wiimote_t *getWiimoteT(int id);
	void preparePart(int id, int partEnum);
	void handle_event(int id, struct wiimote_t* wm);
	void handle_ctrl_status(int id, struct wiimote_t* wm);
	void handle_connect(int id, wiimote* wm);
	void handle_disconnect(int id, wiimote* wm);
};
