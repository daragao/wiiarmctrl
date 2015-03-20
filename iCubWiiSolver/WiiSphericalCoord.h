#pragma once
#include <yarp/sig/all.h>

#include <yarp/sig/Vector.h>
#include <yarp/sig/Matrix.h>
#include <yarp/math/Math.h>

#include <string>

#include "Utils.h"

#define PHI_INDEX 0 
#define THETA_INDEX 2
#define AXISANGLE_INDEX 1

using namespace std;

using namespace yarp::math;
using namespace yarp::sig;

class WiiSphericalCoord
{
public:
	WiiSphericalCoord(double firstPoint[3]);
	~WiiSphericalCoord(void);
	void updateMotionPlusData(double status, double xRate, double yRate, double zRate, double xAngle, double yAngle, double zAngle);
	void setNewPosition(double angle, double xPos, double yPos, double zPos);
	void setNewPosition(double xPos, double yPos, double zPos);
	void setNewRotationMatrix(double rotMatrix[3][3]);
	Vector getPointCoord();
	Vector getAxisAngle();
	Vector getLastRotationMatrix();
private:
	double _calibStatus,_radius, _lastRotMatrix[3][3], _firstPoint[3];
	Vector _mpAngle,_mpAngleRate, _pointCoord;
	void initVars();
	Vector getSphericalCoord(double radius, double phi, double theta);
	void updateSphericalCoordYarpMatrix();
	//UTILS
	string getMatrixStr(Matrix tempMatrix);
};
