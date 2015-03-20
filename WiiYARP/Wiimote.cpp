#include "Wiimote.h"

Wiimote::Wiimote(void)
{
	initVars();
}

Wiimote::~Wiimote(void)
{
}

void Wiimote::initVars(){
	_wiimotes = NULL;
	_connectedWiimotes = 0;
	_isMotionPlusOn = false;
	_wiiData = NULL;
	_motionPlusUtils = NULL;
}

MotionPlusUtils **Wiimote::initMotionPlusUtilsVars(int totalWiimotes){
	MotionPlusUtils **mpUtils = new MotionPlusUtils*[totalWiimotes];
	for(int i = 0; i < totalWiimotes; i++){
		mpUtils[i] = new MotionPlusUtils();
	}
	return mpUtils;
}

WiiDataStruct **Wiimote::initWiiDataVars(int totalWiimotes){
	WiiDataStruct **wiiData = new WiiDataStruct*[totalWiimotes];
	for(int i = 0; i < totalWiimotes; i++){
		wiiData[i] = new WiiDataStruct();
		wiiData[i]->eventNum	= 0;
		wiiData[i]->battery_level	= -1;
		wiiData[i]->botA		= false;
		wiiData[i]->botB		= false;
		wiiData[i]->botUP		= false;
		wiiData[i]->botDOWN		= false;
		wiiData[i]->botLEFT		= false;
		wiiData[i]->botRIGHT	= false;
		wiiData[i]->botHOME		= false;
		wiiData[i]->botMINUS	= false;
		wiiData[i]->botPLUS		= false;
		wiiData[i]->botONE		= false;
		wiiData[i]->botTWO		= false;
		wiiData[i]->isAccelerometerOn	= false;
		wiiData[i]->isMotionPlusOn		= false;
		wiiData[i]->isIROn				= false;
		wiiData[i]->accGForce.resize(3);
		wiiData[i]->accOrientation.resize(3);
		wiiData[i]->irCursor.resize(3);
		wiiData[i]->isIRVisible[0] = false;
		wiiData[i]->isIRVisible[1] = false;
		wiiData[i]->isIRVisible[2] = false;
		wiiData[i]->isIRVisible[3] = false;
		wiiData[i]->irSource[0].resize(3);
		wiiData[i]->irSource[1].resize(3);
		wiiData[i]->irSource[2].resize(3);
		wiiData[i]->irSource[3].resize(3);
		wiiData[i]->isLEDOn[0] = false;
		wiiData[i]->isLEDOn[1] = false;
		wiiData[i]->isLEDOn[2] = false;
		wiiData[i]->isLEDOn[3] = false;
		wiiData[i]->isRumbleOn = false;
		wiiData[i]->motionPlusCalibrationStatus = 0;
		wiiData[i]->motionPlusRaw.resize(3);
		wiiData[i]->motionPlusAngle.resize(3);
	}
	return wiiData;
}

void Wiimote::setBluetoothStack(bool isBluesoleil){
	//WINDOWS ONLY!
	wiimote** wiimotes = _wiimotes;
	if(isBluesoleil)
		wiiuse_set_bluetooth_stack(wiimotes,MAX_WIIMOTES,WIIUSE_STACK_BLUESOLEIL);
	else
		wiiuse_set_bluetooth_stack(wiimotes,MAX_WIIMOTES,WIIUSE_STACK_MS);
}

bool Wiimote::connect(bool tryConnection){
	if(tryConnection)
		return connect(MAX_WIIMOTES,CONNECTION_TIMEOUT);
	else return !disconnect();
}

bool Wiimote::connect(int maxWiimotes, int timeout){
	wiimote** wiimotes;
	int wiimotesFound,connected;

	wiimotes =  wiiuse_init(maxWiimotes);
	if(!wiimotes)
		return false;
	wiimotesFound = wiiuse_find(wiimotes, maxWiimotes, timeout);
	if(!wiimotesFound)
		return false;
	wiiuse_set_timeout(wiimotes, wiimotesFound, (byte)5000, (byte)5000);
	connected = wiiuse_connect(wiimotes, wiimotesFound);
	if(!connected)
		return false;

	_wiiData = initWiiDataVars(connected);
	_motionPlusUtils = initMotionPlusUtilsVars(connected);
	_wiimotes = wiimotes;
	_connectedWiimotes = connected;
	return true;
}

bool Wiimote::disconnect(){
	wiiuse_cleanup(_wiimotes,MAX_WIIMOTES);
	initVars();
	return true;
}

wiimote_t *Wiimote::getWiimoteT(int id){
	if(id>=_connectedWiimotes) return NULL;
	wiimote_t *wm = _wiimotes[id];
	return wm;
}

WiiDataStruct *Wiimote::getWiiDataStruct(int id){
	if(id>=_connectedWiimotes) return NULL;
	WiiDataStruct *wm = _wiiData[id];
	return wm;
}

