#pragma once

#include <math.h>
#include <string>
//#ifndef WIN32
	#include <windows.h>
//#endif


#define TWO_PI (2*3.14159265358979323846264338327950288419716939937510582097494459230781)
#define DEG_TO_RAD (TWO_PI/360)
#define RAD_TO_DEG (360/TWO_PI)

class Utils
{
public:
	Utils(void);
	~Utils(void);
	static void TaitBryan2AxisAngle(double *angleArray, double roll, double pitch, double yaw);
	static void getRotationMatrix(double rMatrix[3][3], double eulerAngles[3]);
	static void getRotationMatrix(double rMatrix[3][3], double roll, double pitch, double yaw);
	static void rotateVector(double newVector[3], double rMatrix[3][3], double x, double y, double z);
	static void axisAngle2Matrix(double rMatrix[3][3], double angle, double x, double y, double z);
	static void crossProduct(double newVector[3], double aVector[3], double bVector[3]);
	static double dotProduct( double aVector[3], double bVector[3]);
	static double normVector(double newVector[3]);
	static void normalizeVector(double newVector[3], double oldVector[3]);
	static double lowPassFilter(double oldValue, double newValue);
	static void getAntiSymmetricMatrix(double rMatrix[3][3], double x, double y, double z);
	static double getMatrixDeterminant( double matrix[3][3]);
	static void mult3DMatrix(double newMatrix[3][3], double aMatrix[3][3], double bMatrix[3][3]);
	static void sum3DMatrix(double newMatrix[3][3], double aMatrix[3][3], double bMatrix[3][3]);
	static void mult3DMatrixVector(double newVector[3], double aMatrix[3][3], double vector[3]);
	static void mult3DMatrixValue(double newMatrix[3][3], double aMatrix[3][3], double value);
	static void getRodriguesRotMatrix(double rotMatrix[3][3], double xAngle, double yAngle, double zAngle, double x, double y, double z);
	static void makeRodriguesRotation(double newVector[3], double oldVector[3], double xAngle, double yAngle, double zAngle);
	static void makeRodriguesRotation(double newVector[3], double rotMatrix[3][3], double oldVector[3], double angle[3]);
};
