#include "iCubCtrl.h"

iCubCtrl::iCubCtrl(void)
{
	initDevice();
	initThread();
}

void iCubCtrl::initThread(){
	Time::turboBoost();

	_thr=new iCubCtrlThread(this, 0.05);
	if (!_thr->start())
	{
		delete _thr;
		return;
	}
	//while(1);
}


iCubCtrl::~iCubCtrl(void)
{
	Util::icubData->isConnected = false;
	_robotDeviceIkinRightArm->close();
}

bool iCubCtrl::initDevice(){
	Util::icubData->isConnected = initIkinDevice();
	//Util::icubData->isConnected = initMotorDevice();
	return Util::icubData->isConnected;
}

bool iCubCtrl::initIkinDevice(){
	PolyDriver *robotDevice = _robotDeviceIkinRightArm = NULL;
	bool ok = false;
	Property options = setProperties("icubSim", "cartesianController/right_arm", "cartesiancontrollerclient", "/rightArmIkin/client");
	robotDevice = new PolyDriver(options);
	ok = robotDevice->isValid();
	if (!ok) return false;
	ok = robotDevice->view(_ikinRightArm);
	if (!ok) return false;
	_robotDeviceIkinRightArm = robotDevice;
	return true;
}

bool iCubCtrl::initMotorDevice(){
	PolyDriver *robotDevice = _robotDeviceMotorRightArm = NULL;
	bool ok = false;
	Property options = setProperties("icubSim", "right_arm", "remote_controlboard", "/rightArmMotor/client");
	robotDevice = new PolyDriver(options);
	ok = robotDevice->isValid();
	if (!ok) return false;
	_robotDeviceMotorRightArm = robotDevice;

	IPositionControl *pos;
	IVelocityControl *vel;
	IEncoders *enc;
	IPidControl *pid;
	IAmplifierControl *amp;
	IControlLimits *lim;

	robotDevice->view(pos);
	robotDevice->view(vel);
	robotDevice->view(enc);
	robotDevice->view(pid);
	robotDevice->view(amp);
	robotDevice->view(lim);
	
	int jnts = 0;
	pos->getAxes(&jnts);

	Vector tmp;
	Vector encoders;
	Vector command_position;
	Vector command_velocity;
	tmp.resize(jnts);
	encoders.resize(jnts);
	command_position.resize(jnts);
	command_velocity.resize(jnts);

	int i;
	for (i = 0; i < jnts; i++) {
		tmp[i] = 50.0;
	}
	pos->setRefAccelerations(tmp.data());

	enc->getEncoders(encoders.data());

	for (i = 0; i < jnts; i++) {
		tmp[i] = 80.0;//deg/sec
	}
	pos->setRefSpeeds(tmp.data());

	return true;
}

Property iCubCtrl::setProperties(string robotName, string part, string device, string localPorts){
	string remotePorts	= "/" + robotName + "/" + part;

	Property options;
	options.put("device", device.c_str());
	options.put("local", localPorts.c_str());   //local port names
	options.put("remote", remotePorts.c_str());         //where we connect to
	return options;
}

void iCubCtrl::moveIkin(ICartesianControl * part){

	//motion plus angle
	Vector mpAngle;
	mpAngle.resize(3);
	mpAngle[0] = Util::wiiData->motionPlusAngleX/DEG_TO_RAD;
	mpAngle[1] = Util::wiiData->motionPlusAngleY/DEG_TO_RAD;
	mpAngle[2] = Util::wiiData->motionPlusAngleZ/DEG_TO_RAD;
	Util::wiiData->motionPlusAngleX = 0;
	Util::wiiData->motionPlusAngleY = 0;
	Util::wiiData->motionPlusAngleZ = 0;

	//origin
	Vector x0, o0;
	part->getPose(x0, o0);

	Util::icubData->posRightArm[0] = x0[0];
	Util::icubData->posRightArm[1] = x0[1];
	Util::icubData->posRightArm[2] = x0[2];
	Util::icubData->oriRightArm[0] = o0[0];
	Util::icubData->oriRightArm[1] = o0[1];
	Util::icubData->oriRightArm[2] = o0[2];
	Util::icubData->oriRightArm[3] = o0[3];

	//destination
	Vector od=o0;
	Vector xd=x0;
	Vector newD;

	newD = rotationMatrix(mpAngle[0],mpAngle[2],mpAngle[1],xd);

	/*double angleX = Util::wiiData->motionPlusAngleX;
	double angleY = Util::wiiData->motionPlusAngleY;
	double angleZ = Util::wiiData->motionPlusAngleZ;
	double test = 3.14;//Util::wiiData->temp;
	double orient[4];
	Util::rotate(orient,angleX,angleY,angleZ);*/

	/*
	od[0]=orient[0];
	od[1]=orient[1];
	od[2]=orient[2];
	od[3]=orient[3];
	*/

	//od[0] = 1;
	//od[1] = 0;
	//od[2] = 0;
	//od[3] = 3.14/2;
	//xd[0] = 2;
	part->setTrajTime(0.1);
	part->goToPosition(newD);
}

