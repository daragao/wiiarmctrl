#pragma once

#include <time.h>
#include <stdio.h>

#include <yarp/sig/Vector.h>

#include <yarp/math/Math.h>

using namespace yarp::sig;
using namespace yarp;

using namespace yarp::math;

typedef enum PART_NAME{
	RIGHT_ARM,
	LEFT_ARM,
	TORSO,
	RIGHT_LEG,
	LEFT_LEG
}PartName;

class iCubGeneric
{
public:
	typedef struct ICUB_DATA{
		long lastBottleTimestamp;
		int controlID;
		Vector solverVector;
		Vector mpRawVector;
		Vector mpAngleVector;
		Matrix solverRotMatrix;
	} iCubData;
	iCubGeneric(void);
	~iCubGeneric(void);
	static void updateTimestamp();
	static void setControlID(int id);
	static void setMPRawVector(double x, double y, double z);
	static Vector getMPRawVector();
	static void setMPAngleVector(double x, double y, double z);
	static Vector getMPAngleVector();
	static void setSolverVector(double x, double y, double z);
	static void setSolverMatrix(double r00, double r01, double r02, double r10, double r11, double r12, double r20, double r21, double r22);
	virtual void updatePosition(bool isTracking) = 0;
	virtual void close() = 0;
protected:	
	static iCubData *_icubData;
	void initVars();
};
