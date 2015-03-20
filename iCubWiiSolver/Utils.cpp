#include "Utils.h"

void Utils::axisAngle2Matrix(double rMatrix[3][3], double angle, double x, double y, double z){

	double c = cos(angle);
	double s = sin(angle);
	double t = 1.0 - c;
	// just in case the axis is not already normalised
	double magnitude = sqrt(x*x + y*y + z*z);
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;

	rMatrix[0][0] = c + x*x*t;
	rMatrix[1][1] = c + y*y*t;
	rMatrix[2][2] = c + z*z*t;


	double tmp1 = x*y*t;
	double tmp2 = z*s;
	rMatrix[1][0] = tmp1 + tmp2;
	rMatrix[0][1] = tmp1 - tmp2;
	tmp1 = x*z*t;
	tmp2 = y*s;
	rMatrix[2][0] = tmp1 - tmp2;
	rMatrix[0][2] = tmp1 + tmp2;    
	tmp1 = y*z*t;
	tmp2 = x*s;
	rMatrix[2][1] = tmp1 + tmp2;
	rMatrix[1][2] = tmp1 - tmp2;
}


void Utils::matrix2Euler(double eulerAngles[3], double rMatrix[3][3]){
	// Assuming the angles are in radians.
	double heading,bank,attitude;
	if (rMatrix[1][0] > 0.998) { // singularity at north pole
		heading = atan2(rMatrix[0][2],rMatrix[2][2]);
		attitude = TWO_PI/4;
		bank = 0;
		return;
	}
	if (rMatrix[1][0] < -0.998) { // singularity at south pole
		heading = atan2(rMatrix[0][2],rMatrix[2][2]);
		attitude = -TWO_PI/4;
		bank = 0;
		return;
	}
	heading = atan2(-rMatrix[2][0],rMatrix[0][0]);
	bank = atan2(-rMatrix[1][2],rMatrix[1][1]);
	attitude = asin(rMatrix[1][0]);
	eulerAngles[0] = heading;
	eulerAngles[1] = bank;
	eulerAngles[2] = attitude;
}

