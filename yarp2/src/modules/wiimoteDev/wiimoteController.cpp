#include "wiimoteController.h"

wiimoteController::wiimoteController()
{
	printf("wiimoteController()\n");
		for(int i = 0; i < MAX_WIIMOTES; i++){
		_motionplus_state = MOTIONPLUS_OFF;
		wiimoteData[i].sequenceNum = 0;
		wiimoteData[i].bot1 = false;
		wiimoteData[i].bot2 = false;
		wiimoteData[i].botA = false;
		wiimoteData[i].botB = false;
		wiimoteData[i].botDown = false;
		wiimoteData[i].botUp = false;
		wiimoteData[i].botLeft = false;
		wiimoteData[i].botRight = false;
		wiimoteData[i].botMinus = false;
		wiimoteData[i].botPlus = false;
		wiimoteData[i].botHome = false;
		wiimoteData[i].connected = false;
		wiimoteData[i].gForce.resize(3);
		wiimoteData[i].accCoord.resize(3);
		wiimoteData[i].accOri.resize(3);
		wiimoteData[i].irCoord.resize(11);
		wiimoteData[i].mpAngle.resize(3);
		wiimoteData[i].mpAngle.zero();
		wiimoteData[i].mpCoord.resize(3);
		wiimoteData[i].mpDiffCoord.resize(3);
		wiimoteData[i].battery_level = -1;
		wiimoteData[i].motionPlusSpeed = -1;
		wiimoteData[i].motionplus_horiz_rotation = 0;
		_initTime = _lastRefreshTime = Time::now();
		_lastMotionPlusTime = -1;
	}
}

wiimoteController::~wiimoteController()
{
	printf("~wiimoteController()\n");
}

void wiimoteController::zeroVariables (){
	for(int i = 0; i < MAX_WIIMOTES; i++){
		_motionplus_state = MOTIONPLUS_OFF;
		wiimoteData[i].sequenceNum = 0;
		wiimoteData[i].bot1 = false;
		wiimoteData[i].bot2 = false;
		wiimoteData[i].botA = false;
		wiimoteData[i].botB = false;
		wiimoteData[i].botDown = false;
		wiimoteData[i].botUp = false;
		wiimoteData[i].botLeft = false;
		wiimoteData[i].botRight = false;
		wiimoteData[i].botMinus = false;
		wiimoteData[i].botPlus = false;
		wiimoteData[i].botHome = false;
		wiimoteData[i].connected = false;
		wiimoteData[i].gForce.resize(3);
		wiimoteData[i].accCoord.resize(3);
		wiimoteData[i].accOri.resize(3);
		wiimoteData[i].irCoord.resize(11);
		wiimoteData[i].mpAngle.resize(3);
		wiimoteData[i].mpAngle.zero();
		wiimoteData[i].mpCoord.resize(3);
		wiimoteData[i].mpDiffCoord.resize(3);
		wiimoteData[i].battery_level = -1;
		wiimoteData[i].motionPlusSpeed = -1;
		wiimoteData[i].motionplus_horiz_rotation = 0;
		_initTime = _lastRefreshTime = Time::now();
		_lastMotionPlusTime = -1;
	}
}

bool wiimoteController::open (yarp::os::Searchable& config)
{
	printf("wiimoteController::open (yarp::os::Searchable& config)\n");
	return true;
}

bool wiimoteController::initController ()
{
	zeroVariables ();
	printf("wiimoteController::initController()\n");
	int searchWiimotes = 4;
	_wiimotes =  wiiuse_init(searchWiimotes);
	_wiimotesFound = wiiuse_find(_wiimotes, searchWiimotes, 5);
	if(!_wiimotesFound){
		printf("wiimoteController::initController(): no wiimotes found!");
		return false;
	}
	int connected = wiiuse_connect(_wiimotes, _wiimotesFound);
	for(int i = 0; i < connected; i++){
		wiimoteData[i].connected = true;
	}
	if (connected){
		printf("wiimoteController::initController(): Connected to %i wiimotes (of %i found).\n", connected, _wiimotesFound);
		//wiiuse_set_flags(_wiimotes[0], WIIUSE_SMOOTHING, NULL);
		wiiuse_set_leds(_wiimotes[0], WIIMOTE_LED_1);
		wiiuse_rumble(_wiimotes[0], 1);
#ifndef WIN32
		usleep(200000);
#else
		Sleep(200);
#endif
		wiiuse_rumble(_wiimotes[0], 0);
	}
	else {
		printf("wiimoteController::initController(): Failed to connect to any wiimote.\n");
		return 0;
	}
	return true;
}

