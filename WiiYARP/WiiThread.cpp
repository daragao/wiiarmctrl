#include "WiiThread.h"

WiiThread::WiiThread(Wiimote *wm, WiiPortCtrl **wiiPorts, double threadPeriod):RateThread(int(1000*threadPeriod)){
	_wm = wm;
	_wiiPorts = wiiPorts;
}

WiiThread::~WiiThread(void){
}

bool WiiThread::threadInit(){
	printf("Starting WiiThread\n");
	Wiimote *wm = _wm;
	return true;
}

void WiiThread::run(){
	static long eventNum = -1;
	Wiimote *wm = _wm;
	int totalWiimotes = wm->getTotalConnectWiimotes();
	wm->poll();
	//cycle all the wiimotes found
	for(int id = 0; id < totalWiimotes; id++){
		WiiPortCtrl *wiiPorts = _wiiPorts[id];
		WiiDataStruct *wiiData = wm->getWiiDataStruct(id);
		if(wiiData->eventNum!=eventNum){
			if(wiiData->isAccelerometerOn){
				//[ACC] gForceX gForceY gForceZ orientationX orientationY orientationZ
				wiiPorts->writeAccData(wiiData->accGForce,wiiData->accOrientation);
			}
			if(wiiData->isIROn){
				// for each visible IRSource {irIndex irSourceX irSourceY irSourceZ}
				//[IR] totalVisibleIRSources {irIndex irSourceX irSourceY irSourceZ} irCursorX irCursorY irSize
				wiiPorts->writeIRData(wiiData->isIRVisible,wiiData->irSource,wiiData->irCursor);
			}
			if(wiiData->isMotionPlusOn){
				//[MP] mpCalibStatus mpRawX mpRawY mpRawZ
				wiiPorts->writeMotionPlusData(wiiData->motionPlusCalibrationStatus,wiiData->motionPlusRaw,wiiData->motionPlusAngle);
			}
			eventNum = wiiData->eventNum;
		}

		//vocabs can be consulted in the WiiPort.h file
		//[BOT] vocabs for active buttons
		wiiPorts->writeBotState(wiiData->botA,wiiData->botB,wiiData->botUP,wiiData->botDOWN,wiiData->botLEFT,
			wiiData->botRIGHT,wiiData->botMINUS,wiiData->botPLUS,wiiData->botONE,wiiData->botTWO,wiiData->botHOME);
		//[STAT] eventSequenceNumber battery_level totalLEDsON ledsVOCAB
		wiiPorts->writeStatusData(wiiData->eventNum,wiiData->isConnected,wiiData->isLEDOn,wiiData->battery_level);

		//update the data from reading port
		portReaderUpdate(id,wm,wiiPorts->getWiiPortCtrlReader());
	}
}

void WiiThread::portReaderUpdate(int id, Wiimote *wm, WiiPortCtrlReader *wiiPortReader){
	WiiDataStruct *wiiData = wm->getWiiDataStruct(id);
	if(wiiData->isRumbleOn!=wiiPortReader->getRumble()){
		wm->setRumble(id,wiiPortReader->getRumble());
		wiiData->isRumbleOn = wiiPortReader->getRumble();
	}
	if( wiiData->isLEDOn[0]!=wiiPortReader->getLED(1)||wiiData->isLEDOn[1]!=wiiPortReader->getLED(2)||
		wiiData->isLEDOn[2]!=wiiPortReader->getLED(3)||wiiData->isLEDOn[3]!=wiiPortReader->getLED(4)){
			int ledValue = wiiPortReader->getLED(1)?WIIMOTE_LED_1:0;
			ledValue += wiiPortReader->getLED(2)?WIIMOTE_LED_2:0;
			ledValue += wiiPortReader->getLED(3)?WIIMOTE_LED_3:0;
			ledValue += wiiPortReader->getLED(4)?WIIMOTE_LED_4:0;
			wm->setLEDs(id,ledValue);
			wiiData->isLEDOn[0] = wiiPortReader->getLED(1);
			wiiData->isLEDOn[1] = wiiPortReader->getLED(2);
			wiiData->isLEDOn[2] = wiiPortReader->getLED(3);
			wiiData->isLEDOn[3] = wiiPortReader->getLED(4);
	}
	if(wiiData->isAccelerometerOn!=wiiPortReader->getAccelerometer()){
		wm->turnOnPart(id,WIIMOTE_ACCELEROMETER,wiiPortReader->getAccelerometer());
	}
	if(wiiData->isIROn!=wiiPortReader->getIR()){
		wm->turnOnPart(id,WIIMOTE_IR,wiiPortReader->getIR());
	}
	if(wiiData->isMotionPlusOn!=wiiPortReader->getMotionPlus()){
		wm->turnOnPart(id,WIIMOTE_MOTIONPLUS,wiiPortReader->getMotionPlus());
	}
}

void WiiThread::threadRelease(){
	printf("Goodbye from WiiThread\n");
}