#include <yarp/os/all.h>
#include <yarp/dev/all.h>
#include "wiiuse/wiiuse.h"
#include <yarp/dev/WiimoteInterface.h>
#include <yarp/os/Time.h>
#include <math.h>

using namespace yarp::dev;
using namespace yarp::sig;
using namespace yarp::os;

#define TWO_PI (2*3.14159265)
#define DEG_TO_RAD (TWO_PI/360)

#define MAX_CALIBRATION_DATA 60 //amount of samples of wm+ data to get an average
#define MAX_CALIBRATION_DIFF 20

#define MOTIONPLUS_OFF 0
#define MOTIONPLUS_NOT_CALIBRATED 1
#define MOTIONPLUS_CALIBRATING 2

#define MOTIONPLUS_OK (MOTIONPLUS_CALIBRATING + MAX_CALIBRATION_DATA - 1) //61

#define PITCH_LOW_SPEED 0x1
#define YAW_LOW_SPEED	0x2
#define ROLL_LOW_SPEED	0x4
#define LOW_SPEED (PITCH_LOW_SPEED | YAW_LOW_SPEED | ROLL_LOW_SPEED)

#define VERY_LITTLE 0.001

namespace yarp {
	namespace dev
	{
		class wiimoteController;
	}
}

class yarp::dev::wiimoteController : public IWiimote, public DeviceDriver
{
public:
	/**
	* Default constructor.
	*/
	wiimoteController();

	/**
	* Destructor.
	*/
	~wiimoteController();

	/**
	* Open the device driver.
	* @param config is a reference to a Searchable object which contains the initialization
	* parameters.
	* @return true/false on success/failure.
	*/
	virtual bool open (yarp::os::Searchable& config);

	/**
	* Close the device driver.
	* @return true/false on success/failure.
	*/
	virtual bool close ();


	//IWiimotte
	/* Read a vector from the sensor.
	* @param out a vector containing the sensor's last readings.
	* @param wiimoteNum a int representing the wiimote that we want to get data from.
	* @return true/false success/failure
	**/
	virtual bool initController ();
	virtual long getSequenceNumber(int wiimoteNum);
	virtual bool readGForce(int wiimoteNum, yarp::sig::Vector &out);
	virtual bool readAcc(int wiimoteNum, yarp::sig::Vector &out);
	virtual bool readIR(int wiimoteNum, yarp::sig::Vector &out);
	virtual bool readMotionPlus (int wiimoteNum, yarp::sig::Vector &out);
	virtual bool readDiffMotionPlus (int wiimoteNum, yarp::sig::Vector &out);
	virtual bool readMotionPlusAngle (int wiimoteNum, yarp::sig::Vector &out);
	virtual bool wiiSetParts(int wiimoteNum, int status);
	virtual bool wiiSetParts(int wiimoteNum, int status, PARTS_ENUM part);
	virtual bool wiiGetBotState(int wiimoteNum, WIIBOTS_ENUM bot);
	virtual bool wiiSetRumble(int wiimoteNum, bool state);
	virtual bool wiiSetLed(int wiimoteNum, int led);
	virtual float getBateryLevel(int wiimoteNum);
	virtual int isMotionPlusLowSpeed(int wiimoteNum);

	virtual bool readMotionPlusCalibrated (int wiimoteNum, yarp::sig::Vector &out);
	virtual float calibrateMotionPlus(int wiimoteNum);
	virtual double getMotionPlusHorizontalAngle(int wiimoteNum);
	virtual double calculateAngleFromAdjancent(bool isPlus, double angle, double adjGrav, double newGrav, double adjGravNorm, double newGravNorm);

	virtual bool wiiRefresh();
private:
	wiimote** _wiimotes;
	int _wiimotesFound;
	double _initTime, _lastRefreshTime, _lastMotionPlusTime;
	void zeroVariables ();
	void handle_event(int wiimoteNum);
	void calculateMotionPlusAngle(int wiimoteNum);
	double getNormalGravity(int wiimoteNum, Vector &gravity);
	//MOTION PLUS CALIBRATION
	int _motionplus_state;
	double _motionplus_calibration_data[MAX_WIIMOTES][3][MAX_CALIBRATION_DATA];
	double _gforce_calibration_data[MAX_WIIMOTES][3][MAX_CALIBRATION_DATA];
	//int calibrateMotionPlus(int wiimoteNum);
};
