#include "glWindow.h"

glWindow::glWindow(int argc, char *argv[]){

	init(argc, argv);
	addFunctions();
	setProjection();

	glutMainLoop();
}

glWindow::~glWindow(void){
}

void glWindow::init(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitWindowSize(700, 700);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE );
	glutCreateWindow("GLUT");
}

void glWindow::addFunctions(){
	glutDisplayFunc(glWindow::draw);
	glutIdleFunc(glWindow::draw);
}

void glWindow::setProjection(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-30.0, 30.0, -30.0, 30.0, -30.0, 30.0);
	glRotatef(30.0, 1.0, 0.0, 0.0); 
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void glWindow::renderString(float x, float y, void *font, const char* str, Vector rgb){  
	glColor3f(rgb[0], rgb[1], rgb[2]); 
	glRasterPos2f(x, y);
	const char *c;
	for (c=str; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void glWindow::drawAxes(void){
	float size = 15;
	float ORG[3] = {0,0,0};
	float XP[3] = {size,0,0}, XN[3] = {-size,0,0},
		YP[3] = {0,size,0}, YN[3] = {0,-size,0},
		ZP[3] = {0,0,size}, ZN[3] = {0,0,-1};

	glPushMatrix ();
	//glScalef (0.25, 0.25, 0.25);
	glLineWidth (2.0);
	glBegin (GL_LINES);
	glColor3f (1,0,0); // X axis is red.
	ORG[0] = -XP[0]*0.1;
	ORG[1] = -XP[1]*0.1;
	ORG[2] = -XP[2]*0.1;
	glVertex3fv (ORG);
	glVertex3fv (XP ); 
	glColor3f (0,1,0); // Y axis is green.
	ORG[0] = -YP[0]*0.1;
	ORG[1] = -YP[1]*0.1;
	ORG[2] = -YP[2]*0.1;
	glVertex3fv (ORG);
	glVertex3fv (YP );
	glColor3f (0,0,1); // z axis is blue.
	ORG[0] = -ZP[0]*0.1;
	ORG[1] = -ZP[1]*0.1;
	ORG[2] = -ZP[2]*0.1;
	glVertex3fv (ORG);
	glVertex3fv (ZP ); 
	glEnd();
	glPopMatrix ();
}

void glWindow::drawVector(double point[3], double color[3]){
	glColor3f(color[0],color[1],color[2]);
	glLineWidth (1.0);
	glBegin (GL_LINES);
	glVertex3d (0,0,0);
	glVertex3d( point[0],point[1],point[2]);
	glEnd();
}

void glWindow::draw(void){
	/*char str[300];
	int length = 0;
	Vector colorText(3);

	//OpenGL drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // Reset current matrix (Modelview)

	glPushMatrix();	
	colorText[0] = 1;
	colorText[1] = 1;
	colorText[2] = 1;
	length = sprintf_s(str,"calibration_status:%3.2f%%",00000000);
	renderString(leftText, topText, GLUT_BITMAP_HELVETICA_12, &(*str), colorText);
	glPopMatrix();

	glRotated(90,-1,0,0);
	glRotated(90,0,0,1);
	glRotated(25,0,0,1);
	
	//Axis angle
	glPushMatrix();
	glutWireSphere(5,10,10);
	drawAxes();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();*/
}