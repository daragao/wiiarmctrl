#include "GLWindow.h"

double GLWindow::_angle[3],GLWindow::_axisAngle[4];
double GLWindow::_pointCoord[3], GLWindow::_rotMatrix[3][3];
GLWindow::GLWindow(int argc, char *argv[])
{
	init(argc, argv);
	/*addFunctions2D();
	setProjection2D();*/
	addFunctions3D();
	setProjection3D();
	_angle[0] = 0;
	_angle[1] = 0;
	_angle[2] = 0;
	_axisAngle[0] = 0;
	_axisAngle[1] = 0;
	_axisAngle[2] = 0;
	_axisAngle[3] = 0;
}

GLWindow::~GLWindow(void)
{
}

void GLWindow::mainLoop(){
	glutMainLoop();
}

void GLWindow::init(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE );
	glutCreateWindow("GLUT");
}

void GLWindow::addFunctions2D(){
	glutDisplayFunc(GLWindow::draw2D);
	glutIdleFunc(GLWindow::draw2D);
}

void GLWindow::addFunctions3D(){
	glutDisplayFunc(GLWindow::draw3D);
	glutIdleFunc(GLWindow::draw3D);
}

void GLWindow::setProjection2D(){
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (0, WIDTH, HEIGHT, 0, 0, 1);
	glMatrixMode (GL_MODELVIEW);
	glDisable(GL_DEPTH_TEST);
}

void GLWindow::draw2D(void){
	//OpenGL drawing
	glClear(GL_COLOR_BUFFER_BIT);
	for(double i = 0; i < 3; i++){
		drawCircle(50+(100*i),50,50);
		drawLine(50+(100*i),50,50,_angle[(int)i]);
	}
	glutSwapBuffers();
}

void GLWindow::drawCircle(double x, double y, double radius){
	double step = 5;
	double finalAngle = 360;
	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	for(double i = 0; i < finalAngle; i+=step){
		glVertex2f(x+sin(i*DEG_TO_RAD) * radius, y+cos(i*DEG_TO_RAD) * radius);
	}
	glEnd();
	glPopMatrix();
}

void GLWindow::drawLine(double x, double y, double length, double angle){
	glPushMatrix();
	glTranslated(x,y,0);
	glRotated(angle,0,0,-1);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(length, 0);
	glEnd();
	glPopMatrix();
}

//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D
//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D
//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D//3D

void GLWindow::drawAxes(void){
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

void GLWindow::drawVector(double point[3], double color[3]){
	glColor3f(color[0],color[1],color[2]);
	glLineWidth (1.0);
	glBegin (GL_LINES);
	glVertex3d (0,0,0);
	glVertex3d( point[0],point[1],point[2]);
	glEnd();
}

void GLWindow::drawAxes(double rotMatrix[3][3]){
	float size = 15;
	double color[3];
	float lineWidth = 1;
	float ORG[3] = {0,0,0};
	/*float XP[3] = {rotMatrix[0][0]*size,rotMatrix[0][1]*size,rotMatrix[0][2]*size}, 
		YP[3] = {rotMatrix[1][0]*size,rotMatrix[1][1]*size,rotMatrix[1][2]*size}, 
		ZP[3] = {rotMatrix[2][0]*size,rotMatrix[2][1]*size,rotMatrix[2][2]*size};*/
	double XP[3] = {1,0,0}, YP[3] = {0,1,0}, ZP[3] = {0,0,1};
	rotateVector(XP,rotMatrix,1,0,0);
	normalizeVector(XP,XP);
	XP[0] *= size;
	XP[1] *= size;
	XP[2] *= size;
	rotateVector(YP,rotMatrix,0,1,0);
	normalizeVector(YP,YP);
	YP[0] *= size;
	YP[1] *= size;
	YP[2] *= size;
	rotateVector(ZP,rotMatrix,0,0,1);
	normalizeVector(ZP,ZP);
	ZP[0] *= size;
	ZP[1] *= size;
	ZP[2] *= size;

	glPushMatrix ();
	glLineWidth (lineWidth);
	color[0] = 1; // X axis is red.
	color[1] = 0;
	color[2] = 0;
	drawVector(XP,color);
	color[0] = 0; //Y axis is green.
	color[1] = 1;
	color[2] = 0;
	drawVector(YP,color);
	color[0] = 0; //Z axis is blue.
	color[1] = 0;
	color[2] = 1;
	drawVector(ZP,color);
	glPopMatrix ();
}

void GLWindow::drawAxisAngle(){
	glPushMatrix();
	double axisAngle[4];
	rotMatrix2AxisAngle(axisAngle,_rotMatrix);
	double mult = 15;
	double color[] = {255,0,255};
	double point[] = {axisAngle[1]*mult,axisAngle[2]*mult,axisAngle[3]*mult};
	drawVector(point,color);
	glTranslated(point[0],point[1],point[2]);
	glRotated(90,0,0,1);
	double point2[] = {axisAngle[1]*5,axisAngle[2]*5,axisAngle[3]*5};
	drawVector(point2,color);
	glPopMatrix();
}

void GLWindow::setProjection3D(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-30.0, 30.0, -30.0, 30.0, -30.0, 30.0);
	glRotatef(30.0, 1.0, 0.0, 0.0); 
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}