void iCubCtrl::moveMotors(PolyDriver *robotDevice){

	IPositionControl *pos;
	IVelocityControl *vel;
	IEncoders *enc;
	IPidControl *pid;
	IAmplifierControl *amp;
	IControlLimits *lim;

	robotDevice->view(pos);
	robotDevice->view(vel);
	robotDevice->view(enc);
	robotDevice->view(pid);
	robotDevice->view(amp);
	robotDevice->view(lim);

	double motionPlusNorm = sqrt(pow(Util::wiiData->motionPlusX,2)+pow(Util::wiiData->motionPlusY,2)+pow(Util::wiiData->motionPlusZ,2));
	if(motionPlusNorm != 0) {
		pos->setRefSpeed(0,80*abs(Util::wiiData->motionPlusX/motionPlusNorm));
		pos->setRefSpeed(1,80*abs(Util::wiiData->motionPlusZ/motionPlusNorm));
		pos->setRefSpeed(2,80*abs(Util::wiiData->motionPlusY/motionPlusNorm));
	}
	if(Util::wiiData->botA){
		pos->relativeMove(0,Util::wiiData->motionPlusAngleX/DEG_TO_RAD);
		pos->relativeMove(1,Util::wiiData->motionPlusAngleZ/DEG_TO_RAD);
		pos->relativeMove(2,Util::wiiData->motionPlusAngleY/DEG_TO_RAD);
	}
	if(Util::wiiData->botB){
		pos->relativeMove(4,Util::wiiData->motionPlusAngleY/DEG_TO_RAD);
		pos->relativeMove(5,Util::wiiData->motionPlusAngleX/DEG_TO_RAD);
		pos->relativeMove(6,Util::wiiData->motionPlusAngleZ/DEG_TO_RAD);
	}
	Util::wiiData->motionPlusAngleX = 0;
	Util::wiiData->motionPlusAngleY = 0;
	Util::wiiData->motionPlusAngleZ = 0;
}

bool iCubCtrl::refreshICub(){
	if(!Util::icubData->isConnected)
		return false;
	moveIkin(_ikinRightArm);
	//moveMotors(_robotDeviceMotorRightArm);
	return true;
}

Vector iCubCtrl::rotationMatrix(double ro, double teta, double psy, Vector posV){
	Matrix rotMatrix(3,3);
	rotMatrix[0][0] = cos(teta)*cos(psy);
	rotMatrix[0][1] = -cos(teta)*sin(psy)+sin(teta)*sin(ro)*cos(psy);
	rotMatrix[0][2] = sin(ro)*sin(psy)+cos(ro)*sin(teta)*cos(psy);

	rotMatrix[1][0] = cos(teta)*sin(psy);
	rotMatrix[1][1] = cos(teta)*cos(psy)+sin(teta)*sin(ro)*sin(psy);
	rotMatrix[1][2] = -sin(ro)*cos(psy)+cos(ro)*sin(teta)*sin(psy);

	rotMatrix[2][0] = -sin(teta);
	rotMatrix[2][1] = sin(ro)*cos(teta);
	rotMatrix[2][2] = cos(ro)*cos(teta);

	Vector newVec(3);
	newVec.zero();
	newVec[0] = rotMatrix[0][0]*posV[0]+rotMatrix[0][1]*posV[1]+rotMatrix[0][2]*posV[2];
	newVec[1] = rotMatrix[1][0]*posV[0]+rotMatrix[1][1]*posV[1]+rotMatrix[1][2]*posV[2];
	newVec[2] = rotMatrix[2][0]*posV[0]+rotMatrix[2][1]*posV[1]+rotMatrix[2][2]*posV[2];
	newVec[0] = abs(newVec[0]) < 0.0001 ? 0 : newVec[0];
	newVec[1] = abs(newVec[1]) < 0.0001 ? 0 : newVec[1];
	newVec[2] = abs(newVec[2]) < 0.0001 ? 0 : newVec[2];
	return newVec;
}

// iCubCtrlThread// iCubCtrlThread// iCubCtrlThread// iCubCtrlThread
iCubCtrlThread::iCubCtrlThread(iCubCtrl *icub, const double period) : RateThread(int(period*1000.0)) {
	_icub = icub;
}
bool iCubCtrlThread::threadInit(){
	return true;
}
void iCubCtrlThread::beforeStart(){}
void iCubCtrlThread::afterStart(bool s){
}
void iCubCtrlThread::run(){
	_icub->refreshICub();
}
void iCubCtrlThread::threadRelease(){}
