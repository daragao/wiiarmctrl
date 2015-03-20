#include <yarp/dev/DeviceDriver.h>
#include <yarp/sig/Vector.h>

#define MAX_WIIMOTES 4

/*! \file WiimoteInterface.h define interfaces for the wiimote*/

namespace yarp {
    namespace dev {
        class IWiimote;
    }
}

/**
 * @ingroup dev_iface_other
 *
 * Interface for wiimote, setting and reading status.
 */
class yarp::dev::IWiimote
{
public:
	enum PARTS_ENUM { MOTION_PLUS, MONTION_SENSE, INFRA_RED};
	enum LEDS_ENUM { LED_NONE = 0x00, LED_ONE = 0x10, LED_TWO = 0x20, LED_THREE = 0x40, LED_FOUR = 0x80 };
	enum WIIBOTS_ENUM { A, B, HOME, ONE, TWO, UP, DOWN, RIGHT, LEFT, PLUS, MINUS };

    virtual ~IWiimote(){}

	/* Read a vector from the sensor.
	* @param out a vector containing the sensor's last readings.
	* @param wiimoteNum a int representing the wiimote that we want to get data from.
	* @return true/false success/failure
	**/
	virtual bool initController () = 0;
	virtual long getSequenceNumber(int wiimoteNum) = 0;
	virtual bool readGForce(int wiimoteNum, yarp::sig::Vector &out) = 0;
	virtual bool readAcc(int wiimoteNum, yarp::sig::Vector &out) = 0;
	virtual bool readIR(int wiimoteNum, yarp::sig::Vector &out) = 0;
	virtual bool readMotionPlus (int wiimoteNum, yarp::sig::Vector &out) = 0;
	virtual bool readDiffMotionPlus (int wiimoteNum, yarp::sig::Vector &out) = 0;
	virtual bool readMotionPlusAngle (int wiimoteNum, yarp::sig::Vector &out) = 0;
	virtual bool wiiSetParts(int wiimoteNum, int status) = 0;
	virtual bool wiiSetParts(int wiimoteNum, int status, PARTS_ENUM part) = 0;
	virtual bool wiiGetBotState(int wiimoteNum, WIIBOTS_ENUM bot) = 0;
	virtual bool wiiSetRumble(int wiimoteNum, bool state) = 0;
	virtual bool wiiSetLed(int wiimoteNum, int led) = 0;
	virtual float getBateryLevel(int wiimoteNum) = 0;
	virtual int isMotionPlusLowSpeed(int wiimoteNum) = 0;

	virtual bool readMotionPlusCalibrated (int wiimoteNum, yarp::sig::Vector &out) = 0;
	virtual float calibrateMotionPlus(int wiimoteNum) = 0;
	virtual double getMotionPlusHorizontalAngle(int wiimoteNum) = 0;

	virtual bool close () = 0;

	virtual bool wiiRefresh() = 0;

protected:
	struct wiiData {
		long sequenceNum;
		bool connected;
		bool botA;
		bool botB;
		bool botUp;
		bool botDown;
		bool botRight;
		bool botLeft;
		bool botMinus;
		bool botPlus;
		bool botHome;
		bool bot1;
		bool bot2;
		yarp::sig::Vector gForce;
		yarp::sig::Vector accCoord;
		yarp::sig::Vector accOri;
		yarp::sig::Vector irCoord;
		yarp::sig::Vector mpCoord;
		yarp::sig::Vector mpDiffCoord;
		yarp::sig::Vector mpAngle;
		float battery_level;
		int motionPlusSpeed;
		bool motionPlusLowSpeedPitch;
		bool motionPlusLowSpeedRoll;
		bool motionPlusLowSpeedYaw;
		double motionplus_horiz_rotation;
	} wiimoteData[MAX_WIIMOTES];
};

//
