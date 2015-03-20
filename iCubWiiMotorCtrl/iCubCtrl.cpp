#include "iCubCtrl.h"

iCubCtrl::iCubCtrl(string robotName, string partName)
{
	initVars();
	_robot = robotName;
	_part = partName;
}

iCubCtrl::~iCubCtrl(void)
{
}

void iCubCtrl::initVars(){
	_robot.clear();
	_part.clear();
	_robotMotorDevice = NULL;
	_robotKinematicDevice = NULL;
	_pos = NULL;
    _vel = NULL;
    _enc = NULL;
    _pid = NULL;
    _amp = NULL;
    _lim = NULL;
	_jnts = 0;
	_armCart = NULL;
}

void iCubCtrl::closeRobot(){
	if(_robotMotorDevice!=NULL)
		_robotMotorDevice->close();
	if(_robotKinematicDevice!=NULL)
		_robotKinematicDevice->close();
	_robotMotorDevice = NULL;
	_robotKinematicDevice = NULL;
}

bool iCubCtrl::initRobotKinematic(){
	string device = "cartesiancontrollerclient";
	string remotePort = "/" + _robot + "/cartesianController/" + _part;
	string localPort = "/iCubWiiMotorCtrl"+remotePort+"/client1";
	Property options;
	options.put("device", device.c_str());
	options.put("local", localPort.c_str());
	options.put("remote", remotePort.c_str());

	PolyDriver *clientCartCtrl = new PolyDriver(options);
	if (!clientCartCtrl->isValid()) {
		printf("Device 'cartesiancontrollerclient' not available.\n");
		return false;
	}
	_robotKinematicDevice = clientCartCtrl;

	ICartesianControl *armCart=NULL;

	if (clientCartCtrl->isValid()) {
		clientCartCtrl->view(armCart);
		_armCart = armCart;
		_armCart->setTrackingMode(false);
	}

	return true;
}

bool iCubCtrl::initRobotMotor(){
	string device = "remote_controlboard";
	string remotePort = "/" + _robot + "/" + _part;
	string localPort = "/iCubWiiMotorCtrl"+remotePort+"/client";
	Property options;
	options.put("device", device.c_str());
	options.put("local", localPort.c_str());
	options.put("remote", remotePort.c_str());

	PolyDriver *robotDevice = new PolyDriver(options);
	if (!robotDevice->isValid()) {
		printf("Device 'remote_device' not available.\n");
		return false;
	}
	_robotMotorDevice = robotDevice;

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
	_pos = pos;
    _vel = vel;
    _enc = enc;
    _pid = pid;
    _amp = amp;
    _lim = lim;
	

	if (pos==0) {
      printf("Error getting IPositionControl interface.\n");
      return false;
    }
	int jnts = 0;
    pos->getAxes(&jnts);
	_jnts = jnts;

	for (int i = 0; i < jnts; i++) {
		amp->enableAmp(i);
		pid->enablePid(i);
	}

	return true;
}

void iCubCtrl::cartesianMove(double transX, double transY, double transZ){
	ICartesianControl *armCart = _armCart;
	if(armCart == NULL) return;
	Vector xd;
	xd.resize(3);
	xd[0] = transX;//meters
	xd[1] = transY;//meters
	xd[2] = transZ;//meters
	armCart->goToPosition(xd);
}

void iCubCtrl::cartesianDesired(double pos[3], double ori[4]){
	ICartesianControl *armCart = _armCart;
	if(armCart == NULL) return;
	Vector posVector,oriVector,joints;
	posVector.resize(3);
	oriVector.resize(4);
	posVector[0] = pos[0];
	posVector[1] = pos[1];
	posVector[2] = pos[2];
	oriVector[3] = ori[0];
	oriVector[0] = ori[1];
	oriVector[1] = ori[2];
	oriVector[2] = ori[3];
	armCart->getDesired(posVector,oriVector,joints);
	pos[0] = posVector[0];
	pos[1] = posVector[1];
	pos[2] = posVector[2];
	ori[3] = oriVector[0];//angle
	ori[0] = oriVector[1];
	ori[1] = oriVector[2];
	ori[2] = oriVector[3];
}

void iCubCtrl::cartesianMove(double transX, double transY, double transZ, double oriAngle, double oriX, double oriY, double oriZ){
	ICartesianControl *armCart = _armCart;
	if(armCart == NULL) return;
	Vector xd,od;
	xd.resize(3);
	od.resize(4);
	xd[0] = transX;//meters
	xd[1] = transY;//meters
	xd[2] = transZ;//meters
	od[3] = oriAngle;
	od[0] = oriX;
	od[1] = oriY;
	od[2] = oriZ;
	armCart->goToPose(xd,od);   
}