bool wiimoteController::close ()
{
	printf("wiimoteController::close(): close ()\n");
	wiiuse_cleanup(_wiimotes,_wiimotesFound);
	return true;
}

bool wiimoteController::wiiGetBotState(int wiimoteNum, WIIBOTS_ENUM bot){
	switch (bot) {
					case A:
						return wiimoteData[wiimoteNum].botA;
						break;
					case B:
						return wiimoteData[wiimoteNum].botB;
						break;
					case HOME:
						return wiimoteData[wiimoteNum].botHome;
						break;
					case ONE:
						return wiimoteData[wiimoteNum].bot1;
						break;
					case TWO:
						return wiimoteData[wiimoteNum].bot2;
						break;
					case UP:
						return wiimoteData[wiimoteNum].botUp;
						break;
					case DOWN:
						return wiimoteData[wiimoteNum].botDown;
						break;
					case RIGHT:
						return wiimoteData[wiimoteNum].botRight;
						break;
					case LEFT:
						return wiimoteData[wiimoteNum].botLeft;
						break;
					case PLUS:
						return wiimoteData[wiimoteNum].botPlus;
						break;
					case MINUS:
						return wiimoteData[wiimoteNum].botMinus;
						break;
	}
	return false;
}

bool wiimoteController::readAcc (int wiimoteNum, yarp::sig::Vector &out)
{
	out.clear();
	out.resize(6);
	Vector accCoord = wiimoteData[wiimoteNum].accCoord;
	Vector accOri = wiimoteData[wiimoteNum].accOri;
	//accelerometer
	if(accCoord.size()!=3)return false;
	out[0] = accCoord[0];
	out[1] = accCoord[1];
	out[2] = accCoord[2];
	//orientation
	if(accOri.size()!=3)return false;
	out[3] = accOri[0];
	out[4] = accOri[1];
	out[5] = accOri[2];
	return true;
}

bool wiimoteController::readGForce (int wiimoteNum, yarp::sig::Vector &out)
{
	//out.clear();
	Vector gforceCoord = wiimoteData[wiimoteNum].gForce;
	if(gforceCoord.size()!=3)return false;
	for(int i = 0; gforceCoord.size() > i; i++){
		out[i] = gforceCoord[i];
	}
	return true;
}

bool wiimoteController::readMotionPlus (int wiimoteNum, yarp::sig::Vector &out)
{
	//out.clear();
	Vector mpCoord = wiimoteData[wiimoteNum].mpCoord;
	if(mpCoord.size()!=3)return false;
	out.resize(3);
	for(int i = 0; mpCoord.size() > i; i++){
		out[i] = mpCoord[i];
	}
	return true;
}

bool wiimoteController::readMotionPlusCalibrated (int wiimoteNum, yarp::sig::Vector &out)
{
	if( _motionplus_state == MOTIONPLUS_OK){
		Vector mpCoord = wiimoteData[wiimoteNum].mpCoord;
		out.resize(3);
		if(mpCoord.size()!=3)return false;
		for(int i = 0; mpCoord.size() > i; i++){
			out[i] = mpCoord[i] - _motionplus_calibration_data[wiimoteNum][i][0];
		}
		//printf("mpcoord:%.4f,%.4f,%.4f\n",out[0],out[1],out[2]);
		return true;
	} else return false;
}

bool wiimoteController::readMotionPlusAngle (int wiimoteNum, yarp::sig::Vector &out)
{
	//out.clear();
	Vector mpAngle = wiimoteData[wiimoteNum].mpAngle;
	if(mpAngle.size()!=3)return false;
	out.resize(3);
	for(int i = 0; mpAngle.size() > i; i++){
		out[i] = mpAngle[i];
	}
	//printf("mpangle:%.4f,%.4f,%.4f\n",out[0],out[1],out[2]);
	return true;
}

