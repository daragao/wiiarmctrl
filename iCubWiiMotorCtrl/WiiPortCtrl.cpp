#include "WiiPortCtrl.h"

WiiPortCtrl::WiiPortCtrl(int wiimoteID, GraspPortCtrl *graspCtrl, iCubMotorCtrl *icub, iCubIKinCtrl *icubKin)
{
	initVars();

	stringstream out;
	out << wiimoteID;
	string portPrefix = "/wiimote"+out.str();
	_wiiPortPrefix = portPrefix;
	portPrefix = "/WiiYarpPortsCtrl" + out.str();
	_wiiSolverPortPrefix = portPrefix;

	_icubMotor = icub;
	_icubKin = icubKin;
	_graspCtrl = graspCtrl;
	_portCtrleader = new WiiPortCtrlReader(this);

	openMotorPorts();
	//init lights
	setLEDs(false,false,false,false);
}

WiiPortCtrl::~WiiPortCtrl(void)
{
}

void WiiPortCtrl::initVars(){
	_ctrlMode = NONE;
	_wiiPortPrefix.clear();
}

void WiiPortCtrl::setMotorCtrlMode(){
	printf("Control Mode: MOTOR.\n");
	_ctrlMode = MOTOR;
	setRobotPartCtrl(0);
	setRumble(true);
	setRumble(false);
}

void WiiPortCtrl::setKinCtrlMode(){
	printf("Control Mode: KINEMATIC.\n");
	_ctrlMode = KINEMATIC;
	setRobotPartCtrl(0);
	setRumble(true);
	setRumble(false);
}

void WiiPortCtrl::closeMotorPorts(){
	if(_solver!= NULL && !_solver->isClosed())_solver->close();
	if( _bots != NULL && !_bots->isClosed())_bots->close();
	if( _mp != NULL && !_mp->isClosed())_mp->close();
	if( _wiiWriter != NULL && !_wiiWriter->isClosed())_wiiWriter->close();
	if( _solverWriter != NULL && !_solverWriter->isClosed())_solverWriter->close();
}

void WiiPortCtrl::openMotorPorts(){
	string wiiPortPrefix = _wiiPortPrefix;
	string wiiSolverPortPrefix = _wiiSolverPortPrefix;
	string localPortPrefix = "/icubWiiMotorCtrl";
	string remotePortName;
	string localPortName;
	//readers
	remotePortName = wiiPortPrefix+"/bots";
	localPortName = localPortPrefix+remotePortName;
	_bots = openConnectReaderWiiPort(remotePortName,localPortName);
	if( _bots != NULL) _bots->useCallback(*_portCtrleader);
	remotePortName = wiiPortPrefix+"/ext/motionplus";
	localPortName = localPortPrefix+remotePortName;
	_mp = openConnectReaderWiiPort(remotePortName,localPortName);
	if( _mp != NULL) _mp->useCallback(*_portCtrleader);
	//solver reader
	remotePortName = wiiSolverPortPrefix+"/solverWriter";
	localPortName = localPortPrefix+remotePortName;
	_solver = openConnectReaderWiiPort(remotePortName,localPortName);
	if( _solver != NULL) _solver->useCallback(*_portCtrleader);
	//writers
	BufferedPort<Bottle> *port;
	port = new BufferedPort<Bottle>();
	localPortName = localPortPrefix+_wiiPortPrefix+"/writer";
	port->open(localPortName.c_str());
	remotePortName = wiiPortPrefix+"/reader";
	Network::connect(localPortName.c_str(),remotePortName.c_str());
	_wiiWriter = port;
	//solver writer
	port = new BufferedPort<Bottle>();
	localPortName = localPortPrefix+wiiSolverPortPrefix+"/solverReader";
	port->open(localPortName.c_str());
	remotePortName = wiiSolverPortPrefix+"/solverReader";
	Network::connect(localPortName.c_str(),remotePortName.c_str());
	_solverWriter = port;
}

BufferedPort<Bottle> *WiiPortCtrl::openConnectReaderWiiPort(string remotePortName, string localPortName){
	bool portSuccess = false;
	BufferedPort<Bottle> *port = new BufferedPort<Bottle>();
	portSuccess = port->open(localPortName.c_str());
	if(!portSuccess){
		printf("ERROR: Couldn't open local port: %s\n",localPortName.c_str());
		return NULL;
	}
	portSuccess = Network::connect(remotePortName.c_str(),localPortName.c_str());
	if(!portSuccess){
		printf("ERROR: Couldn't connect to remote port: %s\n",remotePortName.c_str());
		port->close();
		return NULL;
	}
	return port;
}

