#pragma once

#include <stdio.h>
#include <string.h>
#include <GL/glut.h>

class GLWindow
{
public:
	GLWindow(int argc, char *argv[]);
	~GLWindow(void);
	void mainLoop();
	void setJointPos(int index, double value);
	void setJointOri(int index, double elem1, double elem2, double elem3, double elem4, double elem5, double elem6, double elem7, double elem8, double elem9);
	void redraw();
private:
	bool _glStarted;
	static double _cameraPos[9], _jointsPos[24*3], _jointsOri[24][9];
	void initWindow(int argc, char *argv[], int width = 640, int height = 480);
	void initVars();
	static void displayDraw();
	static void simpleCube();
	static void drawJointPoints();
	static void drawJointOriAxis(int index);
	static void drawText();
	static void cameraRePos();
	static void inputKey(int key, int x, int y);
};
