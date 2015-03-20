#pragma once

#include "Wiimote.h"
#include "iCubCtrl.h"
#include "Utils.h"

#include <GL/gl.h>
#include <GL/glut.h>

using namespace std;

class glWindow
{
public:
	glWindow(int argc, char *argv[]);
	~glWindow(void);
private:
	void init(int argc, char *argv[]);
	void initVars();
	void addFunctions(void);
	void setProjection();
	static void drawAxes(void);
	static void draw(void);
	static void drawVector(double point[3], double color[3]);
	static void drawAxesWithAngles(double xangle, double yangle, double zangle);
	static void renderString(float x, float y, void *font, const char* string, Vector rgb);
	static void getNewVector(double newVector[3],double size, double x, double y, double z, double horizAngle);
};
