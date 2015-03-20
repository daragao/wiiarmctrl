#include "Wiimote.h"


//WIIMMOTE//WIIMMOTE//WIIMMOTE//WIIMMOTE//WIIMMOTE
Wiimote::Wiimote()
{
	_isMotionPlusON = false;
	initDevice();
	initThread();
}

Wiimote::~Wiimote(void)
{
	_wiiDriver->close();
	endThread();
}

bool Wiimote::initDevice(){
	bool ok = false;
	bool wiimoteConnected = false;
	_wiiSensor = NULL;
	_wiiDriver = new PolyDriver("WiimoteDevice");
	ok = _wiiDriver->isValid();
	if (!ok) return false;
	ok = _wiiDriver->view(_wiiSensor);
	if(!ok)return false;
	if(ok) Util::wiiData->isConnected = true;
	return true;
}

bool Wiimote::initWiimote(){
	_wiiSensor->initController();
	Util::wiiData->isConnected = true;
	return true;
}

void Wiimote::rebootWiimote(){
	Util::initWiiData();
	_wiiSensor->close();
	Sleep(3);
	_isMotionPlusON = false;
	initWiimote();
}

void Wiimote::initThread(){
	Time::turboBoost();

	_thr=new WiiCtrlThread(this, CTRL_THREAD_PER);
	if (!_thr->start())
	{
		delete _thr;
		return;
	}
}

void Wiimote::endThread(){
	Util::wiiData->isConnected = false;
	_thr->stop();
	delete _thr;
}

void Wiimote::refreshWiiSensor(){
	if(!Util::wiiData->isConnected || _wiiSensor == NULL || !Util::guiData->isGuiON) return;
	Vector out;

	refreshBotStates();

	//poll wiimote
	_wiiSensor->wiiRefresh();
	//refresh motion plus
	if(_isMotionPlusON){
		int mpOffset = 8020;
		//motion plus low speed?
		Util::wiiData->isMPLowSpeed = _wiiSensor->isMotionPlusLowSpeed(0);
		out.resize(3);
		//calibrate wiimote
		calibrateWiimote();
		//refresh accelerometer
		out.resize(6);
		_wiiSensor->readAcc(0,out);
		Util::wiiData->accX = out[0];
		Util::wiiData->accY = out[1];
		Util::wiiData->accZ = out[2];
		//yaw, pitch, roll;
		Util::wiiData->yaw = out[3];
		Util::wiiData->pitch = out[4];
		Util::wiiData->roll = out[5];
		//bots
		Util::wiiData->botA = _wiiSensor->wiiGetBotState(0,IWiimote::A);
		Util::wiiData->botB = _wiiSensor->wiiGetBotState(0,IWiimote::B);
	}
	//battery life
	Util::wiiData->battery = _wiiSensor->getBateryLevel(0);
}

void Wiimote::calibrateWiimote(){
	double horiz_angle = _wiiSensor->getMotionPlusHorizontalAngle(0);
	if(_wiiSensor->calibrateMotionPlus(0)==1){
		Vector out;
		out.resize(3);
		_wiiSensor->readMotionPlusCalibrated(0,out);
		Util::wiiData->motionPlusX = out[0];
		Util::wiiData->motionPlusY = out[1];
		Util::wiiData->motionPlusZ = out[2];
		//printf("readMotionPlusCalibrated:%.4f,%.4f,%.4f\n",out[0],out[1],out[2]);
		double intPart;
		_wiiSensor->readMotionPlusAngle(0,out);
		Util::wiiData->motionPlusAngleX += out[0];
		Util::wiiData->motionPlusAngleX = TWO_PI*modf(Util::wiiData->motionPlusAngleX/TWO_PI,&intPart);//keep angle between 0 and 2PI
		Util::wiiData->motionPlusAngleY += out[1];
		Util::wiiData->motionPlusAngleY = TWO_PI*modf(Util::wiiData->motionPlusAngleY/TWO_PI,&intPart);
		Util::wiiData->motionPlusAngleZ += out[2];
		Util::wiiData->motionPlusAngleZ = TWO_PI*modf(Util::wiiData->motionPlusAngleZ/TWO_PI,&intPart);
		Util::wiiData->temp += horiz_angle;
		//printf("%d:(%0+7.2f,%0+7.2f,%0+7.2f)%0+7.2f\n",_wiiSensor->getSequenceNumber(0),
		//	out[0]/DEG_TO_RAD,out[1]/DEG_TO_RAD,out[2]/DEG_TO_RAD,horiz_angle);
		//printf("%0+7.2f\n(%0+7.2f,%0+7.2f,%0+7.2f)",horiz_angle/DEG_TO_RAD,Util::wiiData->motionPlusX,Util::wiiData->motionPlusY,Util::wiiData->motionPlusZ);
	}
	return;
}

void Wiimote::refreshBotStates(){
	if(Util::guiData->mpBot && !_isMotionPlusON){
		_wiiSensor->wiiSetParts(0,1,_wiiSensor->MOTION_PLUS);
		_wiiSensor->wiiSetParts(0,1,_wiiSensor->MONTION_SENSE);
		_isMotionPlusON = true;
	}else{
		if(!Util::guiData->mpBot && _isMotionPlusON){
			_wiiSensor->wiiSetParts(0,0,_wiiSensor->MONTION_SENSE);
			_wiiSensor->wiiSetParts(0,0,_wiiSensor->MOTION_PLUS);
			_isMotionPlusON = false;
		}
	}
	if(Util::guiData->wiiRebootBot){
		Util::guiData->wiiRebootBot = false;
		rebootWiimote();
	}
}

// WIICTRLTHREAD// WIICTRLTHREAD// WIICTRLTHREAD// WIICTRLTHREAD
WiiCtrlThread::WiiCtrlThread(Wiimote *wiimote, const double period) : RateThread(int(period*1000.0)) {
	_wiimote = wiimote;
}
bool WiiCtrlThread::threadInit(){
	return true;
}
void WiiCtrlThread::beforeStart(){}
void WiiCtrlThread::afterStart(bool s){
	_wiimote->initWiimote();
}
void WiiCtrlThread::run(){
	_wiimote->refreshWiiSensor();
}
void WiiCtrlThread::threadRelease(){}