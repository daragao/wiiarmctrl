#include "ModuleCtrl.h"

ModuleCtrl::ModuleCtrl(void)
{
	initVars();
	setName("KinectCtrl_Module");
}

ModuleCtrl::~ModuleCtrl(void)
{
}

void ModuleCtrl::initVars(){
	_icubCtrl = NULL;
	_kinectCtrl = NULL;
	_testON = false;
}

double ModuleCtrl::getPeriod (){
	return 0.005;
}

bool ModuleCtrl::respond (const Bottle &command, Bottle &reply){
	if(command.size()<=0) return true;
	if(command.get(0).asString() == "test"){
		cout << "Test started!" << endl;
		_testON = true;
	}
	if(command.get(0).asString() == "start"){
	}
	if(command.get(0).asString() == "stop"){
		if(_icubCtrl!=NULL) _icubCtrl->stopAll();
	}
	return true;
}

bool ModuleCtrl::updateModule (){
	//cout << "updateModule()" << endl;
	doTest();
	_kinectCtrl->update();
	for(int i = 0; i < MAX_USERS; i++){
		kinect2iCub(i);
	}
	return true;
}

bool ModuleCtrl::close (){
	cout << "close()" << endl;
	if(_icubCtrl!=NULL) _icubCtrl->close();
	if(_kinectCtrl!=NULL) _kinectCtrl->close();
	return true;
}

bool ModuleCtrl::open (Searchable &config){
	cout << "open()" << endl;

	string localPortName = "/kinect_ctrl";
	string robotPortName = "/icubSim";
	if(config.check("localPort")) localPortName = config.find("localPort").asString();
	if(config.check("robotPortName")) localPortName = config.find("robotPortName").asString();
	//start polydriver interfaces
	_icubCtrl = new iCubCtrl(localPortName,robotPortName);
	_kinectCtrl = new KinectCtrl(localPortName,true);

	return true;
}

/****************************************************************************
***********************************KINECT_DATA*******************************
****************************************2************************************
**************************************iCub***********************************
****************************************************************************/

bool ModuleCtrl::kinect2iCub(int userID){
	bool success = true;
	double rad2deg = 180.0/PI;
	Matrix waistMatrix, torsoMatrix, headMatrix, leftShoulderMatrix, leftElbowMatrix, rightShoulderMatrix, rightElbowMatrix;
	Vector waistAngles,torsoAngles,headAngles, leftShoulderAngles, leftElbowAngles, rightShoulderAngles, rightElbowAngles;

	if(!_kinectCtrl->isTracking(userID)) return success;

	waistMatrix.resize(3,3);
	_kinectCtrl->getOrientationWaist(userID,waistMatrix);//WAIST
	waistAngles.resize(3);
	waistAngles = matrix2Angles(waistMatrix);
	// TODO: CONTROL MOTORS
	torsoMatrix.resize(3,3);
	if(_kinectCtrl->getOrientationTorso(userID,torsoMatrix)){//TORSO
		torsoAngles.resize(3);
		//torsoAngles = matrix2Angles(waistMatrix,torsoMatrix);
		torsoAngles = matrix2Angles(torsoMatrix);
		//CONTROL MOTORS
		_icubCtrl->moveTorso(0,(torsoAngles[1]*rad2deg));
		_icubCtrl->moveTorso(1,-(torsoAngles[2]*rad2deg));
		_icubCtrl->moveTorso(2,(torsoAngles[0]*rad2deg));

		headMatrix.resize(3,3);
		if(true || _kinectCtrl->getOrientationHead(userID,headMatrix)){//HEAD
			headAngles.resize(3);
			headAngles = matrix2Angles(torsoMatrix,headMatrix);
		}
		leftShoulderMatrix.resize(3,3);
		if(_kinectCtrl->getOrientationLeftShoulder(userID,leftShoulderMatrix)){//LEFT SHOULDER
			leftShoulderAngles.resize(3);
			leftShoulderAngles = matrix2Angles(torsoMatrix,leftShoulderMatrix);
			//CONTROL MOTORS
			_icubCtrl->moveLeftArm(0,-(leftShoulderAngles[1]*rad2deg));
			_icubCtrl->moveLeftArm(1,90+(leftShoulderAngles[2]*rad2deg));
			_icubCtrl->moveLeftArm(2,(leftShoulderAngles[0]*rad2deg));

			leftElbowMatrix.resize(3,3);
			if(_kinectCtrl->getOrientationLeftElbow(userID,leftElbowMatrix)){//LEFT ELBOW
				leftElbowAngles.resize(3);
				leftElbowAngles = matrix2Angles(leftShoulderMatrix,leftElbowMatrix);
				//CONTROL MOTORS
				_icubCtrl->moveLeftArm(3,(leftElbowAngles[1]*rad2deg));
			}
		}
		rightShoulderMatrix.resize(3,3);
		if(_kinectCtrl->getOrientationRightShoulder(userID,rightShoulderMatrix)){//RIGHT SHOULDER
			rightShoulderAngles.resize(3);
			rightShoulderAngles = matrix2Angles(torsoMatrix,rightShoulderMatrix);
			//CONTROL MOTORS
			_icubCtrl->moveRightArm(0,(rightShoulderAngles[1]*rad2deg));
			_icubCtrl->moveRightArm(1,90-(rightShoulderAngles[2]*rad2deg));
			_icubCtrl->moveRightArm(2,(rightShoulderAngles[0]*rad2deg));

			rightElbowMatrix.resize(3,3);
			if(_kinectCtrl->getOrientationRightElbow(userID,rightElbowMatrix)){//RIGHT ELBOW
				rightElbowAngles.resize(3);
				rightElbowAngles = matrix2Angles(rightShoulderMatrix,rightElbowMatrix);
				//CONTROL MOTORS
				_icubCtrl->moveRightArm(3,-(rightElbowAngles[1]*rad2deg));
			}
		}
	}
	return success;
}

