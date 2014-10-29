/*  exj_1.cpp
 *  James A. Feister - thegreatpissant@gmail.com
 */

#include <iostream>
#include <vector>
#include <queue>
#include <unistd.h>
using namespace std;

//  OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

// 3rd Party
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//  Engine parts
#include "common/shader_utils.h"
#include "Model.h"

queue <int> gqueue;

//  Constants and Vars  
//  @@TODO Should move into a variable system
int screenWidth = 640;
int screenHeight = 480;
glm::mat4 Projection;

//  Function Declarations
void Init ();
void GlutIdle ();
void GlutReshape ( int newWidth, int newHeight );
void GlutDisplay ( void );
void GlutKeyboard ( unsigned char key, int x, int y );
void UpdatePerspective ();
void CleanupAndExit ();

int main ( int argc, char ** argv) {
  glutInit ( &argc, argv );
  glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA );
  glutInitWindowSize ( screenWidth, screenHeight );

  glutCreateWindow (argv[0]);
  if ( glewInit() ) {
    std::cerr << "Unable to initialize GLEW ... exiting " << std::endl;
    exit ( EXIT_FAILURE );
  }
  
  Init ();

  glutIdleFunc ( GlutIdle );
  glutReshapeFunc ( GlutReshape );
  glutDisplayFunc ( GlutDisplay );
  glutKeyboardFunc ( GlutKeyboard );
  
  glutMainLoop ();

}

void Init () {
  //  Setup a Thread Safe Queue
  //  Setup thread for game timer
}

void GlutReshape ( int newWidth, int newHeight ) {
  screenWidth = newWidth;
  screenHeight = newHeight;
  UpdatePerspective ();
  glutPostRedisplay ();
}

void GlutDisplay ( void ) {
  glClear (GL_COLOR_BUFFER_BIT);
  glViewport ( 0, 0, screenWidth, screenHeight );
  //
  //  Call on rederer to render Entities
  //
  glFinish ();
  glutSwapBuffers ();
}

void GlutKeyboard ( unsigned char key, int x, int y ) {
  switch ( key ) {
  default:
    break;
  case 27:
  case 'q':
  case 'Q':
    CleanupAndExit ();
    break;
  case 'a':
  case 'A':
    gqueue.push ( 1 );
    break;
  case 's':
  case 'S':
    gqueue.push ( 2 );
    break;
  case 'd':
  case 'D':
    gqueue.push ( 3 );
    break;
  case 'w':
  case 'W':
    gqueue.push ( 4 );
    break;
  }
}

void UpdatePerspective () {
  //  @@TODO Adjustable FOV and Aspect ration
  GLfloat hResolution = screenWidth;  //  640.0f;
  GLfloat vResolution = screenHeight; //  800.0f;
  //  GLfloat hScreenSize = 0.14976f;
  //  GLfloat vScreenSize = 0.0935f;
  GLfloat eyeScreenDist = 0.041f;
  GLfloat aspect = hResolution / (2.0f * vResolution);
  GLfloat fov = 2.0f*(atan(0.0935f/(2.0f*eyeScreenDist)));
  GLfloat zNear  = 0.3f;
  GLfloat zFar   = 1000.0f;
  Projection = glm::perspective( fov, aspect, zNear, zFar );
}

void GlutIdle () {
  //  Pump the events loop
  while ( !gqueue.empty () ) {
    cout << "queue item: " << gqueue.front() << endl;
    gqueue.pop ();
  }
  cout << "--------Queue empty----------" << endl;
  glutPostRedisplay ();
  usleep ( 1000000 );
}

void CleanupAndExit () {
  exit ( EXIT_SUCCESS );
}

//  Next version exj_2.cpp
/*
 * Adjustable fov and aspect ratio
 * Var system
 */