bool wiimoteController::readDiffMotionPlus (int wiimoteNum, yarp::sig::Vector &out)
{
	//out.clear();
	Vector mpDiffCoord = wiimoteData[wiimoteNum].mpDiffCoord;
	if(mpDiffCoord.size()!=3)return false;
	for(int i = 0; mpDiffCoord.size() > i; i++){
		out[i] = mpDiffCoord[i];
	}
	return true;
}

bool wiimoteController::readIR (int wiimoteNum, yarp::sig::Vector &out){
	out.clear();
	Vector irCoord = wiimoteData[wiimoteNum].irCoord;
	if(irCoord.size()<3)return false;
	out.resize(irCoord.size());
	for(int i = 0; irCoord.size() > i; i++){
		out[i] = irCoord[i];
	}
	return true;
}

bool wiimoteController::wiiSetRumble(int wiimoteNum, bool state){
	wiiuse_rumble(_wiimotes[wiimoteNum],state?1:0);
	return state;
}

bool wiimoteController::wiiSetLed(int wiimoteNum, int led){
	wiiuse_set_leds(_wiimotes[wiimoteNum],led);
	return true;
}

long wiimoteController::getSequenceNumber(int wiimoteNum){
	return wiimoteData[wiimoteNum].sequenceNum;
}

bool wiimoteController::wiiSetParts(int wiimoteNum, int status){
	wiiSetParts(wiimoteNum,status,MOTION_PLUS);
	wiiSetParts(wiimoteNum,status,MONTION_SENSE);
	wiiSetParts(wiimoteNum,status,INFRA_RED);
	return true;
}

bool wiimoteController::wiiSetParts(int wiimoteNum, int status, PARTS_ENUM part){
	wiimote *wm = _wiimotes[wiimoteNum];
	switch(part){
		case MOTION_PLUS:
			wiiuse_set_motion_plus(wm, status);
			if(status){
				_motionplus_state = MOTIONPLUS_NOT_CALIBRATED;
			}else _motionplus_state = MOTIONPLUS_OFF;
			break;
		case MONTION_SENSE:
			wiiuse_motion_sensing(wm, status);
			wiiuse_set_smooth_alpha(wm,1);
			break;
		case INFRA_RED:
			wiiuse_set_ir(wm, status);
			//wiiuse_set_ir_position(wm, WIIUSE_IR_BELOW);
			wiiuse_set_ir_sensitivity(wm,5);
			wiiuse_set_aspect_ratio(wm, WIIUSE_ASPECT_16_9);//default for 16:9 is 660X370
			break;
	}
	return true;
}


bool wiimoteController::wiiRefresh(){
	double currentTime = Time::now();
	if (wiiuse_poll(_wiimotes, _wiimotesFound)) {
		/*
		*	This happens if something happened on any wiimote.
		*	So go through each one and check if anything happened.
		*/
		int i = 0;
		for (; i < _wiimotesFound; ++i) {
			wiimoteData[i].sequenceNum++;//poll sequence number
			switch (_wiimotes[i]->event) {
					case WIIUSE_EVENT:
						/* a generic event occured */
						handle_event(i);
						break;

					case WIIUSE_STATUS:
						/* a status event occured */
						//handle_ctrl_status(_wiimotes[i]);
						break;

					case WIIUSE_DISCONNECT:
					case WIIUSE_UNEXPECTED_DISCONNECT:
						/* the wiimote disconnected */
						//handle_disconnect(_wiimotes[i]);
						break;

					case WIIUSE_READ_DATA:
						/*
						*	Data we requested to read was returned.
						*	Take a look at wiimotes[i]->read_req
						*	for the data.
						*/
						break;

					case WIIUSE_NUNCHUK_INSERTED:
						/*
						*	a nunchuk was inserted
						*	This is a good place to set any nunchuk specific
						*	threshold values.  By default they are the same
						*	as the wiimote.
						*/
						//wiiuse_set_nunchuk_orient_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 90.0f);
						//wiiuse_set_nunchuk_accel_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 100);
						printf("Nunchuk inserted.\n");
						break;
					case WIIUSE_NUNCHUK_REMOVED:
						/* some expansion was removed */
						//handle_ctrl_status(wiimotes[i]);
						printf("An expansion was removed.\n");
						break;
					case WIIUSE_MOTION_PLUS_ACTIVATED:
						printf("MotionPlus inserted.\n");
						break;
					case WIIUSE_MOTION_PLUS_REMOVED:
						//handle_ctrl_status(wiimotes[i]);
						printf("MotionPlus was removed.\n");
						break;

					default:
						break;
			}
		}
	}else return false;
	_lastRefreshTime = currentTime;
	return true;
}

