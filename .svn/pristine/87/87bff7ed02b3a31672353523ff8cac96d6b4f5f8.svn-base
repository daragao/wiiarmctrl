#include "SimpleTest.h"

int SimpleTest::win1,SimpleTest::win2;
double SimpleTest::r, SimpleTest::theta, SimpleTest::phi;
int SimpleTest::CurrentMode, SimpleTest::NumModes;
double SimpleTest::Xmin, SimpleTest::Xmax, SimpleTest::Ymin, SimpleTest::Ymax, SimpleTest::calibStatus;
bool SimpleTest::isStarted;
double SimpleTest::forceVector[4];
double SimpleTest::gForce[3],SimpleTest::acc[3],SimpleTest::mp[3];
queue<double> *SimpleTest::queue1,*SimpleTest::queue2;
double SimpleTest::ratioWin2;

SimpleTest::SimpleTest()
{
	isStarted = false;
	// These variables control the current mode
	CurrentMode = 0;
	NumModes = 5;

	// These variables set the dimensions of the rectanglar region we wish to view.
	Xmin = 0.0, Xmax = 3.0;
	Ymin = 0.0, Ymax = 3.0;
	ratioWin2 = 0;

	// glutKeyboardFunc is called below to set this function to handle
	//		all "normal" ascii key presses.
	// Only space bar and escape key have an effect.

	r = MAX_R, theta = 45, phi = 45;
	//x,y,z
	forceVector[0] = 0;
	forceVector[1] = 0;
	forceVector[2] = 0;
	//value
	forceVector[3] = 0;

	calibStatus = -1;

	win1 = 0;
	win2 = 0;

	queue1 = new queue<double>();
	queue2 = new queue<double>();
}

SimpleTest::~SimpleTest(void)
{
}
//void SimpleTest::addThreadData(bool aBot, bool bBot, double calibration, double rx, double ry, double rz, double accX, double accY, double accZ){
void SimpleTest::addThreadData(double timeDiff, bool aBot, bool bBot, double calibration, double mpArg[3], double gForceArg[3], double accArg[3]){
	bool redraw = false;
	double rx = mp[0] = mpArg[0]*RAD_TO_DEG;
	double ry = mp[1] = mpArg[1]*RAD_TO_DEG;
	double rz = mp[2] = mpArg[2]*RAD_TO_DEG;
	double accX = acc[0] = accArg[0];
	double accY = acc[1] = accArg[1];
	double accZ = acc[2] = accArg[2];
	double gForceX = gForce[0] = gForceArg[0];
	double gForceY = gForce[1] = gForceArg[1];
	double gForceZ = gForce[2] = gForceArg[2];

	if(aBot){
		phi		+= rx;
		theta	+= rz;
		r		+= MAX_R*acc2Size(timeDiff,gForceX,gForceY,gForceZ,mp);
		//r		+= MAX_R*acc2Size(accX,accY,accZ);
		redraw = true;
	}
	if(calibration != calibStatus){
		calibStatus = calibration;
		redraw = true;
	}
	if(redraw && isStarted){
		//glutPostRedisplay();
		glutPostWindowRedisplay(win1);
		glutPostWindowRedisplay(win2);
	}
	//printStatus(vector[0],vector[1],vector[2]);
}
void SimpleTest::sphericalCoords2Vector(double vector[3], double r, double theta, double phi){
	vector[0] = r * cos (theta * DEG_TO_RAD) * sin (phi * DEG_TO_RAD);
	vector[1] = r * sin (theta * DEG_TO_RAD) * sin (phi * DEG_TO_RAD);
	vector[2] = r * cos (phi * DEG_TO_RAD);
}
double SimpleTest::acc2Size(double timeDiff, double xValue, double yValue, double zValue, double gyro[3]){
	double valueVector[3] = {xValue,yValue,zValue};
	static double oldValueVector[3] = {xValue,yValue,zValue};
	double gyroValueVector[3] = {gyro[0],gyro[1],gyro[2]};
	static double oldGyroValueVector[3] = {gyro[0],gyro[1],gyro[2]};
	double maxDiff = 10;
	double minDiff = 5;
	double accNormDiff = Utils::normVector(valueVector) - Utils::normVector(oldValueVector);
	double gyroNormDiff = Utils::normVector(gyroValueVector) - Utils::normVector(oldGyroValueVector);
	for(int i=0;i<3;i++){
		if(abs(valueVector[i]-oldValueVector[i])>maxDiff){
			valueVector[i] = oldValueVector[i]+(valueVector[i]>oldValueVector[i])?+maxDiff:-maxDiff;
		}
	}
	double normalizedValues[3] = {0,0,0};
	Utils::normalizeVector(normalizedValues,valueVector);
	double norm = normalizedValues[0];//Utils::normVector(valueVector);

	//printf("-----------\n");
	//norm*=1000;
	//printf("ax*1000=%f t=%f\n",norm,timeDiff);
	
	//V(k)=V(k-1)+A(k)*DT
	static double lastVel = 0;//&-norm*timeDiff;
	double vel = lastVel+norm*timeDiff;
	//printf("vel=%f lastVel=%f\n",vel,lastVel);
	lastVel = vel;
	//P(k)=P(k-1)+1/2*A(k)*DT*DT
	static double lastPos = 0; //-(1/2)*norm*timeDiff*timeDiff;
	double pos = lastPos+ lastVel*timeDiff + (1/2)*norm*timeDiff*timeDiff;
	//printf("pos=%f lastPos=%f\n",pos,lastPos);
	lastPos = pos;

	double result = vel;

	//double result = norm;
	//static double lastResult = norm;
	//result = (norm-lastResult);
	//lastResult = norm;
	//result *= 0.5;

	//x,y,z
	forceVector[0] = normalizedValues[0];
	forceVector[1] = vel;//valueVector[0];
	forceVector[2] = pos;//valueVector[1];
	/*forceVector[0] = gyroNormDiff;
	forceVector[1] = accNormDiff;*/
	//value
	forceVector[3] = accNormDiff;

	oldValueVector[0] = valueVector[0];
	oldValueVector[1] = valueVector[1];
	oldValueVector[2] = valueVector[2];
	oldGyroValueVector[0] = gyroValueVector[0];
	oldGyroValueVector[1] = gyroValueVector[1];
	oldGyroValueVector[2] = gyroValueVector[2];

	return result;
}

