#pragma once

#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#define WIDTH	500//300
#define HEIGHT	500//100

#define TWO_PI (2*3.14159265358979323846264338327950288419716939937510582097494459230781)
#define DEG_TO_RAD (TWO_PI/360)
#define RAD_TO_DEG (360/TWO_PI)

class GLWindow
{
public:
	GLWindow(int argc, char *argv[]);
	~GLWindow(void);
	void mainLoop();
	static double _angle[3];
	static double _axisAngle[4];
	static double _pointCoord[3], _rotMatrix[3][3];
private:
	void init(int argc, char *argv[]);
	void addFunctions2D();
	void addFunctions3D();
	void setProjection2D();
	void setProjection3D();
	static void draw2D(void);
	static void draw3D(void);
	static void drawCircle(double x, double y, double radius);
	static void drawLine(double x, double y, double length, double angle);
	static void drawAxes(void);
	static void drawVector(double point[3], double color[3]);
	static void crossProduct(double newVector[3], double aVector[3], double bVector[3]);
	static void normalizeVector(double newVector[3], double oldVector[3]);
	static double normVector(double newVector[3]);
	static void rotateVector(double newVector[3], double rMatrix[3][3], double x, double y, double z);
	static void rotMatrix2AxisAngle(double axisAngle[4], double rotMat[3][3]);
	static void drawAxisAngle();
	static void drawAxes(double rotMatrix[3][3]);
};
