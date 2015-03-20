#include "WiiPortCtrl.h"

WiiPortCtrl::WiiPortCtrl(int wiimoteID, GLWindow *glWin)
{
	_botsReader = new WiiPortCtrlBotsReader(this,glWin);
	_glWin = glWin;
	openPorts(wiimoteID);
}

WiiPortCtrl::~WiiPortCtrl(void)
{
}

void WiiPortCtrl::openPorts(int wiimoteID){
	stringstream out;
	out << wiimoteID;
	string portPrefix = "/wiimote"+out.str();
	string solverPort = "/WiiYarpPortsCtrl"+out.str();
	BufferedPort<Bottle> *port;
	port = new BufferedPort<Bottle>();
	port->open("/TestWiiYarpOpenGL/bots");
	string portName = portPrefix+"/bots";
	Network::connect(portName.c_str(),"/TestWiiYarpOpenGL/bots");
	port->useCallback(*_botsReader);
	_bots = port;
	port = new BufferedPort<Bottle>();
	port->open("/TestWiiYarpOpenGL/writer");
	portName = portPrefix+"/reader";
	Network::connect("/TestWiiYarpOpenGL/writer",portName.c_str());
	_reader = port;
	port = new BufferedPort<Bottle>();
	port->open("/TestWiiYarpOpenGL/motionplus");
	portName = portPrefix+"/ext/motionplus";
	Network::connect(portName.c_str(),"/TestWiiYarpOpenGL/motionplus");
	port->useCallback(*_botsReader);
	_mp = port;

	port = new BufferedPort<Bottle>();
	port->open("/TestWiiYarpOpenGL/solver");
	portName = solverPort+"/solverWriter";
	Network::connect(portName.c_str(),"/TestWiiYarpOpenGL/solver");
	port->useCallback(*_botsReader);
	_solver = port;

	port = new BufferedPort<Bottle>();
	port->open("/TestWiiYarpOpenGL/solverInput");
	portName = solverPort+"/solverReader";
	Network::connect(port->getName().c_str(),portName.c_str());
	_solverInput = port;
}

BufferedPort<Bottle> *WiiPortCtrl::getReader(){
	return _reader;
}

BufferedPort<Bottle> *WiiPortCtrl::getSolverReader(){
	return _solverInput;
}

void WiiPortCtrl::closePorts(){
	if(!_bots->isClosed()) _bots->close();
}

WiiPortCtrlBotsReader::WiiPortCtrlBotsReader(WiiPortCtrl *wiiPortCtrl, GLWindow *glWin){
	_glWin = glWin;
	_wiiPortCtrl = wiiPortCtrl;
}

WiiPortCtrlBotsReader::~WiiPortCtrlBotsReader(void){}

void WiiPortCtrlBotsReader::onRead(Bottle& b){
	static bool rumbleON = false;
	static bool motionplusON = false;
	static double mpLastPressed = Time::now();
	//BUTTONS OPERATIONS
	/*if(b.get(0).asVocab() == WII_BOT_VOCAB){
		for(int i = 1; b.get(i).asVocab() != 0; i++){
			if(b.get(i).asVocab() == WII_BOTHOME_VOCAB){
				rumbleON = !rumbleON;
				Bottle& output = _wiiPortCtrl->getReader()->prepare();
				output.clear();
				output.addVocab(WII_RUMBLE_VOCAB);
				output.addInt(rumbleON);
				_wiiPortCtrl->getReader()->write();
			}
			if(b.get(i).asVocab() == WII_BOTA_VOCAB){
				if(Time::now()-mpLastPressed < 3) return;
				motionplusON = !motionplusON;
				Bottle& output = _wiiPortCtrl->getReader()->prepare();
				output.clear();
				output.addVocab(WII_MOTIONPLUS_VOCAB);
				output.addInt(motionplusON);
				_wiiPortCtrl->getReader()->write();
				mpLastPressed = Time::now();
			}
			if(b.get(i).asVocab() == WII_BOTB_VOCAB){
				Bottle& output = _wiiPortCtrl->getSolverReader()->prepare();
				output.clear();
				output.addVocab(WIISOLVER_AXISANGLE_VOCAB);
				output.addDouble(45);
				output.addDouble(0);
				output.addDouble(0);
				output.addDouble(1);
				_wiiPortCtrl->getSolverReader()->write();
			}
		}
	}*/
	if(b.get(0).asVocab() == WII_MOTIONPLUS_VOCAB){
		int offset = 5;
		if(abs(b.get(offset+0).asDouble())>2) _glWin->_angle[0] += b.get(offset+0).asDouble();
		if(abs(b.get(offset+1).asDouble())>2) _glWin->_angle[1] += b.get(offset+1).asDouble();
		if(abs(b.get(offset+2).asDouble())>2) _glWin->_angle[2] += b.get(offset+2).asDouble();
	}
	if(b.get(0).asVocab() == WII_ICUBSOLVER_VOCAB){
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
		//
		_glWin->_pointCoord[0] = point[0];
		_glWin->_pointCoord[1] = point[1];
		_glWin->_pointCoord[2] = point[2];
		_glWin->_rotMatrix[0][0] = rotMat[0][0];
		_glWin->_rotMatrix[0][1] = rotMat[0][1];
		_glWin->_rotMatrix[0][2] = rotMat[0][2];
		_glWin->_rotMatrix[1][0] = rotMat[1][0];
		_glWin->_rotMatrix[1][1] = rotMat[1][1];
		_glWin->_rotMatrix[1][2] = rotMat[1][2];
		_glWin->_rotMatrix[2][0] = rotMat[2][0];
		_glWin->_rotMatrix[2][1] = rotMat[2][1];
		_glWin->_rotMatrix[2][2] = rotMat[2][2];
	}
	return;
}

double WiiPortCtrlBotsReader::normVector(double newVector[3]){
	double result = 0;
	result = sqrt(pow(newVector[0],2)+pow(newVector[1],2)+pow(newVector[2],2));
	return result;
}