void WiiPortCtrl::setMotionPlus(bool setOn){
	int stateInt = (setOn?1:0);
	BufferedPort<Bottle> *writerPort = _wiiWriter;
	Bottle& output = writerPort->prepare();
	output.clear();
	output.addVocab(WII_MOTIONPLUS_VOCAB);
	output.addInt(stateInt);
	writerPort->write();
}

void WiiPortCtrl::setLEDs(bool led1, bool led2, bool led3, bool led4){
	BufferedPort<Bottle> *writerPort = _wiiWriter;
	Bottle& output = writerPort->prepare();
	output.clear();
	output.addVocab(WII_LED_VOCAB);
	output.addInt(led1);
	output.addInt(led2);
	output.addInt(led3);
	output.addInt(led4);
	writerPort->write();
}

void WiiPortCtrl::setRumble(bool isOn){
	BufferedPort<Bottle> *writerPort = _wiiWriter;
	Bottle& output = writerPort->prepare();
	output.clear();
	output.addVocab(WII_RUMBLE_VOCAB);
	writerPort->write();
}

void WiiPortCtrl::setSolverVector(double x, double y, double z){
	if(_ctrlMode == KINEMATIC){
		BufferedPort<Bottle> *writerPort = _solverWriter;
		Bottle& output = writerPort->prepare();
		output.clear();
		output.addVocab(WIISOLVER_POINT_VOCAB);
		output.addDouble(x);
		output.addDouble(y);
		output.addDouble(z);
		writerPort->write();
	}
}

void WiiPortCtrl::setSolverAxisAngle(double angle, double x, double y, double z){
	if(_ctrlMode == KINEMATIC){
		BufferedPort<Bottle> *writerPort = _solverWriter;
		Bottle& output = writerPort->prepare();
		output.clear();
		output.addVocab(WIISOLVER_AXISANGLE_VOCAB);
		output.addDouble(angle);
		output.addDouble(x);
		output.addDouble(y);
		output.addDouble(z);
		writerPort->write();
	}
}

void WiiPortCtrl::setRotationMatrix(double rotMAtrix[3][3]){
	BufferedPort<Bottle> *writerPort = _solverWriter;
	Bottle& output = writerPort->prepare();
	output.clear();
	output.addVocab(WIISOLVER_ROTMAT_VOCAB);
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			output.addDouble(rotMAtrix[i][j]);
	writerPort->write();
}

void WiiPortCtrl::setRobotPartCtrl(int partID){
	iCubMotorCtrl *icubMotor = _icubMotor;
	iCubIKinCtrl *icubKin = _icubKin;
	if(_ctrlMode == MOTOR){
		switch(partID){
			case 0:
				setLEDs(false,false,false,false);
				icubMotor->toggleActivePart(0);
				break;
			case 1:
				//arm active
				setLEDs(true,true,false,false);
				icubMotor->toggleActivePart(1);
				break;
			case 2:
				//forearm active
				setLEDs(false,false,true,true);
				icubMotor->toggleActivePart(2);
				break;
		}
	}
	if(_ctrlMode == KINEMATIC){
		switch(partID){
						case 0:
							setLEDs(false,false,false,false);
							break;
						case 1:
							//position move active
							setLEDs(true,true,false,false);
							icubKin->setActivePart(1);
							break;
						case 2:
							//orientation active
							setLEDs(false,false,true,true);
							icubKin->setActivePart(2);
							break;
						case 3:
							//position move (with buttons) active
							setLEDs(true,false,false,true);
							icubKin->setActivePart(1);
							break;
		}
	}
}