void GLWindow::draw3D(void){
	normalizeVector(_rotMatrix[0],_rotMatrix[0]);
	normalizeVector(_rotMatrix[1],_rotMatrix[1]);

	normalizeVector(_rotMatrix[2],_rotMatrix[2]);
	//OpenGL drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // Reset current matrix (Modelview)

	glPushMatrix();	

	glRotated(90,-1,0,0);
	glRotated(90,0,0,1);
	glRotated(25,0,0,1);

	glColor3f(255,255,255);
	glutWireSphere(15,10,10);

	drawAxes();
	drawAxes(_rotMatrix);

	double mult = 15;
	double point[] = {0,0,0};
	double point2[] = {0,0,0};
	//rotateVector(point,_rotMatrix,0,1,0);
	point[0]=_pointCoord[0];
	point[1]=_pointCoord[1];
	point[2]=_pointCoord[2];
	normalizeVector(point,point);
	point[0]*=mult;
	point[1]*=mult;
	point[2]*=mult;
	mult = 2;
	rotateVector(point2,_rotMatrix,0,0,1);
	normalizeVector(point2,point2);
	point2[0] = (point2[0] * mult) + point[0];
	point2[1] = (point2[1] * mult) + point[1];
	point2[2] = (point2[2] * mult) + point[2];

	glPushMatrix();	
	double color[] = {255,255,0};
	drawVector(point,color);
	glPopMatrix();

	glPushMatrix();
	//glColor3f(color[0],color[1],color[2]);
	glLineWidth (1.0);
	glBegin (GL_LINES);
	glVertex3d (point[0],point[1],point[2]);
	glVertex3d( point2[0],point2[1],point2[2]);
	glEnd();
	glPopMatrix();

	drawAxisAngle();

	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void GLWindow::crossProduct(double newVector[3], double aVector[3], double bVector[3]){
	double resultV[3];
	resultV[0] = aVector[1]*bVector[2] - aVector[2]*bVector[1];
	resultV[1] = aVector[2]*bVector[0] - aVector[0]*bVector[2];
	resultV[2] = aVector[0]*bVector[1] - aVector[1]*bVector[0];
	newVector[0] = resultV[0];
	newVector[1] = resultV[1];
	newVector[2] = resultV[2];
}

double GLWindow::normVector(double newVector[3]){
	double result = 0;
	result = sqrt(pow(newVector[0],2)+pow(newVector[1],2)+pow(newVector[2],2));
	return result;
}

void GLWindow::normalizeVector(double newVector[3], double oldVector[3]){
	double norm = normVector(oldVector);
	if(norm == 0 || norm != norm) return;
	newVector[0] = oldVector[0] / norm;
	newVector[1] = oldVector[1] / norm;
	newVector[2] = oldVector[2] / norm;
}

void GLWindow::rotateVector(double newVector[3], double rMatrix[3][3], double x, double y, double z){
	newVector[0] = (rMatrix[0][0]*x)+(rMatrix[0][1]*y)+(rMatrix[0][2]*z);
	newVector[1] = (rMatrix[1][0]*x)+(rMatrix[1][1]*y)+(rMatrix[1][2]*z);
	newVector[2] = (rMatrix[2][0]*x)+(rMatrix[2][1]*y)+(rMatrix[2][2]*z);
}

void GLWindow::rotMatrix2AxisAngle(double axisAngle[4], double rotMat[3][3]) {
	double angle,x,y,z; // variables for result
	double epsilon = 0.01; // margin to allow for rounding errors
	double epsilon2 = 0.1; // margin to distinguish between 0 and 180 degrees
	// optional check that input is pure rotation, 'isRotationMatrix' is defined at:
	// http://www.euclideanspace.com/maths/algebra/matrix/orthogonal/rotation/
	if ((abs(rotMat[0][1]-rotMat[1][0])< epsilon)
		&& (abs(rotMat[0][2]-rotMat[2][0])< epsilon)
		&& (abs(rotMat[1][2]-rotMat[2][1])< epsilon)) {
			// singularity found
			// first check for identity matrix which must have +1 for all terms
			//  in leading diagonaland zero in other terms
			if ((abs(rotMat[0][1]+rotMat[1][0]) < epsilon2)
				&& (abs(rotMat[0][2]+rotMat[2][0]) < epsilon2)
				&& (abs(rotMat[1][2]+rotMat[2][1]) < epsilon2)
				&& (abs(rotMat[0][0]+rotMat[1][1]+rotMat[2][2]-3) < epsilon2)) {
					// this singularity is identity matrix so angle = 0
					axisAngle[0] = 0;
					axisAngle[1] = 1;
					axisAngle[2] = 0;
					axisAngle[3] = 0;
					return;// zero angle, arbitrary axis
			}
			// otherwise this singularity is angle = 180
			angle = TWO_PI/2;
			double xx = (rotMat[0][0]+1)/2;
			double yy = (rotMat[1][1]+1)/2;
			double zz = (rotMat[2][2]+1)/2;
			double xy = (rotMat[0][1]+rotMat[1][0])/4;
			double xz = (rotMat[0][2]+rotMat[2][0])/4;
			double yz = (rotMat[1][2]+rotMat[2][1])/4;
			if ((xx > yy) && (xx > zz)) { // m[0][0] is the largest diagonal term
				if (xx< epsilon) {
					x = 0;
					y = 0.7071;
					z = 0.7071;
				} else {
					x = sqrt(xx);
					y = xy/x;
					z = xz/x;
				}
			} else if (yy > zz) { // m[1][1] is the largest diagonal term
				if (yy< epsilon) {
					x = 0.7071;
					y = 0;
					z = 0.7071;
				} else {
					y = sqrt(yy);
					x = xy/y;
					z = yz/y;
				}	
			} else { // m[2][2] is the largest diagonal term so base result on this
				if (zz< epsilon) {
					x = 0.7071;
					y = 0.7071;
					z = 0;
				} else {
					z = sqrt(zz);
					x = xz/z;
					y = yz/z;
				}
			}
			axisAngle[0] = angle;
			axisAngle[1] = x;
			axisAngle[2] = y;
			axisAngle[3] = z;
			return;  // return 180 deg rotation
	}
	// as we have reached here there are no singularities so we can handle normally
	double s = sqrt((rotMat[2][1] - rotMat[1][2])*(rotMat[2][1] - rotMat[1][2])
		+(rotMat[0][2] - rotMat[2][0])*(rotMat[0][2] - rotMat[2][0])
		+(rotMat[1][0] - rotMat[0][1])*(rotMat[1][0] - rotMat[0][1])); // used to normalise
	if (abs(s) < 0.001) s=1; 
	// prevent divide by zero, should not happen if matrix is orthogonal and should be
	// caught by singularity test above, but I've left it in just in case
	angle = acos(( rotMat[0][0] + rotMat[1][1] + rotMat[2][2] - 1)/2);
	x = (rotMat[2][1] - rotMat[1][2])/s;
	y = (rotMat[0][2] - rotMat[2][0])/s;
	z = (rotMat[1][0] - rotMat[0][1])/s;
	axisAngle[0] = angle;
	axisAngle[1] = x;
	axisAngle[2] = y;
	axisAngle[3] = z;
	return; 
}