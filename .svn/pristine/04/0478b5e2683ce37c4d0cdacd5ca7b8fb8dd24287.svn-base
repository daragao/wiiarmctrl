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
	HEAD,
	SHOULDERS
}PartName;

typedef enum SkeletonJoint
{
	SKEL_HEAD			= 0,
	SKEL_NECK			= 1,
	SKEL_TORSO			= 2,
	SKEL_WAIST			= 3,

	SKEL_LEFT_COLLAR	= 4,
	SKEL_LEFT_SHOULDER	= 5,
	SKEL_LEFT_ELBOW		= 6,
	SKEL_LEFT_WRIST		= 7,//NO ORIENTATION
	SKEL_LEFT_HAND		= 8,//NO ORIENTATION
	SKEL_LEFT_FINGERTIP	= 9,//NO ORIENTATION

	SKEL_RIGHT_COLLAR	=10,
	SKEL_RIGHT_SHOULDER	=11,
	SKEL_RIGHT_ELBOW	=12,
	SKEL_RIGHT_WRIST	=13,//NO ORIENTATION
	SKEL_RIGHT_HAND		=14,//NO ORIENTATION
	SKEL_RIGHT_FINGERTIP=15,//NO ORIENTATION

	SKEL_LEFT_HIP		=16,
	SKEL_LEFT_KNEE		=17,
	SKEL_LEFT_ANKLE		=18,//NO ORIENTATION
	SKEL_LEFT_FOOT		=19,//NO ORIENTATION

	SKEL_RIGHT_HIP		=20,
	SKEL_RIGHT_KNEE		=21,
	SKEL_RIGHT_ANKLE	=22,//NO ORIENTATION
	SKEL_RIGHT_FOOT		=23	//NO ORIENTATION
} SkeletonJoint;


class iCubGeneric
{
public:
	typedef struct ICUB_DATA{
		long lastBottleTimestamp;
		Vector handRealPos;
		Vector handProjPos;
		Vector jointsPos[24];
		Matrix jointsOri[24];
	} iCubData;
	iCubGeneric(void);
	~iCubGeneric(void);
	static void updateTimestamp();
	static void setHandRealPos(double x, double y, double z);
	static void setHandProjPos(double x, double y, double z);
	static void setJointPos(int index, double x, double y, double z);
	static void setJointOri(int index, double m00, double m01, double m02, double m10, double m11, double m12,double m20, double m21, double m22);
	virtual void updatePosition(bool isTracking) = 0;
	virtual void close() = 0;
protected:	
	static iCubData *_icubData;
	void initVars();
};
