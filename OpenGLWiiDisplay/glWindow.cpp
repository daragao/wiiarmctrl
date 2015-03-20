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

void glWindow::initVars(){
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

void glWindow::getNewVector(double newVector[3],double size, double x, double y, double z, double horizAngle){
	double rMatrix[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
	Utils::axisAngle2Matrix(rMatrix, horizAngle, 0, -1, 0);
	Utils::rotateVector(newVector,rMatrix,x,y,z);
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

void glWindow::drawAxesWithAngles(double xangle, double yangle, double zangle){
	float size = 15;
	double color[3] = {0,0,0};
	double xVector[3] = {0,0,0};
	double yVector[3] = {0,0,0};
	double zVector[3] = {0,0,0};
	double rMatrix[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
	Utils::getRotationMatrix(rMatrix,xangle,0,0);
	Utils::rotateVector(xVector,rMatrix,0,0,1);
	Utils::normalizeVector(xVector,xVector);

	Utils::getRotationMatrix(rMatrix,0,yangle,0);
	Utils::rotateVector(yVector,rMatrix,0,1,0);
	Utils::normalizeVector(yVector,yVector);

	Utils::getRotationMatrix(rMatrix,0,0,zangle);
	Utils::rotateVector(zVector,rMatrix,1,0,0);
	Utils::normalizeVector(zVector,zVector);

	drawAxes();
	glPushMatrix ();
	color[0] = 1.0;
	color[1] = 0.0;
	color[2] = 0.0;
	xVector[0] *= size;
	xVector[1] *= size;
	xVector[2] *= size;
	drawVector(xVector,color);
	color[0] = 0.0;
	color[1] = 0.0;
	color[2] = 1.0;
	yVector[0] *= size;
	yVector[1] *= size;
	yVector[2] *= size;
	drawVector(yVector,color);
	color[0] = 0.0;
	color[1] = 1.0;
	color[2] = 0.0;
	zVector[0] *= size;
	zVector[1] *= size;
	zVector[2] *= size;
	drawVector(zVector,color);
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
	char str[300];
	int length = 0;
	Vector colorText(3);
	float leftText = -29.5f;
	float topText = 32.5f;
	double gForce[3] = {0,0,0};
	double acc[3] = {0,0,0};
	double accOrient[3] = {0,0,0};
	double gForceNorm = 0;
	double point[3] = {0,0,0};
	double color[3] = {0,1,1};
	static double gyro[3] = {0,0,0};
	double gyroNorm = 0;
	double gyroRMatrix[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
	static long lastSequenceNum = -1;
	static double gyroVector[3] = {0,1,0};
	double estCoord[3] = {0,0,0};
	double gyroWeight = 5;
	double iCubCoords[3] = {iCubCtrl::_icubCoords[0],iCubCtrl::_icubCoords[1],iCubCtrl::_icubCoords[2]};
	Utils::normalizeVector(iCubCoords,iCubCoords);
	double iCubOri[4] = {iCubCtrl::_icubCoords[3],iCubCtrl::_icubCoords[4],iCubCtrl::_icubCoords[5],iCubCtrl::_icubCoords[6]};

	gForce[0] = Wiimote::_gForce[0];
	gForce[1] = Wiimote::_gForce[1];
	gForce[2] = Wiimote::_gForce[2];
	gForceNorm = Utils::normVector(gForce);
	Utils::normalizeVector(gForce,gForce);
	if(Wiimote::_calibStatus == 1){
		acc[0] = Wiimote::_acc[0];
		acc[1] = Wiimote::_acc[1];
		acc[2] = Wiimote::_acc[2];
		accOrient[0] = Wiimote::_acc[3]*DEG_TO_RAD;
		accOrient[1] = Wiimote::_acc[4]*DEG_TO_RAD;
		accOrient[2] = Wiimote::_acc[5]*DEG_TO_RAD;
		gyro[0] = Wiimote::_mpAngleRate[0];
		gyro[1] = Wiimote::_mpAngleRate[1];
		gyro[2] = Wiimote::_mpAngleRate[2];
		gyroNorm = Utils::normVector(gyro);

		if(Wiimote::_sequenceNum != lastSequenceNum){
			lastSequenceNum = Wiimote::_sequenceNum;
			Utils::getRotationMatrix(gyroRMatrix,-gyro[0],0,0);
			Utils::rotateVector(gyroVector,gyroRMatrix,gyroVector[0],gyroVector[1],gyroVector[2]);
			Utils::getRotationMatrix(gyroRMatrix,0,-gyro[2],0);
			Utils::rotateVector(gyroVector,gyroRMatrix,gyroVector[0],gyroVector[1],gyroVector[2]);
			Utils::getRotationMatrix(gyroRMatrix,0,0,-gyro[1]);
			Utils::rotateVector(gyroVector,gyroRMatrix,gyroVector[0],gyroVector[1],gyroVector[2]);
			Utils::normalizeVector(gyroVector,gyroVector);
		}

		//calculate estimated value
		/*estCoord[0] = (gForce[0] + (gyroVector[0]*gyroWeight))/(1+gyroWeight);
		estCoord[1] = (gForce[1] + (gyroVector[1]*gyroWeight))/(1+gyroWeight);
		estCoord[2] = (gForce[2] + (gyroVector[2]*gyroWeight))/(1+gyroWeight);
		Utils::normalizeVector(estCoord,estCoord);*/
	}

	//OpenGL drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // Reset current matrix (Modelview)

	glPushMatrix();	
	colorText[0] = 1 - Wiimote::_calibStatus;
	colorText[1] = Wiimote::_calibStatus;
	colorText[2] = 0;
	length = sprintf_s(str,"calibration_status:%3.2f%%",Wiimote::_calibStatus*100);
	renderString(leftText, topText, GLUT_BITMAP_HELVETICA_12, &(*str), colorText);
	colorText[0] = 1;
	colorText[1] = 1;
	colorText[2] = 1;
	length = sprintf_s(str,"gForce:(%3.2f,%3.2f,%3.2f):%3.2f",gForce[0],gForce[1],gForce[2],gForceNorm);
	renderString(leftText, topText-2, GLUT_BITMAP_HELVETICA_12, &(*str), colorText);
	colorText[0] = 1;
	colorText[1] = 1;
	colorText[2] = 0;
	length = sprintf_s(str,"gyro:(%3.2fº,%3.2fº,%3.2fº):%3.2f", gyro[0]*RAD_TO_DEG, gyro[1]*RAD_TO_DEG, gyro[2]*RAD_TO_DEG, gyroNorm);
	renderString(leftText, topText-4, GLUT_BITMAP_HELVETICA_12, &(*str), colorText);
	length = sprintf_s(str,"gyroCoord:(%3.2f,%3.2f,%3.2f)", gyroVector[0], gyroVector[1], gyroVector[2]);
	renderString(leftText, topText-6, GLUT_BITMAP_HELVETICA_12, &(*str), colorText);
	colorText[0] = 0;
	colorText[1] = 1;
	colorText[2] = 1;
	length = sprintf_s(str,"estCoord:(%3.2f,%3.2f,%3.2f)", estCoord[0], estCoord[1], estCoord[2]);
	renderString(leftText, topText-8, GLUT_BITMAP_HELVETICA_12, &(*str), colorText);
	colorText[0] = 1;
	colorText[1] = 1;
	colorText[2] = 1;
	length = sprintf_s(str,"acc:(%3.2f,%3.2f,%3.2f)", acc[0], acc[1], acc[2]);
	renderString(leftText, topText-10, GLUT_BITMAP_HELVETICA_12, &(*str), colorText);
	length = sprintf_s(str,"accOrient:(%3.2fº,%3.2fº,%3.2fº)", accOrient[0]*RAD_TO_DEG, accOrient[1]*RAD_TO_DEG, accOrient[2]*RAD_TO_DEG);
	renderString(leftText, topText-12, GLUT_BITMAP_HELVETICA_12, &(*str), colorText);
	glPopMatrix();

	glRotated(90,-1,0,0);
	glRotated(90,0,0,1);
	glRotated(25,0,0,1);
	
	//Axis angle
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	if(Wiimote::_mpPointCoords.data()!=NULL){
		color[0] = 1.0;
		color[1] = 1.0;
		color[2] = 1.0;
		double pointP[3] = {Wiimote::_mpPointCoords[0],Wiimote::_mpPointCoords[1],Wiimote::_mpPointCoords[2]};
		Utils::normalizeVector(pointP,pointP);
		pointP[0]*=15;
		pointP[1]*=15;
		pointP[2]*=15;
		drawVector(pointP,color);
		//normal
		pointP[0] = Wiimote::_mpPointCoords[3];
		pointP[1] = Wiimote::_mpPointCoords[4];
		pointP[2] = Wiimote::_mpPointCoords[5];
		Utils::normalizeVector(pointP,pointP);
		pointP[0]*=15;
		pointP[1]*=15;
		pointP[2]*=15;
		drawVector(pointP,color);
	}
	color[0] = 1.0;
	color[1] = 1.0;
	color[2] = 0.0;
	point[0] = gyroVector[0]*15;
	point[1] = gyroVector[1]*15;
	point[2] = gyroVector[2]*15;
	drawVector(point,color);
	color[0] = 0.0;
	color[1] = 1.0;
	color[2] = 1.0;
	point[0] = estCoord[0]*15;
	point[1] = estCoord[1]*15;
	point[2] = estCoord[2]*15;
	drawVector(point,color);
	color[0] = 1.0;
	color[1] = 0.0;
	color[2] = 1.0;
	point[0] = iCubCoords[0]*15;
	point[1] = iCubCoords[1]*15;
	point[2] = iCubCoords[2]*15;
	drawVector(point,color);
	glPushMatrix();
	color[0] = 1.0;
	color[1] = 0.0;
	color[2] = 1.0;
	glRotated(iCubOri[3],iCubCoords[0],iCubCoords[1],iCubCoords[2]);
	glTranslated(point[0],point[1],point[2]);
	point[0] = iCubOri[0]*5;
	point[1] = iCubOri[1]*5;
	point[2] = iCubOri[2]*5;
	drawVector(point,color);
	glPopMatrix();
	drawAxes();
	//drawAxesWithAngles(gyroCumulative[0],gyroCumulative[1],gyroCumulative[2]);
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}