void WiiPortCtrl::setSolverKinematicState(bool isOrientationActive, double pos[3], double ori[4]/*angle/axis like in icub*/, double rotMatrix[3][3]){
	double shoulderPos[3] = {0,0,0};//{-0.009,0.094,-0.177+0.3};
	double newPosition[3] = {0,0,0};
	double firstPoint[3] = {0,1,0};
	double orientationFirstPoint[3] = {1,0,0};
	double rotMatrixTemp[3][3], newRotMatrix[3][3];
	if(isOrientationActive){
		Utils::axisAngle2Matrix(rotMatrix,ori[0],ori[1],ori[2],ori[3]);

		//iCub X axis with the align Y wiimote axis
		Utils::axisAngle2Matrix(rotMatrixTemp,(90*DEG_TO_RAD),0,0,1);
		Utils::mult3DMatrixVector(newRotMatrix[0],rotMatrixTemp,rotMatrix[0]);
		Utils::mult3DMatrixVector(newRotMatrix[1],rotMatrixTemp,rotMatrix[1]);
		Utils::mult3DMatrixVector(newRotMatrix[2],rotMatrixTemp,rotMatrix[2]);

		setRotationMatrix(newRotMatrix);
	}else{
		newPosition[0] = pos[0];
		newPosition[1] = pos[1];
		newPosition[2] = pos[2];
		Utils::normalizeVector(newPosition,newPosition);
		double angle = Utils::dotProduct(firstPoint,newPosition);
		angle = acos(angle);
		Utils::crossProduct(newPosition,firstPoint,newPosition);
		Utils::axisAngle2Matrix(rotMatrix,angle,newPosition[0],newPosition[1],newPosition[2]);
		resetUpVector(newRotMatrix,pos,rotMatrix);
		setRotationMatrix(newRotMatrix);
	}
}

void WiiPortCtrl::resetUpVector(double newRotMatrix[3][3], double solverPoint[3], double rotMatrix[3][3]){
	double resetUpVectorRotMatrix[3][3];
	double planeNormal[3],axis = 1;
	double newPosition[3] = {0,0,1};
	double rotatedVector[3] = {rotMatrix[2][0],rotMatrix[2][1],rotMatrix[2][2]};
	Utils::normalizeVector(solverPoint,solverPoint);
	Utils::normalizeVector(rotatedVector,rotatedVector);

	Utils::crossProduct(planeNormal,solverPoint,newPosition);
	Utils::normalizeVector(planeNormal,planeNormal);

	double angle = Utils::dotProduct(rotatedVector,planeNormal);
	angle = asin(angle);
	if(solverPoint[0]<0 && solverPoint[1]<0 && solverPoint[2]>0) axis = 1;
	if(solverPoint[0]<0 && solverPoint[1]>0 && solverPoint[2]>0) axis = 1;
	if(solverPoint[0]<0 && solverPoint[1]<0 && solverPoint[2]<0) axis = -1;
	if(solverPoint[0]<0 && solverPoint[1]>0 && solverPoint[2]<0) axis = -1;
	Utils::axisAngle2Matrix(resetUpVectorRotMatrix,angle,0,axis,0);
	Utils::mult3DMatrix(newRotMatrix,rotMatrix,resetUpVectorRotMatrix);
	//printf("angle: %0.2f \n",angle*RAD_TO_DEG);
}

void WiiPortCtrl::moveRelativeRobotKinematic(int pressedBot){
	double step = 0.01;
	iCubIKinCtrl *icubKin = _icubKin;
	static double pos[3] =  {0,1,0}, ori[4] = {0,0,0,0};
	if(_ctrlMode == KINEMATIC){
		switch(pressedBot){
			case 1:
				pos[0] += step; //back
				break;
			case 2:
				pos[1] += step; //right
				break;
			case 3:
				pos[0] -= step; //front
				break;
			case 4:
				pos[1] -= step; //left
				break;
			case 5:
				pos[2] -= step; //down
				break;
			case 6:
				pos[2] += step; //up
				break;
		}
		if(pressedBot==0) {
			icubKin->stop();
			icubKin->getCartesianPose(pos,ori);
			icubKin->setRadius(Utils::normVector(pos));
		} else 
			icubKin->movePositionWithOrientation(pos,ori);

	}
}

void WiiPortCtrl::moveRobotKinematic(bool isBotPressed, bool doStop, double solverPoint[3], double rotMAtrix[3][3]){
	iCubIKinCtrl *icubKin = _icubKin;
	double shoulderPosition[3];//useful for the solver translation,so that the rotation is relative to the shoulder and not the 0,0,0 of the icub
	icubKin->getShoulderPosition(shoulderPosition);
	bool isOrientationActive = icubKin->isOrientationActive();
	static double pos[3] =  {0,1,0}, ori[4] = {0,0,0,0};
	double newPos[3], newSolverPoint[3];
	Utils::normalizeVector(solverPoint,solverPoint);
	newSolverPoint[0] = (solverPoint[0] * icubKin->getRadius()) + shoulderPosition[0];
	newSolverPoint[1] = (solverPoint[1] * icubKin->getRadius()) + shoulderPosition[1];
	newSolverPoint[2] = (solverPoint[2] * icubKin->getRadius()) + shoulderPosition[2];

	Utils::normalizeVector(newSolverPoint,newSolverPoint);
	newSolverPoint[0] *= icubKin->getRadius();
	newSolverPoint[1] *= icubKin->getRadius();
	newSolverPoint[2] *= icubKin->getRadius();

	if(_ctrlMode == KINEMATIC){
		if(!isBotPressed){
			if(doStop) icubKin->stop();
			icubKin->getCartesianPose(pos,ori);
			icubKin->setRadius(Utils::normVector(pos));
			newPos[0] = pos[0] - shoulderPosition[0];
			newPos[1] = pos[1] - shoulderPosition[1];
			newPos[2] = pos[2] - shoulderPosition[2];
			setSolverKinematicState(isOrientationActive,newPos,ori,rotMAtrix);
		}else{
			icubKin->kinecticMoveActivePart(pos,ori,newSolverPoint,rotMAtrix);
		}
	}
}