MotionPlusUtils *Wiimote::getMotionPlusUtils(int id){
	if(id>=_connectedWiimotes) return NULL;
	MotionPlusUtils *mpUtils = _motionPlusUtils[id];
	return mpUtils;
}

void Wiimote::poll(){
	wiimote *wm,**wmArray = _wiimotes;
	int connected = _connectedWiimotes;
	if (wiiuse_poll(wmArray, connected)) {
		for (int id = 0; id < connected; ++id) {
			wm = wmArray[id];
			eventSwitch(id, wm);
			handle_ctrl_status(id,wm);
		}
		//update status
	}
}

WIIUSE_EVENT_TYPE Wiimote::eventSwitch(int id, wiimote *wm){
	WiiDataStruct *wiiData = getWiiDataStruct(id);
	/*NOT IMPLEMENTED
	WIIUSE_NUNCHUK_INSERTED
	WIIUSE_NUNCHUK_REMOVED
	WIIUSE_CLASSIC_CTRL_INSERTED
	WIIUSE_CLASSIC_CTRL_REMOVED
	WIIUSE_GUITAR_HERO_3_CTRL_INSERTED
	WIIUSE_GUITAR_HERO_3_CTRL_REMOVED*/
	switch(wm->event){
			case WIIUSE_NONE:
				//printf("WIIUSE_NONE\n");
				break;
			case WIIUSE_EVENT:
				//printf("WIIUSE_EVENT\n");
				handle_event(id,wm);
				break;
			case WIIUSE_STATUS:
				//printf("WIIUSE_STATUS\n");
				handle_ctrl_status(id,wm);
				break;
			case WIIUSE_CONNECT:
				//printf("WIIUSE_CONNECT\n");
				handle_connect(id,wm);
				break;
			case WIIUSE_DISCONNECT:
			case WIIUSE_UNEXPECTED_DISCONNECT:
				//printf("WIIUSE_DISCONNECT\n");
				handle_disconnect(id,wm);
				break;
			case WIIUSE_READ_DATA:
				//printf("WIIUSE_READ_DATA\n");
				break;
			case WIIUSE_MOTION_PLUS_ACTIVATED:
				//printf("WIIUSE_MOTION_PLUS_ACTIVATED\n");
				_isMotionPlusOn = true;
				wiiData->isMotionPlusOn = true;
				break;
			case WIIUSE_MOTION_PLUS_REMOVED:
				//printf("WIIUSE_MOTION_PLUS_REMOVED\n");
				_isMotionPlusOn = false;
				wiiData->isMotionPlusOn = false;
				break;
	}
	return wm->event;
}

void Wiimote::handle_event(int id, struct wiimote_t* wm){
	WiiDataStruct *wiiData = getWiiDataStruct(id);
	wiiData->eventNum++;
	/* if a button is pressed, report it */
	wiiData->botA		= IS_PRESSED(wm, WIIMOTE_BUTTON_A);
	wiiData->botB		= IS_PRESSED(wm, WIIMOTE_BUTTON_B);
	wiiData->botUP		= IS_PRESSED(wm, WIIMOTE_BUTTON_UP);
	wiiData->botDOWN	= IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN);
	wiiData->botLEFT	= IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT);
	wiiData->botRIGHT	= IS_PRESSED(wm, WIIMOTE_BUTTON_RIGHT);
	wiiData->botMINUS	= IS_PRESSED(wm, WIIMOTE_BUTTON_MINUS);
	wiiData->botPLUS	= IS_PRESSED(wm, WIIMOTE_BUTTON_PLUS);
	wiiData->botONE		= IS_PRESSED(wm, WIIMOTE_BUTTON_ONE);
	wiiData->botTWO		= IS_PRESSED(wm, WIIMOTE_BUTTON_TWO);
	wiiData->botHOME	= IS_PRESSED(wm, WIIMOTE_BUTTON_HOME);


	if (WIIUSE_USING_ACC(wm)) {
		/* accelerometer data */
		wiiData->accOrientation.resize(3);
		wiiData->accOrientation[0] = wm->orient.roll;
		wiiData->accOrientation[1] = wm->orient.pitch;
		wiiData->accOrientation[2] = wm->orient.yaw;
		wiiData->accGForce.resize(3);
		wiiData->accGForce[0] = wm->gforce.x;
		wiiData->accGForce[1] = wm->gforce.y;
		wiiData->accGForce[2] = wm->gforce.z;
	}

	if (WIIUSE_USING_IR(wm)) {
		/* go through each of the 4 possible IR sources */
		for (int i = 0; i < 4; ++i) {
			/* check if the source is visible */
			wiiData->isIRVisible[i] = wm->ir.dot[i].visible?true:false;
			if (wm->ir.dot[i].visible){
				wiiData->irSource[i].resize(3);
				wiiData->irSource[i][0] = (double)wm->ir.dot[i].x;
				wiiData->irSource[i][1] = (double)wm->ir.dot[i].y;
				wiiData->irSource[i][2] = (double)wm->ir.dot[i].size;
			}
		}
		wiiData->irCursor.resize(3);
		wiiData->irCursor[0] = (double)wm->ir.x;
		wiiData->irCursor[1] = (double)wm->ir.y;
		wiiData->irCursor[2] = (double)wm->ir.z;
	}

	if (wm->exp.type == EXP_MOTION_PLUS && _isMotionPlusOn) {
		/* motion plus */
		struct motion_plus_t* mp = (motion_plus_t*)&wm->exp.mp;
		bool roll_low_speed		= (mp->status & ROLL_LOW_SPEED)?true:false;
		bool pitch_low_speed	= (mp->status & PITCH_LOW_SPEED)?true:false;
		bool yaw_low_speed		= (mp->status & YAW_LOW_SPEED)?true:false;
		double mpX = (double)mp->rx;
		double mpY = (double)mp->ry;
		double mpZ = (double)mp->rz;
		wiiData->motionPlusRaw.resize(3);
		wiiData->motionPlusRaw[0] = mpX;
		wiiData->motionPlusRaw[1] = mpY;
		wiiData->motionPlusRaw[2] = mpZ;
		MotionPlusUtils *mpUtils = getMotionPlusUtils(id);
		wiiData->motionPlusCalibrationStatus = mpUtils->calibrateMP(wiiData->motionPlusRaw);
		wiiData->motionPlusAngle = mpUtils->getAngle(roll_low_speed,pitch_low_speed,yaw_low_speed,wiiData->motionPlusRaw);
	}
}