inline Vector ModuleCtrl::matrix2Angles(Matrix matrixParent, Matrix matrixChild){
	Matrix relMatrix = matrixChild * matrixParent.transposed();
	return matrix2Angles(relMatrix);
}

inline Vector ModuleCtrl::matrix2Angles(Matrix matrix){
	Vector result(3);
	/*result[2] = matrix2AnglesXYZ(matrix)[2];
	result[0] = matrix2AnglesYZX(matrix)[0];
	result[1] = matrix2AnglesZXY(matrix)[1];*/
	result = matrix2AnglesZYX(matrix);
	result[0] = matrix2AnglesYZX(matrix)[0];
	return result;
}

inline Vector ModuleCtrl::matrix2AnglesXYZ(Matrix matrix){
	Vector result(3);
	// Assuming the angles are in radians.
	//RxRyRz
	double thetaX,thetaY,thetaZ;
	if (matrix[0][2] < 1) {
		if (matrix[0][2] > -1){
			thetaY = asin(matrix[0][2]);
			thetaX = atan2(-matrix[1][2],matrix[2][2]);
			thetaZ = atan2(-matrix[0][1],matrix[0][0]);
		}else{// matrix[0][2] == -1	
			// Not a unique solution: thetaZ - thetaX = atan2(r10,r11)
			thetaY = -PI/2.0;
			thetaX = -atan2(matrix[1][0],matrix[1][1]);
			thetaZ = 0;
		}
	}else{// matrix[0][2] == 1
		// Not a unique solution: thetaZ + thetaX = atan2(r10,r11)
		thetaY = +PI/2.0;
		thetaX = atan2(matrix[1][0],matrix[1][1]);
		thetaZ = 0.0;
	}
	result[0] = thetaX;
	result[1] = thetaY;
	result[2] = thetaZ;
	
	return result;
}

Vector ModuleCtrl::matrix2AnglesXZY(Matrix rMatrix){

	Vector eulerAngles(3);
	// Assuming the angles are in radians.
	//RxRzRy
	double thetaX,thetaY,thetaZ;
	if (rMatrix[0][1] < 1) {
		if (rMatrix[0][1] > -1){

			thetaZ = asin(-rMatrix[0][1]);
			thetaX = atan2(rMatrix[2][1],rMatrix[1][1]);
			thetaY = atan2(rMatrix[0][2],rMatrix[0][0]);
		}else{// rMatrix[0][1] == -1	
			// Not a unique solution: thetaZ - thetaX = atan2(r10,r11)
			thetaZ = +PI/2.0;
			thetaX = atan2(-rMatrix[2][0],rMatrix[2][2]);
			thetaY = 0;
		}
	}else{// rMatrix[0][1] == 1

		// Not a unique solution: thetaZ + thetaX = atan2(r10,r11)
		thetaZ = -PI/2.0;
		thetaX = atan2(-rMatrix[2][0],rMatrix[2][2]);
		thetaY = 0;
	}
	eulerAngles[0] = thetaX;
	eulerAngles[1] = thetaY;
	eulerAngles[2] = thetaZ;
	return eulerAngles;
}