void wiimoteController::handle_event(int wiimoteNum){
	wiimote *wm = _wiimotes[wiimoteNum];
	wiiData *wiiBot = &wiimoteData[wiimoteNum];
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_A)){
		wiiBot->botA = true;
	} else wiiBot->botA = false;
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_B)){
		wiiBot->botB = true;
	} else wiiBot->botB = false;
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_UP))
		wiiBot->botUp = true;
	else wiiBot->botUp = false;
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN))
		wiiBot->botDown = true;
	else wiiBot->botDown = false;
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT))
		wiiBot->botLeft = true;
	else wiiBot->botLeft = false;
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_RIGHT))
		wiiBot->botRight = true;
	else wiiBot->botRight = false;
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_MINUS))
		wiiBot->botMinus = true;
	else wiiBot->botMinus = false;
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_PLUS))
		wiiBot->botPlus = true;
	else wiiBot->botPlus = false;
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_ONE))
		wiiBot->bot1 = true;
	else wiiBot->bot1 = false;
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_TWO))
		wiiBot->bot2 = true;
	else wiiBot->bot2 = false;
	if (IS_PRESSED(wm, WIIMOTE_BUTTON_HOME))
		wiiBot->botHome = true;
	else wiiBot->botHome = false;

	//position and orientation
	if (WIIUSE_USING_ACC(wm)) {
		//gforce
		wiiBot->gForce[0]=wm->gforce.x;
		wiiBot->gForce[1]=wm->gforce.y;
		wiiBot->gForce[2]=wm->gforce.z;
		//accelerometer
		wiiBot->accCoord[0]=wm->accel.x;
		wiiBot->accCoord[1]=wm->accel.y;
		wiiBot->accCoord[2]=wm->accel.z;
		//orientation
		wiiBot->accOri[0]=wm->orient.pitch;
		wiiBot->accOri[1]=wm->orient.roll;
		wiiBot->accOri[2]=wm->orient.yaw;
	}
	//IR camera
	if (WIIUSE_USING_IR(wm)) {
		for (int i = 0; i < 4; ++i) {
			if (wm->ir.dot[i].visible){
				wiiBot->irCoord[0]=wm->ir.x;
				wiiBot->irCoord[1]=wm->ir.y;
				wiiBot->irCoord[2]=wm->ir.z;
				break;
			}
		}
		bool foundIRPoint = false;
		for (int i = 0; i < 4; ++i) {
			if (wm->ir.dot[i].visible){
				foundIRPoint = true;
				wiiBot->irCoord[3+(i*2)+0]=wm->ir.dot[i].x;
				wiiBot->irCoord[3+(i*2)+1]=wm->ir.dot[i].y;
			}else{
				wiiBot->irCoord[3+(i*2)+0]=-1;
				wiiBot->irCoord[3+(i*2)+1]=-1;
			}
		}
	}
	//MotionPlus
	if (wm->exp.type == EXP_MOTION_PLUS) {
		/* motion plus */
		struct motion_plus_t* mp = (motion_plus_t*)&wm->exp.mp;
		double rx = mp->rx;
		double ry = mp->ry;
		double rz = mp->rz;

		wiiBot->mpDiffCoord[0]=wiiBot->mpCoord[0]-rx;
		wiiBot->mpDiffCoord[1]=wiiBot->mpCoord[1]-ry;
		wiiBot->mpDiffCoord[2]=wiiBot->mpCoord[2]-rz;

		wiiBot->mpCoord[0]=rx;
		wiiBot->mpCoord[1]=ry;
		wiiBot->mpCoord[2]=rz;

		if((wm->exp.mp.status & LOW_SPEED) == LOW_SPEED){
			wiiBot->motionPlusSpeed = 1;
		}else {
			wiiBot->motionPlusSpeed = 0;
		}
		if(wm->exp.mp.status & PITCH_LOW_SPEED) wiiBot->motionPlusLowSpeedPitch = true;
		else wiiBot->motionPlusLowSpeedPitch = false;
		if(wm->exp.mp.status & ROLL_LOW_SPEED) wiiBot->motionPlusLowSpeedRoll = true;
		else wiiBot->motionPlusLowSpeedRoll = false;
		if(wm->exp.mp.status & YAW_LOW_SPEED) wiiBot->motionPlusLowSpeedYaw = true;
		else wiiBot->motionPlusLowSpeedYaw = false;
		calculateMotionPlusAngle(wiimoteNum);
	}else{
		/*wiiBot->mpDiffCoord[0]=0;
		wiiBot->mpDiffCoord[1]=0;
		wiiBot->mpDiffCoord[2]=0;
		wiiBot->mpCoord[0]=0;
		wiiBot->mpCoord[1]=0;
		wiiBot->mpCoord[2]=0;*/
		wiiBot->motionPlusSpeed = -1;
	}

	wiiBot->battery_level = wm->battery_level;
}