void SimpleTest::myKeyboardFunc( unsigned char key, int x, int y )
{
	switch ( key ) {

	case ' ':									// Space bar
		theta += 10;
		glutPostRedisplay();
		break;
	case 'm':
		// Increment the current mode, and tell operating system screen needs redrawing
		CurrentMode = (CurrentMode+1)%NumModes;
		glutPostRedisplay();
		break;

	case 27:									// "27" is theEscape key
		exit(1);
	}
}

void SimpleTest::drawScene2(void){
	char strChar[50];
	isStarted = true;
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set drawing color to white
	glColor3f( 1.0, 1.0, 1.0 );		

	double space = Xmax/4;

	glPushMatrix();
	glTranslated(0,.5,0);
	drawMotionSenseBars("gForce",1,gForce);
	glTranslated(space,0,0);
	drawMotionSenseBars("acc",500,acc);
	glTranslated(space,0,0);
	drawMotionSenseBars("forceVector",1,forceVector);
	glTranslated(.8,0,0);
	drawSize(forceVector[3],1,1,1,0);
	glTranslated(.5,0,0);
	drawSize(r,1,1,1,0);
	sprintf_s(strChar,"r:%3.2f",r);
	renderString(0,-.15, GLUT_BITMAP_HELVETICA_12, strChar, 1,1,1);
	glPopMatrix();

	
	//add to queue
	int n = 300;
	if(queue1->size()==n) queue1->pop();
	queue1->push(forceVector[0]);
	plotLine(n,queue1,-1,1, 1,0,1);
	if(queue2->size()==n) queue2->pop();
	queue2->push(forceVector[3]);
	plotLine(n,queue2,-1,1, 0,1,1);

	glFlush();
}

void SimpleTest::plotLine(double totalPoints, queue<double> *points, double minValue, double maxValue, double r, double g, double b){
	double minX,maxX,minY,maxY,currPointX,currPointY;
	double point;
	queue<double> auxPoints;
	minX = Xmin;
	maxX = Xmax;
	minY = Ymin;
	maxY = Ymax;
	if(!points->empty()){
		point = points->front();
		points->pop();
		auxPoints.push(point);
	}
	currPointX = minX;
	currPointY = minY + maxY*((point-minValue)/(maxValue-minValue));
	glPushMatrix();
	glColor3f(r,g,b);
	glLineWidth (1);
	glBegin (GL_LINES);
	for(int i = 0; i < totalPoints; i++){
		glVertex3d (currPointX,currPointY,0);
		if(!points->empty()){
			point = points->front();
			points->pop();
			auxPoints.push(point);
		}
		currPointX = minX + (maxX*(i/totalPoints));
		currPointY = minY + maxY*((point-minValue)/(maxValue-minValue));
		glVertex3d (currPointX,currPointY,0);
	}
	glEnd();
	glPopMatrix();
	*points = auxPoints;
}

