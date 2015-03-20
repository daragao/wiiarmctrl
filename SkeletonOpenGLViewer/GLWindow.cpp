#include "GLWindow.h"


double GLWindow::_cameraPos[9], GLWindow::_jointsPos[24*3], GLWindow::_jointsOri[24][9];

GLWindow::GLWindow(int argc, char *argv[])
{
	initVars();
	initWindow(argc, argv);
}

void GLWindow::initVars(){
	_glStarted = false;
	int i;
	for(i = 0; i < 9; i++)
		_cameraPos[i] = 0;
	for(i = 0; i < (15*3); i++)
		_jointsPos[i] = 0;
}

void GLWindow::cameraRePos(){
	gluLookAt(_cameraPos[0],_cameraPos[1],_cameraPos[2],_cameraPos[3],_cameraPos[4],_cameraPos[5],_cameraPos[6],_cameraPos[7],_cameraPos[8]);
}

void GLWindow::redraw(){
	if(_glStarted)
		glutPostRedisplay();
}

void GLWindow::initWindow(int argc, char *argv[], int width, int height){
	glutInit(&argc,argv);
	glutInitWindowSize(width,height);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_RGBA|GLUT_DOUBLE);
	glutCreateWindow("GL_Skeleton");
	//set callbacks
	glutDisplayFunc(GLWindow::displayDraw);
	glutSpecialFunc(GLWindow::inputKey);
	//set projection
	double ratio = width / height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0,0,width,height);
	gluPerspective(45,ratio,1,20000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	_cameraPos[2] = 10000;
	//_cameraPos[5] = 1;
	_cameraPos[7] = 1;
	cameraRePos();

	//color settings
	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
}

void GLWindow::mainLoop(){
	_glStarted = true;
	glutMainLoop();
}

GLWindow::~GLWindow(void)
{
}

void GLWindow::displayDraw(){
	glLoadIdentity();
	//set camera
	cameraRePos();
	//draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	//simpleCube();
	drawJointPoints();
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

void GLWindow::simpleCube(){
	glColor3f(0.9,0.0,0.0);
	glutWireSphere(2,10,10);
}

void GLWindow::drawJointPoints(){
	glPointSize(6);
	glLineWidth(1);
	for(int i = 0; i < 24; i++){
		glPushMatrix();
		glTranslated(_jointsPos[(i*3)+0],_jointsPos[(i*3)+1],_jointsPos[(i*3)+2]);
		//position
		glColor3f(0,1,1);
		glBegin(GL_POINTS);
		glVertex3f(0,0,0);
		glEnd();
		//orientation
		drawJointOriAxis(i);
		glPopMatrix();
	}
}

void GLWindow::drawText(){
	glColor4f(1,0,0,1);
	glRasterPos2i(20, 50);
	char strLabel[20];
	sprintf_s(strLabel, "OLA");
	
	void *font = GLUT_BITMAP_HELVETICA_18;
	char * str = strLabel;
	//printing function
	int i,l = strlen(str);
	for(i=0; i<l; i++)
		glutBitmapCharacter(font,*str++);
}

void GLWindow::drawJointOriAxis(int index){
	int length = 200;
	glColor3f(1,0,0);
	glBegin(GL_LINES);
	glVertex3f(0,0,0);
	glVertex3f(_jointsOri[index][0]*length,_jointsOri[index][1]*length,_jointsOri[index][2]*length);
	glColor3f(0,1,0);
	glVertex3f(0,0,0);
	glVertex3f(_jointsOri[index][3]*length,_jointsOri[index][4]*length,_jointsOri[index][5]*length);
	glColor3f(0,0,1);
	glVertex3f(0,0,0);
	glVertex3f(_jointsOri[index][6]*length,_jointsOri[index][7]*length,_jointsOri[index][8]*length);
	glEnd();

}

void GLWindow::inputKey(int key, int x, int y){
	float step = 100;
	switch(key){
		case GLUT_KEY_LEFT:
			_cameraPos[0]-=step;
			_cameraPos[3]-=step;
			break;
		case GLUT_KEY_RIGHT:
			_cameraPos[0]+=step;
			_cameraPos[3]+=step;
			break;
		case GLUT_KEY_DOWN:
			_cameraPos[1]-=step;
			_cameraPos[4]-=step;
			break;
		case GLUT_KEY_UP:
			_cameraPos[1]+=step;
			_cameraPos[4]+=step;
			break;
		case GLUT_KEY_HOME:
			_cameraPos[2]+=step;
			_cameraPos[5]+=step;
			break;
		case GLUT_KEY_INSERT:
			_cameraPos[2]-=step;
			_cameraPos[5]-=step;
			break;
	}
	glutPostRedisplay();
}

void GLWindow::setJointPos(int index, double value){
	_jointsPos[index] = value;
}

void GLWindow::setJointOri(int index, double elem1, double elem2, double elem3
						   , double elem4, double elem5, double elem6
						   , double elem7, double elem8, double elem9){
	_jointsOri[index][0] = elem1;
	_jointsOri[index][1] = elem2;
	_jointsOri[index][2] = elem3;
	_jointsOri[index][3] = elem4;
	_jointsOri[index][4] = elem5;
	_jointsOri[index][5] = elem6;
	_jointsOri[index][6] = elem7;
	_jointsOri[index][7] = elem8;
	_jointsOri[index][8] = elem9;
}