float wiimoteController::getBateryLevel(int wiimoteNum){
	return wiimoteData[wiimoteNum].battery_level;
}

int wiimoteController::isMotionPlusLowSpeed(int wiimoteNum){
	//low speed 0, high speed 1, bad value -1
	return wiimoteData[wiimoteNum].motionPlusSpeed;
}

double wiimoteController::getMotionPlusHorizontalAngle(int wiimoteNum){
	return wiimoteData[wiimoteNum].motionplus_horiz_rotation;
}

void wiimoteController::calculateMotionPlusAngle(int wiimoteNum){
	double currentTime = Time::now();
	if(_lastMotionPlusTime==-1){
		_lastMotionPlusTime = currentTime;
		return;
	}
	double rate = currentTime - _lastMotionPlusTime;

	Vector mpCoord = wiimoteData[wiimoteNum].mpCoord;
	Vector avgMPCoord;
	avgMPCoord.resize(3);
	avgMPCoord[0] = (mpCoord[0] - _motionplus_calibration_data[wiimoteNum][0][0]);
	avgMPCoord[1] = (mpCoord[1] - _motionplus_calibration_data[wiimoteNum][1][0]);
	avgMPCoord[2] = (mpCoord[2] - _motionplus_calibration_data[wiimoteNum][2][0]);
	Vector gForce = wiimoteData[wiimoteNum].gForce;

	double horiz_rotation = 0;
	double horiz_rotation_x = avgMPCoord[0] * gForce[0] * rate;
	double horiz_rotation_y = avgMPCoord[1] * gForce[1] * rate;
	double horiz_rotation_z = avgMPCoord[2] * gForce[2] * rate;

	double fast = 20;
	double slow = 5;

	if(wiimoteData[wiimoteNum].motionPlusLowSpeedPitch){
		horiz_rotation	=	-horiz_rotation_x / fast;
	}else{
		horiz_rotation	=	-horiz_rotation_x / slow;
	}
	if(wiimoteData[wiimoteNum].motionPlusLowSpeedRoll){
		horiz_rotation	-=	horiz_rotation_y / fast;
	}else{
		horiz_rotation	-=	horiz_rotation_y / slow;
	}
	if(wiimoteData[wiimoteNum].motionPlusLowSpeedYaw){
		horiz_rotation	-=	horiz_rotation_z / fast;
	}else{
		horiz_rotation	-=	horiz_rotation_z / slow;
	}

	horiz_rotation *= DEG_TO_RAD;
	wiimoteData[wiimoteNum].motionplus_horiz_rotation = horiz_rotation;

	Vector gravity;
	static double old_cos_roll, old_cos_pitch, old_z;
	double g = getNormalGravity(wiimoteNum,gravity);
	double cos_roll =	sqrt(1-pow(gravity[0],2));
	double cos_pitch =	sqrt(1-pow(gravity[1],2));
	double z =			sqrt(1-pow(gravity[2],2));

	Vector mpAngleV;
	mpAngleV.resize(3);

	fast = 20;
	slow = 4.4;

	if(cos_roll > VERY_LITTLE){
		if(wiimoteData[wiimoteNum].motionPlusLowSpeedPitch){
			mpAngleV[0] = (avgMPCoord[0] * rate / fast) * DEG_TO_RAD;
		}else{
			mpAngleV[0] = (avgMPCoord[0] * rate / slow) * DEG_TO_RAD;
		}
	}else mpAngleV[0] = 0;
	if(cos_pitch > VERY_LITTLE){
		if(wiimoteData[wiimoteNum].motionPlusLowSpeedRoll){
			mpAngleV[1] = (avgMPCoord[1] * rate / fast) * DEG_TO_RAD;
		}else{
			mpAngleV[1] = (avgMPCoord[1] * rate / slow) * DEG_TO_RAD;
		}
	}else mpAngleV[1] = 0;
	if(z > VERY_LITTLE){
		if(wiimoteData[wiimoteNum].motionPlusLowSpeedYaw){
			mpAngleV[2] = (avgMPCoord[2] * rate / fast) * DEG_TO_RAD;
		}else{
			mpAngleV[2] = (avgMPCoord[2] * rate / slow) * DEG_TO_RAD;
		}
	}else mpAngleV[2] = 0;

	wiimoteData[wiimoteNum].mpAngle[0] = mpAngleV[0];
	wiimoteData[wiimoteNum].mpAngle[1] = mpAngleV[1];
	wiimoteData[wiimoteNum].mpAngle[2] = mpAngleV[2];

	//Implement simple low pass filter
/*
	double percent = 0.15;
	wiimoteData[wiimoteNum].mpAngle[0] = (wiimoteData[wiimoteNum].mpAngle[0] * percent) + (mpAngleV[0] * (1-percent)) ;
	wiimoteData[wiimoteNum].mpAngle[1] = (wiimoteData[wiimoteNum].mpAngle[1] * percent) + (mpAngleV[1] * (1-percent)) ;
	wiimoteData[wiimoteNum].mpAngle[2] = (wiimoteData[wiimoteNum].mpAngle[2] * percent) + (mpAngleV[2] * (1-percent)) ;
*/
	_lastMotionPlusTime = currentTime;
	old_cos_roll = cos_roll;
	old_cos_pitch = cos_pitch;
	old_z = z;
}

