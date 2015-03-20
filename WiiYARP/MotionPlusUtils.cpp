#include "MotionPlusUtils.h"

MotionPlusUtils::MotionPlusUtils(void)
{
	initVars();
}

MotionPlusUtils::~MotionPlusUtils(void)
{
}

void MotionPlusUtils::initVars(){
	_calibration_status = 0;
	_currentTime = -1;
	_calibratedMPValue.resize(3);
	_calibratedMPValue.zero();
	_lastMPRawValue.resize(3);
	_lastMPRawValue.zero();
}

double MotionPlusUtils::getCalibrationStatus(){
	return _calibration_status/MAX_CALIBRATION_DATA;
}

void MotionPlusUtils::recalibrate(){
	initVars();
}

double MotionPlusUtils::calibrateMP(Vector mpRaw){
	bool isCalibrated = _calibration_status==MAX_CALIBRATION_DATA?true:false;
	if(!isCalibrated){
		if( abs(_lastMPRawValue[0]-mpRaw[0])>MAX_CALIBRATION_DIFF || 
			abs(_lastMPRawValue[1]-mpRaw[1])>MAX_CALIBRATION_DIFF ||
			abs(_lastMPRawValue[2]-mpRaw[2])>MAX_CALIBRATION_DIFF){
				initVars();
				_calibration_status=MOTIONPLUS_NOT_CALIBRATED;
		}else{
			if(_calibration_status==MOTIONPLUS_NOT_CALIBRATED){
				_calibratedMPValue.zero();
				_calibration_status++;
			}
			if(_calibration_status>=MOTIONPLUS_CALIBRATING){
				_calibratedMPValue[0] += mpRaw[0];
				_calibratedMPValue[1] += mpRaw[1];
				_calibratedMPValue[2] += mpRaw[2];
				_calibration_status++;
			}
			if(_calibration_status==MAX_CALIBRATION_DATA){
				_calibratedMPValue[0] /= (MAX_CALIBRATION_DATA-1);
				_calibratedMPValue[1] /= (MAX_CALIBRATION_DATA-1);
				_calibratedMPValue[2] /= (MAX_CALIBRATION_DATA-1);
			}
		}
		_lastMPRawValue = mpRaw;
	}
	return getCalibrationStatus();
}

Vector MotionPlusUtils::getAngle(bool rollLowSpeed, bool pitchLowSpeed, bool yawLowSpeed, Vector mpRaw){
	Vector angleResult;
	angleResult.resize(3);
	angleResult.zero();
	if(_calibration_status!=MAX_CALIBRATION_DATA)
		return angleResult;

	double oldTime = _currentTime;
	if(oldTime==-1) oldTime = Time::now();
	double currentTime = Time::now();
	double timeDiff = currentTime - oldTime;

	double fast = 2000.0/440.0;
	double slow = 1.0;

	double *avgMPRaw = _calibratedMPValue.data();

	//statement from http://wiibrew.org/wiki/Wiimote/Extension_Controllers (Data Format (Wii Motion Plus))
	//Voltage reference is 1.35V that you can assume it as 8192 unit (half of the ADC range), 
	//using 2.27 mV/deg/s, 8192 is 595 deg/s (1.35V/2.27mV), 
	//you must divide by ~13.768 unit/deg/s (8192/595) to know the correct deg/s.
	double unitDegSec = 1 / (8192.0/595.0);

	double mpVal[3] = {0,0,0};
	mpVal[0] = avgMPRaw[0] - mpRaw[0];
	mpVal[1] = avgMPRaw[1] - mpRaw[1];
	mpVal[2] = avgMPRaw[2] - mpRaw[2];

	angleResult[0] = mpVal[0] * (rollLowSpeed?slow:fast)	*  timeDiff * unitDegSec;
	angleResult[1] = mpVal[1] * (pitchLowSpeed?slow:fast)	*  timeDiff * unitDegSec;
	angleResult[2] = mpVal[2] * (yawLowSpeed?slow:fast)		*  timeDiff * unitDegSec;

	_currentTime = currentTime;
	return angleResult;
}
