#pragma once

#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <cmath>

#define VERY_LITTLE 0.001

#define MAX_CALIBRATION_DATA 60.0 //amount of samples of wm+ data to get an average
#define MAX_CALIBRATION_DIFF 15

#define MOTIONPLUS_NOT_CALIBRATED 0
#define MOTIONPLUS_CALIBRATING 1

using namespace std;
using namespace yarp::os;
using namespace yarp::sig;

class MotionPlusUtils
{
public:
	MotionPlusUtils(void);
	~MotionPlusUtils(void);
	double calibrateMP(Vector mpRaw);
	Vector getAngle(bool rollLowSpeed, bool pitchLowSpeed, bool yawLowSpeed, Vector mpRaw);
	double getCalibrationStatus();
	void recalibrate();
private:
	double _calibration_status;
	double _currentTime;
	Vector _calibratedMPValue;
	Vector _lastMPRawValue;
	void initVars();
};