double wiimoteController::calculateAngleFromAdjancent(bool isPlus, double angle, double adjGrav, double newGrav, double adjGravNorm, double newGravNorm){
	double result = 0;
	double angleAdd = acos(- (adjGrav * newGrav + 1) / (adjGravNorm * newGravNorm));
	//only true if it is infinite
	if(angleAdd!=angleAdd) angleAdd = 0;
	if(isPlus){
		result = angle - angleAdd;
	}else{
		result = angle + angleAdd;
	}
	//if( _motionplus_state == MOTIONPLUS_OK)
		//printf("\n:%f(+/-)%0+7.2f/%0+7.2f->%0+7.2f:",angle,(adjGrav * newGrav + 1),(adjGravNorm * newGravNorm),result);
	return result;
}

double wiimoteController::getNormalGravity(int wiimoteNum, Vector &gravity){
	Vector gforce;
	gforce = wiimoteData[wiimoteNum].gForce;
	/*gforce[0] -=  _gforce_calibration_data[wiimoteNum][0][0];
	gforce[1] -=  _gforce_calibration_data[wiimoteNum][1][0];
	gforce[2] -=  _gforce_calibration_data[wiimoteNum][2][0];*/
	double g = sqrt(pow(gforce[0],2) + pow(gforce[1],2) + pow(gforce[2],2));
	gravity.resize(3);
	gravity[0] = gforce[0] / g;
	gravity[1] = gforce[1] / g;
	gravity[2] = gforce[2] / g;
	return g;
}