void Wiimote::handle_ctrl_status(int id, struct wiimote_t* wm){
	//printf("\n\n--- CONTROLLER STATUS [wiimote id %i] ---\n", wm->unid);
	WiiDataStruct *wiiData = getWiiDataStruct(id);
	wiiData->isLEDOn[0] = WIIUSE_IS_LED_SET(wm, 1);
	wiiData->isLEDOn[1] = WIIUSE_IS_LED_SET(wm, 2);
	wiiData->isLEDOn[2] = WIIUSE_IS_LED_SET(wm, 3);
	wiiData->isLEDOn[3] = WIIUSE_IS_LED_SET(wm, 4);
	wiiData->battery_level = wm->battery_level;
}

void Wiimote::handle_connect(int id, wiimote* wm){
	//printf("\n--- CONNECTED [wiimote id %i] ---\n", wm->unid);
	WiiDataStruct *wiiData = getWiiDataStruct(id);
	wiiData->isConnected = true;
}

void Wiimote::handle_disconnect(int id, wiimote* wm){
	//printf("\n--- DISCONNECTED [wiimote id %i] ---\n", wm->unid);
	WiiDataStruct *wiiData = getWiiDataStruct(id);
	wiiData->isConnected = false;
}

//Conect parts!!!
//WIIMOTE_ACCELEROMETER WIIMOTE_IR WIIMOTE_MOTIONPLUS
void Wiimote::turnOnPart(int id, int partEnum, bool partOn){
	wiimote_t *wm = getWiimoteT(id);
	WiiDataStruct *wiiData = getWiiDataStruct(id);
	switch (partEnum){
		case WIIMOTE_ACCELEROMETER:
			wiiuse_motion_sensing(wm,partOn?1:0);
			wiiData->isAccelerometerOn = partOn;
			break;
		case WIIMOTE_IR:
			wiiuse_set_ir(wm,partOn?1:0);
			wiiData->isIROn = partOn;
			break;
		case WIIMOTE_MOTIONPLUS:
			MotionPlusUtils *mpUtils = getMotionPlusUtils(id);
			wiiuse_set_motion_plus(wm,partOn?1:0);
			mpUtils->recalibrate();
			wiiData->isMotionPlusOn = partOn;
			break;
	}
	if(partOn)
		preparePart(id,partEnum);
}

void Wiimote::preparePart(int id, int partEnum){
	float accSmoothAlpha = 1;
	int irSensitivity = 5;
	aspect_t irRatio = WIIUSE_ASPECT_16_9;//default for 16:9 is 660X370
	wiimote_t *wm = getWiimoteT(id);
	switch (partEnum){
		case WIIMOTE_ACCELEROMETER:
			wiiuse_set_smooth_alpha(wm,accSmoothAlpha);
			break;
		case WIIMOTE_IR:
			wiiuse_set_ir_sensitivity(wm,irSensitivity);
			wiiuse_set_aspect_ratio(wm, irRatio);
			break;
		case WIIMOTE_MOTIONPLUS:
			break;
	}
}

//SET
/*wii->setLEDs(0,WIIMOTE_LED_1+WIIMOTE_LED_2);*/
void Wiimote::setLEDs(int id, int ledsValues){
	wiiuse_set_leds(getWiimoteT(id),ledsValues);
}

void Wiimote::setRumble(int id, bool rumbleOn){
	wiimote_t *wm = getWiimoteT(id);
	if(rumbleOn)
		wiiuse_rumble(wm,1);
	else wiiuse_rumble(wm,0);
}

int Wiimote::getTotalConnectWiimotes(){
	return _connectedWiimotes;
}