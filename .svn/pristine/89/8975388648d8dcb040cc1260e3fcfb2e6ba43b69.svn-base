#include "PortCtrlMod.h"

double PortCtrlMod::getPeriod(){
	return 0.02;
}

bool PortCtrlMod::updateModule(){
	char letter[500];
	string filename;
	std::cout << "<---------MENU--------->\n's' for start\n'f' for finish\n'i' for information\n't' for task\n'b' for break\n'e' for error\n'o' for other\n'h' for home position\n'r' to reconnect\n'q' to quit\n<---------------------->\n-> ";
	cin.getline(letter,500);
	switch(letter[0]){
		case 's':
			filename = ((letter+2));
			if(_pFile == NULL)
				fopen_s(&_pFile,(filename+".txt").c_str(),"a+");
			break;
		case 'b':
			if(_pFile != NULL){
				fputs("<------------------------",_pFile);
				fputs("------------------------>\n",_pFile);
			}
			break;
		case 'h'://goes to home and logs
			if(letter[0]=='h')_icub->goToHome();
		case 'r':
			if(letter[0]=='r') _icub->reconnect();
			if(!Network::isConnected(_wiimotePortName.c_str(),_userTestsPortName.c_str()))
				Network::connect(_wiimotePortName.c_str(),_userTestsPortName.c_str());
		case 't':
			if(_pFile != NULL){
				fputs("<------------------------",_pFile);
				fputs("------------------------>\n",_pFile);
			}
		case 'o':
		case 'i':
		case 'e':
			if(_pFile != NULL){
				writeMiliSeconds();
				fputc('[',_pFile);
				fputc(toupper(letter[0]),_pFile);
				fputc(']',_pFile);
				fputs(letter+1,_pFile);
				fputs("\n",_pFile);
			}
			break;
		case 'f':
			if(_pFile != NULL)
				fclose(_pFile);
			_pFile = NULL;
			break;
		case 'q':
			if(_pFile != NULL)
				fclose(_pFile);
			_icub->close();
			return false;
			break;
		default:
			if(_pFile != NULL){
				writeMiliSeconds();
				fputs("[DEFAULT ERROR]\t",_pFile);
				fputs(letter,_pFile);
				fputs("\n",_pFile);
			}
			break;
	}
	fflush(_pFile);
	return true;
}

bool PortCtrlMod::respond(const Bottle& command, Bottle& reply){
	return true;
}

void PortCtrlMod::onRead(Bottle &command){
	static bool wasBpressed = false;
	bool isBpressed = false;
	static bool wasApressed = false;
	bool isApressed = false;
	static bool wasHpressed = false;
	bool isHpressed = false;
	if(_pFile==NULL) return;
	switch(command.get(0).asVocab()){
		case VOCAB3('B','O','T'):{
			for(int i = 1; i < command.size(); i++){
				switch(command.get(i).asVocab()){
		case VOCAB1('B'):
			isBpressed = true;
			if(!wasBpressed && isBpressed){
				//writeMiliSeconds();
				//fputs("[B PRESS]",_pFile);
				//fputs("\n",_pFile);
			}
			break;
		case VOCAB1('A'):
			isApressed = true;
			if(!wasApressed && isApressed){
				writeMiliSeconds();
				fputs("[A PRESS]",_pFile);
				fputs("\n",_pFile);
			}
			break;
		case VOCAB1('H'):
			isHpressed = true;
			if(!wasHpressed && isHpressed){
				writeMiliSeconds();
				fputs("[H PRESS]",_pFile);
				fputs("\n",_pFile);
			}
			_icub->goToHome();
			break;
				}
			}
			if(wasBpressed && !isBpressed){
				//writeMiliSeconds();
				//fputs("[B RELEASE]",_pFile);
				//fputs("\n",_pFile);
			}
			wasHpressed = isHpressed;
			wasBpressed = isBpressed;
			wasApressed = isApressed;
			break;
								 }
	}
}

bool PortCtrlMod::configure(ResourceFinder &rf){
	_pFile = NULL;

	_userTestsPortName = "/userTestsCtrl";
	_wiimotePortName = "/wiimote1/bots";

	_handlerPort = new BufferedPort<Bottle>();
	_handlerPort->open(_userTestsPortName.c_str());
	_handlerPort->useCallback(*this);

	_icub = new iCubMotorCtrl("userTestsCtrl","icub","left_arm");

	Network::connect(_wiimotePortName.c_str(),_userTestsPortName.c_str());
	return true;
}

bool PortCtrlMod::interruptModule(){
	return true;
}

bool PortCtrlMod::close(){
	return true;
}

void PortCtrlMod::writeTimeStamp(){
	char buffer[80];
	ltime=time(NULL); /* get current cal time */
	strftime(buffer,80,"%c",localtime(&ltime));
	fputs(buffer,_pFile);
}

void PortCtrlMod::writeMiliSeconds(){
	ltime=time(NULL); /* get current cal time */
	string miliseconds;
	ostringstream temp;
	temp << (((double)clock()/(double)CLOCKS_PER_SEC)*1000.0);
	fputs(("("+temp.str()+")\t").c_str(),_pFile);
	writeTimeStamp();
}