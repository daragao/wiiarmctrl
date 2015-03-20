#include "iCubCtrl.h"

PolyDriver *iCubCtrl::_clientCartCtrl;
ICartesianControl *iCubCtrl::_armCart;
Vector iCubCtrl::_icubCoords;

iCubCtrl::iCubCtrl(void)
{

}

iCubCtrl::~iCubCtrl(void)
{
	if(_clientCartCtrl!=NULL)
		_clientCartCtrl->close();
	printf("iCubCtrl: closing iCubCtrl!\n");
}

void iCubCtrl::init_iCubCtrl(){
	initVariables();
	openCartInterface(_clientCartCtrl, _armCart);
	_armCart->setTrackingMode(false);
}

void iCubCtrl::initVariables(){
	_clientCartCtrl = NULL;
	_armCart = NULL;
	//xyz and angle xyz
	_icubCoords.resize(3+4);
	_icubCoords.zero();
}

void iCubCtrl::openCartInterface(PolyDriver*& clientCartCtrl, ICartesianControl*& armCart){
	Property option;
	option.put("device","cartesiancontrollerclient");
	option.put("remote","/icubSim/cartesianController/right_arm");
	option.put("local","/client/right_arm");

	clientCartCtrl = new PolyDriver(option);

	armCart=NULL;

	if (clientCartCtrl->isValid()) {
		clientCartCtrl->view(armCart);
	}else printf("ERROR:openCartInterface(): clientCartCtrl is not valid!\n");
}

void iCubCtrl::setPose(double x, double y, double z){
	if(_armCart==NULL) return;
	_armCart->setTrajTime(1.5);
	Vector xd;
	xd.resize(3);
	xd[0] = x;
	xd[1] = y;
	xd[2] = z;
	Vector od;
	od.resize(4);
	od.zero();
	setPose(_armCart, xd, od);
}

void iCubCtrl::setPose(ICartesianControl *armCart, Vector xd, Vector od){
	armCart->setTrajTime(0.5);
	//armCart->goToPose(xd,od);
	armCart->goToPosition(xd,0.5);
}

void iCubCtrl::setPose(Vector point){
	Vector od;
	od.resize(4);
	od.zero();
	double newPoint[3] = {0,0,0};
	Utils::normalizeVector(newPoint,point.data());
	newPoint[0] *= 15;
	newPoint[1] *= 15;
	newPoint[2] *= 15;
	point[0] = newPoint[0];
	point[1] = newPoint[1];
	point[2] = newPoint[2];
	setPose(_armCart,point,od);
}

void iCubCtrl::getPose(Vector &xd, Vector &od){
	getPose(_armCart,xd,od);
}

void iCubCtrl::getPose(ICartesianControl *armCart, Vector &xd, Vector &od){
	armCart->getPose(xd,od);
}

void iCubCtrl::test(){
	Vector xd;
	xd.resize(3);
	xd.zero();
	Vector od;
	od.resize(4);
	od.zero();
	_armCart->getPose(xd,od);
	setPose(_armCart, xd, od);
}

void iCubCtrl::refresh_iCub(){
	Vector xd;
	xd.resize(3);
	xd.zero();
	Vector od;
	od.resize(4);
	od.zero();
	getPose(xd,od);
	_icubCoords[0] = xd[0];
	_icubCoords[1] = xd[1];
	_icubCoords[2] = xd[2];
	_icubCoords[3] = od[0];
	_icubCoords[4] = od[1];
	_icubCoords[5] = od[2];
	_icubCoords[6] = od[3];
}

double iCubCtrl::desirableDistance(Vector xd){
	ICartesianControl *armCart = _armCart;
	//Vector xd = _icubCoords;
	Vector xdd,odd,qdd;
	armCart->getDesired(xdd,odd,qdd);
	double diffX[3],normXD[3], normXDD[3];
	Utils::normalizeVector(normXD,xd.data());
	Utils::normalizeVector(normXDD,xdd.data());
	diffX[0] = normXD[0]-normXDD[0];
	diffX[1] = normXD[0]-normXDD[1];
	diffX[2] = normXD[0]-normXDD[2];
	double distance = Utils::normVector(diffX);
	return distance;
}