void WiiPortCtrl::addToKinematicRadius(double meters){
	iCubIKinCtrl *icubKin = _icubKin;
	if(_ctrlMode == KINEMATIC){
		icubKin->addToRadius(meters);
	}
}

void WiiPortCtrl::moveRobotMotor(bool isBotPressed, double mpRaw[3],double mpAngle[3]){
	iCubMotorCtrl *icubMotor = _icubMotor;
	if(_ctrlMode == MOTOR){
		if(!isBotPressed){
			icubMotor->stopMotion();
			icubMotor->resetSavedAngle();
			icubMotor->velocityMoveActivePart(0,0,0);
		}else{
			icubMotor->velocityMoveActivePart(mpAngle[0],mpAngle[1],mpAngle[2]);
		}
	}
}

void WiiPortCtrl::doGraspAction(){
	Time::turboBoost();
	static double lastTime = Time::now();
	double nowTime = Time::now();
	if((nowTime-lastTime) < 3 && (nowTime-lastTime) != 0)
		return;
	GraspPortCtrl *graspCtrl = _graspCtrl;
	static int graspActionID = 0;
	switch(graspActionID){
		case 0:
			graspCtrl->sendCommand("do PREGRIP");
			break;
		case 1:
			graspCtrl->sendCommand("do GRIP");
			break;
		case 2:
			graspCtrl->sendCommand("do RELEASE");
			break;
	}
	graspActionID = (++graspActionID)%3;
	lastTime = nowTime;

}

//WiiPortCtrlReader//WiiPortCtrlReader//WiiPortCtrlReader//WiiPortCtrlReader//WiiPortCtrlReader//WiiPortCtrlReader
//WiiPortCtrlReader//WiiPortCtrlReader//WiiPortCtrlReader//WiiPortCtrlReader//WiiPortCtrlReader//WiiPortCtrlReader
//WiiPortCtrlReader//WiiPortCtrlReader//WiiPortCtrlReader//WiiPortCtrlReader//WiiPortCtrlReader//WiiPortCtrlReader

WiiPortCtrlReader::WiiPortCtrlReader(WiiPortCtrl *wiiPortCtrl){
	_wiiPortCtrl = wiiPortCtrl;
}

void WiiPortCtrlReader::initVars(){
	_wiiPortCtrl= NULL;
	_botB = false;
	_kinBotPressed = false;
	_useKinematicButtons = true;
}

WiiPortCtrlReader::~WiiPortCtrlReader(){
}

void WiiPortCtrlReader::onRead(Bottle& b){
	int vocab = b.get(0).asVocab();
	switch(vocab){
		case WII_BOT_VOCAB:
			checkButtons(b);
			break;
		case WII_MOTIONPLUS_VOCAB:
			updateMotionPlusData(b);
			break;
		case WII_MPSOLVER_VOCAB:
			updateSolverData(b);
			break;
	}
}

