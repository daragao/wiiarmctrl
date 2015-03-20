#pragma once
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
	void addFunctions(void);
	void setProjection();
	static void drawAxes(void);
	static void draw(void);
	static void drawVector(double point[3], double color[3]);
	static void renderString(float x, float y, void *font, const char* string, Vector rgb);
};