Vector ModuleCtrl::matrix2AnglesYZX(Matrix rMatrix){
	Vector eulerAngles(3);
	// Assuming the angles are in radians.
	//RyRzRx
	double thetaX,thetaY,thetaZ;
	if (rMatrix[1][0] < 1) {
		if (rMatrix[1][0] > -1){

			thetaZ = asin(rMatrix[1][0]);
			thetaY = atan2(-rMatrix[2][0],rMatrix[0][0]);
			thetaX = atan2(-rMatrix[1][2],rMatrix[1][1]);
		}else{// rMatrix[1][0] == -1	
			// Not a unique solution: thetaZ - thetaX = atan2(r10,r11)
			thetaZ = -PI/2.0;
			thetaY = -atan2(rMatrix[2][1],rMatrix[2][2]);
			thetaX = 0;
		}
	}else{// rMatrix[1][0] == 1

		// Not a unique solution: thetaZ + thetaX = atan2(r10,r11)
		thetaZ = +PI/2.0;
		thetaY = atan2(rMatrix[2][1],rMatrix[2][2]);
		thetaX = 0;
	}
	eulerAngles[0] = thetaX;
	eulerAngles[1] = thetaY;
	eulerAngles[2] = thetaZ;
	return eulerAngles;
}

Vector ModuleCtrl::matrix2AnglesZXY(Matrix rMatrix){
	Vector eulerAngles(3);
	// Assuming the angles are in radians.
	//RzRxRy
	double thetaX,thetaY,thetaZ;
	if (rMatrix[2][1] < 1) {
		if (rMatrix[2][1] > -1){

			thetaX = asin(rMatrix[2][1]);
			thetaZ = atan2(-rMatrix[0][1],rMatrix[1][1]);
			thetaY = atan2(-rMatrix[2][0],rMatrix[2][2]);
		}else{// rMatrix[2][1] == -1	
			// Not a unique solution: thetaZ - thetaX = atan2(r10,r11)
			thetaX = -PI/2.0;
			thetaZ = -atan2(rMatrix[0][2],rMatrix[0][0]);
			thetaY = 0;
		}
	}else{// rMatrix[2][1] == 1

		// Not a unique solution: thetaZ + thetaX = atan2(r10,r11)
		thetaX = +PI/2.0;
		thetaZ = atan2(rMatrix[0][2],rMatrix[0][0]);
		thetaY = 0;
	}
	eulerAngles[0] = thetaX;
	eulerAngles[1] = thetaY;
	eulerAngles[2] = thetaZ;
	return eulerAngles;
}

Vector ModuleCtrl::matrix2AnglesZYX(Matrix rMatrix){
	Vector eulerAngles(3);
	// Assuming the angles are in radians.
	//RzRxRy
	double thetaX,thetaY,thetaZ;
	if (rMatrix[2][0] < 1) {
		if (rMatrix[2][0] > -1){

			thetaY = asin(-rMatrix[2][0]);
			thetaZ = atan2(rMatrix[1][0],rMatrix[0][0]);
			thetaX = atan2(rMatrix[2][1],rMatrix[2][2]);
		}else{// rMatrix[2][0] == -1	
			// Not a unique solution: thetaZ - thetaX = atan2(r10,r11)
			thetaY = +PI/2.0;
			thetaZ = -atan2(-rMatrix[1][2],rMatrix[1][1]);
			thetaX = 0;
		}
	}else{// rMatrix[2][0] == 1

		// Not a unique solution: thetaZ + thetaX = atan2(r10,r11)
		thetaY = -PI/2.0;
		thetaZ = atan2(-rMatrix[1][2],rMatrix[1][1]);
		thetaX = 0;
	}
	eulerAngles[0] = thetaX;
	eulerAngles[1] = thetaY;
	eulerAngles[2] = thetaZ;
	return eulerAngles;
}

/**********************************************************************
*********************************UTILS*********************************
**********************************************************************/

inline void ModuleCtrl::printAngles(Vector anglesV){
	double angleX = anglesV[0]*(180.0/PI);
	double angleY = anglesV[1]*(180.0/PI);
	double angleZ = anglesV[2]*(180.0/PI);
	cout << "X(" << angleX << ")\t" << "Y(" << angleY << ")\t" << "Z(" << angleZ << ")" << endl;
}

inline void ModuleCtrl::printMatrix(Matrix matrix){
	cout << "\t| " << matrix[0][0] << "\t" << matrix[0][1] << "\t" << matrix[0][2] << " |" << endl;
	cout << "\t| " << matrix[1][0] << "\t" << matrix[1][1] << "\t" << matrix[1][2] << " |" << endl;
	cout << "\t| " << matrix[2][0] << "\t" << matrix[2][1] << "\t" << matrix[2][2] << " |" << endl;
}


bool ModuleCtrl::doTest(){
	if(_testON){
		if(_icubCtrl->moveLeftArm(0,-90) &&
			_icubCtrl->moveLeftArm(1,90) &&
			_icubCtrl->moveLeftArm(2,0)){
				cout << "Test ended!" << endl;
			_testON = false;
		}else cout << endl;
		return true;
	}
	return false;
}