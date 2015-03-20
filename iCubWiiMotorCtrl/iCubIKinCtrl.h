#pragma once

#include "Utils.h"
#include "iCubCtrl.h"

class iCubIKinCtrl
{
public:
	iCubIKinCtrl(iCubCtrl *icub);
	~iCubIKinCtrl(void);
	void movePositionWithOrientation(double position[3],double axisAngle[4]);
	void movePosition( double x, double y, double z);
	void stop();
	void kinecticMoveActivePart(double position[3],double axisAngle[4],double solverPoint[3], double rotMAtrix[3][3]);
	void getCartesianPose(double posArray[3], double oriArray[4]);
	void setActivePart(int partID);
	bool isOrientationActive();
	void setMinCartTrajTime(double seconds);
	void addToRadius(double meters);
	double getRadius();
	void setRadius(double newRadius);
	void cartesianDesired(double pos[3], double ori[4]);
	void getShoulderPosition(double shoulderPosition[3]);
private:
	iCubCtrl *_icub;
	double _minCartTrajTime, _radius, _shoulderPosition[3];
	bool _movePosition;
	void setPartsOn();
	void move(bool withOrientation, double angle, double oriX, double oriY, double oriZ, double x, double y, double z);
};
