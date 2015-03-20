#pragma once

#include <cmath>
#include "iCubCtrl.h"

class iCubMotorCtrl
{
public:
	iCubMotorCtrl(iCubCtrl *icub);
	~iCubMotorCtrl(void);
	void setAsLeft();
	void setAsRight();
	void toggleActivePart(int partID);
	void setAcceleration(double acceleration);
	double getAcceleration();
	void setSpeed(double speed);
	double getSpeed();
	void setSpeedMultiplier(double speedmultiplier);
	double getSpeedMultiplier();
	void velocityMoveActivePart(double angle1,double angle2,double angle3);
	void resetSavedAngle();
	void stopMotion();
private:
	iCubCtrl *_icub;
	bool _areJointsValid, _isLeft;
	double _maxspeed,_maxacceleration,_speedmultiplier,_angle[3];
	int _actJoints[3],_angleMultiplier[3];
	void initVars();
	void velocityMove(int jointID, double acceleration, double startSpeed);
};
