#include "ThreadCtrl.h"

ThreadCtrl::ThreadCtrl(const double period, Wiimote *wiimote, iCubCtrl *icub):RateThread(int(period*1000.0)){
	_wiimote = wiimote;
	_icub = icub;
}

ThreadCtrl::~ThreadCtrl(void)
{
}

bool ThreadCtrl::threadInit(){
	printf("threadInit\n");
	return true;
}
void ThreadCtrl::beforeStart(){
	printf("beforeStart\n");
	_icub->init_iCubCtrl();
	_wiimote->initDevice();
}
void ThreadCtrl::afterStart(bool s){
	printf("afterStart\n");
	_wiimote->initWiimote();
}
void ThreadCtrl::run(){
	static double lastTime = Time::now();
	double timeDiff = Time::now() - lastTime;
	static long lastSequenceNum = -1;
	//refresh :D
	_wiimote->refreshWiiSensor();
	_icub->refresh_iCub();
	//check if anything happened to the wiimote
	if(lastSequenceNum == _wiimote->_sequenceNum)
		return;
	else lastSequenceNum = _wiimote->_sequenceNum;
	//do cool stuff
	double icubCords[3] = {_icub->_icubCoords[0],_icub->_icubCoords[1],_icub->_icubCoords[2]};
	double mpPointCoords[3] = {0,-1,0};
	double mpPointNormal[3] = {0,0,1};
	if(_wiimote->_mpPointCoords.data()!=NULL){
		mpPointCoords[0] = _wiimote->_mpPointCoords[0];
		mpPointCoords[1] = _wiimote->_mpPointCoords[1];
		mpPointCoords[2] = _wiimote->_mpPointCoords[2];
		mpPointNormal[0] = _wiimote->_mpPointCoords[3];
		mpPointNormal[1] = _wiimote->_mpPointCoords[4];
		mpPointNormal[2] = _wiimote->_mpPointCoords[5];
	}
	static bool reset=true;
	SimpleTest::addThreadData(timeDiff,_wiimote->_botA,_wiimote->_botB,Wiimote::_calibStatus,Wiimote::_mpAngleRate.data(),Wiimote::_gForce.data(),Wiimote::_acc.data());//TESTCLASS
	//SimpleTest::addThreadData(_wiimote->_botA,_wiimote->_botB,Wiimote::_calibStatus,Wiimote::_mpAngleRate[0]*RAD_TO_DEG,Wiimote::_mpAngleRate[1]*RAD_TO_DEG,Wiimote::_mpAngleRate[2]*RAD_TO_DEG,Wiimote::_gForce[0],Wiimote::_gForce[1],Wiimote::_gForce[2]);//TESTCLASS
	if(_wiimote->_botB){
		if(reset) _wiimote->_mpPointCoords = _wiimote->getGyroCoords(icubCords,mpPointNormal);
		else _wiimote->_mpPointCoords = _wiimote->getGyroCoords(mpPointCoords,mpPointNormal);
		_icub->setPose(_wiimote->_mpPointCoords);
		reset = false;
	}else{
		reset = true;
		_wiimote->_mpPointCoords = _wiimote->getGyroCoords(mpPointCoords,mpPointNormal);
	}
	lastTime = Time::now();
}
void ThreadCtrl::threadRelease(){
	printf("threadRelease\n");
}

void ThreadCtrl::initThread(ThreadCtrl *thr){
	Time::turboBoost();

	if (!thr->start())
	{
		delete thr;
		return;
	}
}
