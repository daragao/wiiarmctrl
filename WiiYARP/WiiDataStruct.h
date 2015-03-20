#include <yarp/sig/Vector.h>

using namespace yarp::sig;

struct WiiDataStruct{
	long eventNum;
	bool isConnected;
	bool isMotionPlusOn;
	double motionPlusCalibrationStatus;
	Vector motionPlusRaw;
	Vector motionPlusAngle;
	bool isIROn;
	bool isIRVisible[4];
	Vector irSource[4];
	Vector irCursor;
	bool isAccelerometerOn;
	Vector accGForce;
	Vector accOrientation;
	bool isLEDOn[4];
	bool isRumbleOn;
	float battery_level;
	bool botA;
	bool botB;
	bool botUP;
	bool botDOWN;
	bool botLEFT;
	bool botRIGHT;
	bool botMINUS;
	bool botPLUS;
	bool botONE;
	bool botTWO;
	bool botHOME;
};