void WiiPortCtrlReader::checkButtons(Bottle& b){
	WiiPortCtrl *wiiPortCtrl = _wiiPortCtrl;
	int i = 1;
	_botB = false;
	_kinBotPressed = false;
	bool radiusChanged = false;
	static bool radiusWasChanging = false;
	for(int vocab = b.get(i).asVocab(); vocab != 0; vocab = b.get(++i).asVocab()){
		switch(vocab){
		case WII_BOTA_VOCAB:
			//wiiPortCtrl->setMotionPlus(true);
			wiiPortCtrl->doGraspAction();
			break;
		case WII_BOTB_VOCAB:
			_botB = true;
			break;
		case WII_BOTDOWN_VOCAB:
			if(_useKinematicButtons){
				wiiPortCtrl->moveRelativeRobotKinematic(1);
				_kinBotPressed = true;
			}
			break;
		case WII_BOTUP_VOCAB:
			if(_useKinematicButtons){
				wiiPortCtrl->moveRelativeRobotKinematic(3);
				_kinBotPressed = true;
			}
			break;
		case WII_BOTLEFT_VOCAB:
			if(_useKinematicButtons){
				wiiPortCtrl->moveRelativeRobotKinematic(4);
				_kinBotPressed = true;
			}
			break;
		case WII_BOTRIGHT_VOCAB:
			if(_useKinematicButtons){
				wiiPortCtrl->moveRelativeRobotKinematic(2);
				_kinBotPressed = true;
			}
			break;
		case WII_BOTHOME_VOCAB:
			wiiPortCtrl->setRobotPartCtrl(3);
			_useKinematicButtons = true;
			break;
		case WII_BOTMINUS_VOCAB:
			if(_useKinematicButtons){
				wiiPortCtrl->moveRelativeRobotKinematic(5);
			}else{
				wiiPortCtrl->addToKinematicRadius(-0.01);
				radiusChanged = true;
			}
			_kinBotPressed = true;
			break;
		case WII_BOTPLUS_VOCAB:
			if(_useKinematicButtons){
				wiiPortCtrl->moveRelativeRobotKinematic(6);
			}else {
				wiiPortCtrl->addToKinematicRadius(0.01);
				radiusChanged = true;
			}
			_kinBotPressed = true;
			break;
		case WII_BOTONE_VOCAB:
			wiiPortCtrl->setRobotPartCtrl(1);
			_useKinematicButtons = false;
			break;
		case WII_BOTTWO_VOCAB:
			wiiPortCtrl->setRobotPartCtrl(2);
			_useKinematicButtons = false;
			break;
		}
	}
	if((_useKinematicButtons&&!_kinBotPressed)||(!_useKinematicButtons&&radiusWasChanging&&!radiusChanged)){
		wiiPortCtrl->moveRelativeRobotKinematic(0);
	}
	radiusWasChanging = radiusChanged;
}

void WiiPortCtrlReader::updateMotionPlusData(Bottle& b){
	WiiPortCtrl *wiiPortCtrl = _wiiPortCtrl;

	double calibStatus = b.get(1).asDouble();
	double mpRaw[3] = {0,0,0};
	mpRaw[0] = b.get(2).asDouble();
	mpRaw[1] = b.get(3).asDouble();
	mpRaw[2] = b.get(4).asDouble();
	double mpAngle[3] = {0,0,0};
	mpAngle[0] = (abs(b.get(5).asDouble()) > VERY_LITTLE) ? b.get(5).asDouble() : 0;
	mpAngle[1] = (abs(b.get(6).asDouble()) > VERY_LITTLE) ? b.get(6).asDouble() : 0;
	mpAngle[2] = (abs(b.get(7).asDouble()) > VERY_LITTLE) ? b.get(7).asDouble() : 0;
	wiiPortCtrl->moveRobotMotor(_botB,mpRaw,mpAngle);
}

void WiiPortCtrlReader::updateSolverData(Bottle& b){
	int i = 1;
	//b.get(1).asVocab() == PNT (Point)
	i++;
	double point[3];
	point[0] = b.get(i++).asDouble();
	point[1] = b.get(i++).asDouble();
	point[2] = b.get(i++).asDouble();
	//b.get(5).asVocab() == ROTM (RotationMatrix)
	i++;
	double rotMat[3][3];
	rotMat[0][0] = b.get(i++).asDouble();
	rotMat[0][1] = b.get(i++).asDouble();
	rotMat[0][2] = b.get(i++).asDouble();
	rotMat[1][0] = b.get(i++).asDouble();
	rotMat[1][1] = b.get(i++).asDouble();
	rotMat[1][2] = b.get(i++).asDouble();
	rotMat[2][0] = b.get(i++).asDouble();
	rotMat[2][1] = b.get(i++).asDouble();
	rotMat[2][2] = b.get(i++).asDouble();

	WiiPortCtrl *wiiPortCtrl = _wiiPortCtrl;
	if(_useKinematicButtons){
		wiiPortCtrl->moveRobotKinematic(false,false,point,rotMat);//is using buttons
	}else{
		wiiPortCtrl->moveRobotKinematic(_botB,true,point,rotMat);//is using gyroscope
	}
}