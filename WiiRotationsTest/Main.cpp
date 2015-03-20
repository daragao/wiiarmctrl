/*#include <stdio.h>
#include "glWindow.h"


//MAIN
int main(int argc, char *argv[]) {

	//--------
	printf("\n\n--------- OpenGL :D ---------\n\n");
	//glWindow *glWin = new glWindow(argc, argv);

	return 0;
}
*/

#include <iostream>
#include <cstdlib>
#include <GL/glut.h>
using namespace std;

// function prototypes
void disp(void);
void keyb(unsigned char key, int x, int y);
void drawVector(double point[3], double color[3]);


// window identifier
static int win;

int main(int argc, char **argv){
 
  //////////
  // INIT //
  //////////

  // initialize glut
  glutInit(&argc, argv);

  // specify the display mode to be RGB and single buffering 
  // we use single buffering since this will be non animated
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

  // define the size
  glutInitWindowSize(500,500);

  // the position where the window will appear
  glutInitWindowPosition(100,100);
  

  // if we would want fullscreen:
  // glutFullScreen();

  // create the window, set the title and keep the 
  // window identifier.
  win = glutCreateWindow("Yet another teapot");

  //////////////
  // CALLBACK //
  //////////////
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-30.0, 30.0, -30.0, 30.0, -30.0, 30.0);
	glRotatef(30.0, 1.0, 0.0, 0.0); 
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

  glutDisplayFunc(disp);
  glutKeyboardFunc(keyb);
  
  ////////////
  // OPENGL //
  ////////////

  // define the color we use to clearscreen 
  glClearColor(0.0,0.0,0.0,0.0);



  // enter the main loop
  glutMainLoop();

  return 0;
}


void disp(void){

  // do  a clearscreen
  glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity(); // Reset current matrix (Modelview)

  
  // draw something

  //glutWireTeapot(0.5);
  // glutSolidTeapot(0.5);
  // glutWireSphere(0.5,10,100);
  // glutSolidSphere(0.5,100,100);
  // glutWireTorus(0.3,0.5,100,100);
  // glutSolidTorus(0.3,0.5,100,100);
  // glutWireIcosahedron();
  // glutSolidIcosahedron();
  // glutWireDodecahedron();
  // glutSolidDodecahedron();
  // glutWireCone(0.5,0.5,100,100);
  // glutSolidCone(0.5,0.5,100,100);
  // glutWireCube(0.5);
  // glutSolidCube(0.5);
  double point[3] = {0,1,0};
  double color[3] = {0,0,1};
  drawVector(point,color);
}

void keyb(unsigned char key, int x, int y){
  cout << "Pressed key " << key << " on coordinates (" << x << "," << y << ")";
  cout << endl;
  if(key == 'q'){
    cout << "Got q,so quitting " << endl;
    glutDestroyWindow(win);
    exit(0);
  }
}

void drawVector(double point[3], double color[3]){
	glColor3f(color[0],color[1],color[2]);
	glLineWidth (1.0);
	glBegin (GL_LINES);
	glVertex3d (0,0,0);
	glVertex3d( point[0],point[1],point[2]);
	glEnd();
}