void Utils::axisAngle2Euler(double eulerAngles[3],double axisAngle[4]){
	double rMatrix[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	axisAngle2Matrix(rMatrix,axisAngle[0],axisAngle[1],axisAngle[2],axisAngle[3]);
	matrix2Euler(eulerAngles,rMatrix);
}

double Utils::dotProduct( double aVector[3], double bVector[3]){
	double result = aVector[0]*bVector[0]+aVector[1]*bVector[1]+aVector[2]*bVector[2];
	return result;
}

void Utils::crossProduct(double newVector[3], double aVector[3], double bVector[3]){
	double resultV[3];
	resultV[0] = aVector[1]*bVector[2] - aVector[2]*bVector[1];
	resultV[1] = aVector[2]*bVector[0] - aVector[0]*bVector[2];
	resultV[2] = aVector[0]*bVector[1] - aVector[1]*bVector[0];
	newVector[0] = resultV[0];
	newVector[1] = resultV[1];
	newVector[2] = resultV[2];
}

void Utils::rotateVector(double newVector[3], double rMatrix[3][3], double x, double y, double z){
	newVector[0] = (rMatrix[0][0]*x)+(rMatrix[0][1]*y)+(rMatrix[0][2]*z);
	newVector[1] = (rMatrix[1][0]*x)+(rMatrix[1][1]*y)+(rMatrix[1][2]*z);
	newVector[2] = (rMatrix[2][0]*x)+(rMatrix[2][1]*y)+(rMatrix[2][2]*z);
}

void Utils::getRodriguesRotMatrix(double rotMatrix[3][3], double xAngle, double yAngle, double zAngle, double x, double y, double z){
	double v[3] = {x,y,z};
	double theta = Utils::normVector(v);
	double identity[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
	double antiSimMatrix[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	Utils::getAntiSymmetricMatrix(antiSimMatrix,xAngle,yAngle,zAngle);
	//R = I + antiSimMatrix*sin(theta) + (1-cos(theta))*antiSimMatrix*antiSimMatrix
	//antiSimMatrix*sin(theta)
	double mult1[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	Utils::mult3DMatrixValue(mult1,antiSimMatrix,sin(theta));
	//(1-cos(theta))*antiSimMatrix*antiSimMatrix
	double mult2[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	Utils::mult3DMatrix(mult2,antiSimMatrix,antiSimMatrix);
	Utils::mult3DMatrixValue(mult2,mult2,(1-cos(theta)));
	//I + antiSimMatrix*sin(theta) + (1-cos(theta))*antiSimMatrix*antiSimMatrix
	Utils::sum3DMatrix(rotMatrix,mult1,mult2);
	Utils::sum3DMatrix(rotMatrix,identity,rotMatrix);
	return;
}

void Utils::makeRodriguesRotation(double newVector[3], double oldVector[3], double xAngle, double yAngle, double zAngle){
	double rotMatrix[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	getRodriguesRotMatrix(rotMatrix,xAngle,yAngle,zAngle,oldVector[0],oldVector[1],oldVector[2]);
	Utils::rotateVector(newVector,rotMatrix,oldVector[0],oldVector[1],oldVector[2]);
}

double Utils::normVector(double newVector[3]){
	double result = 0;
	result = sqrt((newVector[0]*newVector[0])+(newVector[1]*newVector[1])+(newVector[2]*newVector[2]));
	return result;
}

void Utils::getAntiSymmetricMatrix(double rMatrix[3][3], double x, double y, double z){
	rMatrix[0][0] = 0;
	rMatrix[0][1] = -z;
	rMatrix[0][2] = y;
	rMatrix[1][0] = z;
	rMatrix[1][1] = 0;
	rMatrix[1][2] = -x;
	rMatrix[2][0] = -y;
	rMatrix[2][1] = x;
	rMatrix[2][2] = 0;
}

void Utils::mult3DMatrixValue(double newMatrix[3][3], double aMatrix[3][3], double value){
	for(int l = 0; l != 3; l++)
		for(int c = 0; c != 3; c++)
			newMatrix[l][c] = aMatrix[l][c]*value;
}

void Utils::mult3DMatrix(double newMatrix[3][3], double aMatrix[3][3], double bMatrix[3][3]){
	for(int l = 0; l != 3; l++)
		for(int c = 0; c != 3; c++)
			newMatrix[l][c] = aMatrix[l][0]*bMatrix[0][c]+aMatrix[l][1]*bMatrix[1][c]+aMatrix[l][2]*bMatrix[2][c];
}

void Utils::sum3DMatrix(double newMatrix[3][3], double aMatrix[3][3], double bMatrix[3][3]){
	for(int l = 0; l != 3; l++)
		for(int c = 0; c != 3; c++)
			newMatrix[l][c] = aMatrix[l][c]+bMatrix[l][c];
}

void Utils::mult3DMatrixVector(double newVector[3], double aMatrix[3][3], double vector[3]){
	for(int l = 0; l != 3; l++)
			newVector[l] = aMatrix[l][0]*vector[0]+aMatrix[l][1]*vector[1]+aMatrix[l][2]*vector[2];
}

void Utils::TaitBryan2AxisAngle(double *angleArray, double roll, double pitch, double yaw){
	double rMatrix[3][3];
	euler2Matrix(rMatrix,roll,pitch,yaw);

/*
//test at http://en.wikipedia.org/wiki/Rotation_matrix (one of the examples,74º (-1/3,2/3,2/3))
	rMatrix[0][0] = 0.36;
	rMatrix[0][1] = 0.48;
	rMatrix[0][2] = -0.8;
	rMatrix[1][0] = -0.8;
	rMatrix[1][1] = 0.60;
	rMatrix[1][2] = 0;
	rMatrix[2][0] = 0.48;
	rMatrix[2][1] = 0.64;
	rMatrix[2][2] = 0.60;
*/
	//convert rotation matrix to axis angle
	double x,y,z,angle;
	double norm = sqrt(pow((rMatrix[2][1] - rMatrix[1][2]),2)+pow((rMatrix[0][2] - rMatrix[2][0]),2)+pow((rMatrix[1][0] - rMatrix[0][1]),2));
	if(norm!=0){
		angle = acos(( rMatrix[0][0] + rMatrix[1][1] + rMatrix[2][2] - 1)/2);
		x = (rMatrix[2][1] - rMatrix[1][2]) / norm;
		y = -(rMatrix[0][2] - rMatrix[2][0]) / norm;
		z = -(rMatrix[1][0] - rMatrix[0][1]) / norm;
	}else{
		angle = 0;
		x = 0;
		y = 0;
		z = 0;
	}
	angleArray[0] = x;
	angleArray[1] = y;
	angleArray[2] = z;
	angleArray[3] = angle;
	return;
}

void Utils::euler2Matrix(double rMatrix[3][3], double eulerAngles[3]){
	euler2Matrix(rMatrix,eulerAngles[0],eulerAngles[1],eulerAngles[2]);
}

void Utils::euler2Matrix(double rMatrix[3][3], double roll, double pitch, double yaw){
	double ch = cos(yaw);
    double sh = sin(yaw);
    double ca = cos(pitch);
    double sa = sin(pitch);
    double cb = cos(roll);
    double sb = sin(roll);
    rMatrix[0][0] = ch * ca;
    rMatrix[0][1] = sh*sb - ch*sa*cb;
    rMatrix[0][2] = ch*sa*sb + sh*cb;
    rMatrix[1][0] = sa;
    rMatrix[1][1] = ca*cb;
    rMatrix[1][2] = -ca*sb;
    rMatrix[2][0] = -sh*ca;
    rMatrix[2][1] = sh*sa*cb + ch*sb;
    rMatrix[2][2] = -sh*sa*sb + ch*cb;
}

void Utils::normalizeVector(double newVector[3], double oldVector[3]){
	double norm = Utils::normVector(oldVector);
	newVector[0] = oldVector[0] / norm;
	newVector[1] = oldVector[1] / norm;
	newVector[2] = oldVector[2] / norm;
}