float wiimoteController::calibrateMotionPlus(int wiimoteNum){
	if(_motionplus_state == MOTIONPLUS_OFF) return 0;
	if(_motionplus_state == MOTIONPLUS_OK) return 1;
	if(_motionplus_state > MOTIONPLUS_OK) return -1;
	bool failed = false;
	//printf("%d;",_motionplus_state);
	Vector mpRates = wiimoteData[wiimoteNum].mpCoord;
	Vector gForce = wiimoteData[wiimoteNum].gForce;
	float isEnd = 0;
	double minG = 0.95;
	double maxG = 1.05;
	double maxYCalib = 0.08;
	int lowSpeedType = isMotionPlusLowSpeed(wiimoteNum);
	Vector gravity;
	double g = getNormalGravity(wiimoteNum,gravity);
	int dataIndex = _motionplus_state - MOTIONPLUS_CALIBRATING;
	if(g < maxG && g > minG && abs(gravity[0]) < maxYCalib){
		if(_motionplus_state == MOTIONPLUS_NOT_CALIBRATED){
			//wm+ calibration
			_motionplus_calibration_data[wiimoteNum][0][0] = mpRates[0];
			_motionplus_calibration_data[wiimoteNum][1][0] = mpRates[1];
			_motionplus_calibration_data[wiimoteNum][2][0] = mpRates[2];
			//gforce calibration
			_gforce_calibration_data[wiimoteNum][0][0] = gForce[0];
			_gforce_calibration_data[wiimoteNum][1][0] = gForce[1];
			_gforce_calibration_data[wiimoteNum][2][0] = gForce[2];
			_motionplus_state = MOTIONPLUS_CALIBRATING;
			return isEnd;
		}else if(_motionplus_state >= MOTIONPLUS_CALIBRATING &&
			(abs(_motionplus_calibration_data[wiimoteNum][0][dataIndex] - mpRates[0]) < MAX_CALIBRATION_DIFF) &&
			(abs(_motionplus_calibration_data[wiimoteNum][1][dataIndex] - mpRates[1]) < MAX_CALIBRATION_DIFF) &&
			(abs(_motionplus_calibration_data[wiimoteNum][2][dataIndex] - mpRates[2]) < MAX_CALIBRATION_DIFF)){
				///GET MAX_CALIBRATION_DATA OF SAMPLES A DO THE MEAN VALUE
				_motionplus_state++;//add up until reaches MOTIONPLUS_OK
				dataIndex = _motionplus_state - MOTIONPLUS_CALIBRATING;
				_motionplus_calibration_data[wiimoteNum][0][dataIndex] = mpRates[0];
				_motionplus_calibration_data[wiimoteNum][1][dataIndex] = mpRates[1];
				_motionplus_calibration_data[wiimoteNum][2][dataIndex] = mpRates[2];
				//gforce calibration (using wm+ conditions)
				_gforce_calibration_data[wiimoteNum][0][dataIndex] = gForce[0];
				_gforce_calibration_data[wiimoteNum][1][dataIndex] = gForce[1];
				_gforce_calibration_data[wiimoteNum][2][dataIndex] = gForce[2];

				if(_motionplus_state == MOTIONPLUS_OK){
					//calibration complete!!!
					for(int i = 0; i < 3; i++){
						double sum = 0, gforceSum = 0;
						for(int j = 0; j < MAX_CALIBRATION_DATA; j++){
							sum += _motionplus_calibration_data[wiimoteNum][i][j];
							gforceSum += _gforce_calibration_data[wiimoteNum][i][j];
						}
						_motionplus_calibration_data[wiimoteNum][i][0] = sum / MAX_CALIBRATION_DATA;
						_gforce_calibration_data[wiimoteNum][i][0] = gforceSum / MAX_CALIBRATION_DATA;
					}
					isEnd = 1;
				}
		}else failed = true;
	}else failed = true;
	if(failed) {
		failed = false;
		_motionplus_state = MOTIONPLUS_NOT_CALIBRATED;
		//printf("reset!%d\n",_motionplus_state);
	}
	return ((float)_motionplus_state)/MOTIONPLUS_OK;
}