void iCubCtrl::setCartTrajTime(double seconds){
	ICartesianControl *armCart = _armCart;
	if(armCart == NULL) return;
	_armCart->setTrajTime(seconds);
}

//first argument is the number of DOFs to be changed
//the others are the new values of the DOF - (-1) mantains the old value
void iCubCtrl::setCartDOF(int dofSize, ...){
	ICartesianControl *armCart = _armCart;
	if(armCart == NULL) return;
	// get current dofs
	Vector newDof, curDof;
	armCart->getDOF(curDof);
	newDof=curDof;

	// enable the torso yaw and pitch
	// disable the torso roll
	va_list list;
	va_start(list,dofSize);
	for(int i = 1;i<=dofSize; i++){
		int p =va_arg(list,int);
		if(p==-1) continue;
		newDof[i] = p;
	}
	va_end(list);

	// send the request for dofs reconfiguration
	armCart->setDOF(newDof,curDof);
}

void iCubCtrl::positionMove(int jointID, double acceleration, double speed, double angle){
	IPositionControl *pos = _pos;
	pos->setRefAcceleration(jointID,acceleration);
	pos->setRefSpeed(jointID,speed);
	//pos->relativeMove(jointID,angle);//not working I don't know why
	//pos->positionMove(jointID,getAngle(jointID)+angle);
	pos->positionMove(jointID,angle);
}

void iCubCtrl::velocityMove(int jointID, double acceleration, double startSpeed){
	IVelocityControl *vel = _vel;
	vel->setRefAcceleration(jointID,acceleration);
	vel->velocityMove(jointID,startSpeed);
}


bool iCubCtrl::velocityMove(int jointID, double angleDeg, double acceleration, double speed){
	bool result = false;
	double limMin;
	double limMax;
	double currAngle;
	double offset = 5;
	double limit = 2;
	_enc->getEncoder(jointID,&currAngle);
	angleDeg += currAngle;
	_lim->getLimits(jointID,&limMin,&limMax);
	_vel->setRefAcceleration(jointID,acceleration);
	double tempVal;
	if(!(limMin>currAngle||limMax<currAngle)||!(limMin>angleDeg||limMax<angleDeg)){
		tempVal = (angleDeg-currAngle);
		tempVal = (tempVal<0?-tempVal:tempVal);
		if(tempVal<offset){
			
			speed = (speed/2.0) * (tempVal/offset);
		}
		if(tempVal>limit){
			if(currAngle<angleDeg){
				_vel->velocityMove(jointID,speed);
			}
			if(currAngle>angleDeg){
				_vel->velocityMove(jointID,-speed);
			}
		}else{
			_vel->velocityMove(jointID,0);
			Time::delay((2.0/1000.0));
			_vel->stop(jointID);
		}
	}
	return result;
}


void iCubCtrl::stopKinematics(){
	ICartesianControl *armCart = _armCart;
	armCart->stopControl();
}

void iCubCtrl::getCartesianPose(int joint, double posArray[3], double oriArray[4]){
	if(_armCart==NULL)return;
		ICartesianControl *armCart = _armCart;
	Vector position,orientation;
	position.resize(3);
	orientation.resize(4);
	armCart->getPose(joint,position,orientation);
	posArray[0] = position[0];
	posArray[1] = position[1];
	posArray[2] = position[2];
	oriArray[0] = orientation[3];
	oriArray[1] = orientation[0];
	oriArray[2] = orientation[1];
	oriArray[3] = orientation[2];
}

void iCubCtrl::getCartesianPose(double posArray[3], double oriArray[4]){
	ICartesianControl *armCart = _armCart;
	Vector position,orientation;
	position.resize(3);
	orientation.resize(4);
	armCart->getPose(position,orientation);
	posArray[0] = position[0];
	posArray[1] = position[1];
	posArray[2] = position[2];
	oriArray[0] = orientation[3];
	oriArray[1] = orientation[0];
	oriArray[2] = orientation[1];
	oriArray[3] = orientation[2];
}

void iCubCtrl::stopJoint(int jointID){
	IPositionControl *pos = _pos;
	IVelocityControl *vel = _vel;
	pos->stop(jointID);
	vel->stop(jointID);
}

double iCubCtrl::getAngle(int jointID){
	double result = 0;
	IEncoders *enc = _enc;
	enc->getEncoder(jointID,&result);
	return result;
}

bool iCubCtrl::isOverLimit(int jointID, double angle){
	double min = 0, max = 0;
	IControlLimits *lim = _lim;
	lim->getLimits(jointID,&min,&max);
	if(angle > min && angle < max)
		return false;
	else return true;
}