void SimpleTest::drawMotionSenseBars(string title, double maxSize, double motionData[3]){
	string str;
	char strChar[50];
	glPushMatrix();
	sprintf_s(strChar,"%s(%3.2f,%3.2f,%3.2f)",title.c_str(),motionData[0],motionData[1],motionData[2]);
	renderString(0,-.35, GLUT_BITMAP_HELVETICA_12, strChar, 1,1,1);
	glPopMatrix();
	glPushMatrix();
	glTranslated(.2,0,0);
	str = "x";
	renderString(0,-.15, GLUT_BITMAP_HELVETICA_12, str.c_str(), 1,0,0);
	drawSize(motionData[0],maxSize,1,0,0);
	glTranslated(.2,0,0);
	str = "y";
	renderString(0,-.15, GLUT_BITMAP_HELVETICA_12, str.c_str(), 0,1,0);
	drawSize(motionData[1],maxSize,0,1,0);
	glTranslated(.2,0,0);
	str = "z";
	renderString(0,-.15, GLUT_BITMAP_HELVETICA_12, str.c_str(), 0,0,1);
	drawSize(motionData[2],maxSize,0,0,1);
	glPopMatrix();
}

/*
* drawScene() handles the animation and the redrawing of the
*		graphics window contents.
*/
void SimpleTest::drawScene(void)
{
	char strChar[50];
	//getVector
	double vector[3];
	sphericalCoords2Vector(vector,r,theta,phi);

	isStarted = true;
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set drawing color to white
	glColor3f( 1.0, 1.0, 1.0 );		


	glPushMatrix();
	glTranslatef(1.5, 1.5, 0);
	std::ostringstream os;
	os << calibStatus;
	string str = "calibStatus: " + os.str();
	renderString(-1.5,1.5-.2, GLUT_BITMAP_HELVETICA_12, str.c_str(), 1-calibStatus,calibStatus,0);
	sprintf_s(strChar,"mp(%3.2f,%3.2f,%3.2f)",mp[0]*RAD_TO_DEG,mp[1]*RAD_TO_DEG,mp[2]*RAD_TO_DEG);
	renderString(-1.5,1.5-.4, GLUT_BITMAP_HELVETICA_12, strChar, 1,1,1);
	//switch (CurrentMode)
	//{

	//case 0:
	glRotated(90,-1,0,0);
	glRotated(90,0,0,1);
	glRotated(25,0,0,1);
	glRotated(25,1,0,0);
	//break;
	//}

	drawAxes();

	glLineWidth (1.0);
	glColor3f(0.7f,0.7f,0.7f);
	glutWireSphere(1,10,10);


	drawVector(vector[0],vector[1],vector[2],0,1,1);
	drawVector(forceVector[0],forceVector[1],forceVector[2],1-forceVector[3],0,forceVector[3]);
	glPopMatrix();

	// Flush the pipeline.  (Not usually necessary.)
	glFlush();

}
void SimpleTest::printStatus(double x,double y,double z){
	printf("--------\n");
	printf("(r = %.3f, ",r);
	printf("theta = %.3f, ",theta);
	printf("phi = %.3f)\n",phi);
	printf("x = r * cos(theta) * sin (phi) = %.3f\n",x);
	printf("y = r * sin(theta) * sin (phi) = %.3f\n",y);
	printf("z = r * cos (phi) = %.3f\n",z);
}
void SimpleTest::drawVector(double x, double y, double z, double r, double g, double b){
	glPushMatrix();
	glColor3f(r,g,b);
	glLineWidth (1.0);
	glBegin (GL_LINES);
	glVertex3d (0,0,0);
	glVertex3d( x, y, z);
	glEnd();
	glPopMatrix();
}
void SimpleTest::drawSize(double size, double maxSize, double r, double g, double b){
	double height = 2;
	glPushMatrix();
	glColor3f(1,1,1);
	glLineWidth (16.0);
	glBegin (GL_LINES);
	glVertex3d (0,-.05,0);
	glVertex3d(0,(height)+.05,0);
	glEnd();
	glColor3d(r,g,b);
	glLineWidth (14.0);
	glBegin (GL_LINES);
	glVertex3d (0,0,0);
	glVertex3d(0,height*size/maxSize,0);
	glEnd();
	glPopMatrix();
}
void SimpleTest::drawAxes(void){
	float size = 1;
	float ORG[3] = {0,0,0};
	float XP[3] = {size,0,0}, XN[3] = {-size,0,0},
		YP[3] = {0,size,0}, YN[3] = {0,-size,0},
		ZP[3] = {0,0,size}, ZN[3] = {0,0,-size};

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

void SimpleTest::renderString(float x, float y, void *font, const char* str, float r, float g, float b){  
	int i = 0;
	glColor3f(r,g,b); 
	glRasterPos2f(x, y);
	for (i = 0; str[i] != '\0'; i++) {
		glutBitmapCharacter(font, str[i]);
	}
}

// Initialize OpenGL's rendering modes
void SimpleTest::initRendering()
{

	glEnable ( GL_DEPTH_TEST );

	// Uncomment out the first block of code below, and then the second block,
	//		to see how they affect line and point drawing.
	/*
	// The following commands should cause points and line to be drawn larger
	//	than a single pixel width.
	glPointSize(8);
	glLineWidth(5);
	*/

	/*
	// The following commands should induce OpenGL to create round points and 
	//	antialias points and lines.  (This is implementation dependent unfortunately).
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// Make round points, not square points
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);		// Antialias the lines
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	*/
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

// Called when the window is resized
//		w, h - width and height of the window in pixels.
void SimpleTest::resizeWindow(int w, int h)
{
	double scale, center;
	double windowXmin, windowXmax, windowYmin, windowYmax;

	// Define the portion of the window used for OpenGL rendering.
	glViewport( 0, 0, w, h );	// View port uses whole window

	// Set up the projection view matrix: orthographic projection
	// Determine the min and max values for x and y that should appear in the window.
	// The complication is that the aspect ratio of the window may not match the
	//		aspect ratio of the scene we want to view.
	w = (w==0) ? 1 : w;
	h = (h==0) ? 1 : h;
	if ( (Xmax-Xmin)/w < (Ymax-Ymin)/h ) {
		scale = ((Ymax-Ymin)/h)/((Xmax-Xmin)/w);
		center = (Xmax+Xmin)/2;
		windowXmin = center - (center-Xmin)*scale;
		windowXmax = center + (Xmax-center)*scale;
		windowYmin = Ymin;
		windowYmax = Ymax;
	}
	else {
		scale = ((Xmax-Xmin)/w)/((Ymax-Ymin)/h);
		center = (Ymax+Ymin)/2;
		windowYmin = center - (center-Ymin)*scale;
		windowYmax = center + (Ymax-center)*scale;
		windowXmin = Xmin;
		windowXmax = Xmax;
	}

	// Now that we know the max & min values for x & y that should be visible in the window,
	//		we set up the orthographic projection.
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( windowXmin, windowXmax, windowYmin, windowYmax, -1, 1 );

}


void SimpleTest::resizeWindow2(int w, int h)
{
	glViewport( 0, 0, w, h );	// View port uses whole window
	glMatrixMode( GL_PROJECTION );
	double ratio = 1;//((double)w/(double)h)/ratioWin2;
	glLoadIdentity();
	glOrtho( Xmin, Xmax, Ymin, Ymax*ratio, -1, 1 );
}

// Main routine
// Set up OpenGL, define the callbacks and start the main loop
int SimpleTest::initSimpleTest( int argc, char** argv )
{
	glutInit(&argc,argv);

	// The image is not animated so single buffering is OK. 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );

	// Window position (from top corner), and size (width and hieght)
	glutInitWindowPosition( 20, 60 );
	glutInitWindowSize( 360, 360 );
	win1 = glutCreateWindow( "SimpleTest" );

	// Initialize OpenGL as we like it..
	initRendering();

	// Set up callback functions for key presses
	glutKeyboardFunc( myKeyboardFunc );			// Handles "normal" ascii symbols
	// glutSpecialFunc( mySpecialKeyFunc );		// Handles "special" keyboard keys

	// Set up the callback function for resizing windows
	glutReshapeFunc( resizeWindow );

	// Call this for background processing
	// glutIdleFunc( myIdleFunction );

	// call this whenever window needs redrawing
	glutDisplayFunc( drawScene );

	//window2
	glutInitWindowPosition( 380, 60 );
	glutInitWindowSize( 560, 360 );
	ratioWin2 = 560.0/360.0;
	win2 = glutCreateWindow( "SimpleTest2" );
	glDisable( GL_DEPTH_TEST );
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutReshapeFunc( resizeWindow2 );
	glutDisplayFunc( drawScene2 );


	glutMainLoop(  );

	return(0);	// This line is never reached.
}
