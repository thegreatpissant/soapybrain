/* test1.cpp
 * First test of a cpp opengl app
 * James A. Feister thegreatpissant
 * This a combination of the tutorials in my git repo
 * Style guideline as per idsoftware except for comments and class names
 */

#include <GL/glew.h>
#include <GL/freeglut.h>

/* glm headers, http://glm.g-truc.net/ 
 *  found in opengl-wiki examples
 */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/* 
 * Helper functions for shaders 
 * As per opengl-wiki examples as well.  
 * I liked these the best
 */
#include "common/shader_utils.h"


/*
 * The setup of glut is a combination of both.
 */

#define WINDOW_TITLE_PREFIX "Test 1"

int currentWindowWidth = 800;
int currentWindowHeight = 600;
int currentWindowHandle = 0;

void Initialize ( int, char *[] );
void InitWindow ( int, char *[] );
void ResizeFunction ( int, int );
void RenderFunction ( void );
void KeyboardFunction ( unsigned char key, int x, int y );

void GlewErrorCheck ( GLenum );

int main ( int argc, char * argv[] )
{
  Initialize ( argc, argv );
  glutMainLoop ( );
  return EXIT_SUCCESS;
}

void Initialize ( int argc, char * argv[] )
{
  GLenum glewInitResult;
  InitWindow ( argc, argv );

  //  This is optional, find out why and make it a flag
  glewExperimental = GL_TRUE;
  glewInitResult = glewInit ( );
  GlewErrorCheck ( glewInitResult );
}

void InitWindow ( int argc, char *argv[] )
{
  glutInit ( &argc, argv );
  glutInitContextVersion ( 3, 3 );
  glutInitContextFlags ( GLUT_FORWARD_COMPATIBLE );
  glutInitContextProfile (GLUT_CORE_PROFILE );
  glutSetOption ( GLUT_ACTION_ON_WINDOW_CLOSE,
		  GLUT_ACTION_GLUTMAINLOOP_RETURNS );
  glutInitWindowSize ( currentWindowWidth, currentWindowHeight );
  glutInitDisplayMode ( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
  currentWindowHandle = glutCreateWindow ( WINDOW_TITLE_PREFIX );

  if ( currentWindowHandle < 1 ) {
    fprintf ( stderr,
	      "ERROR: Could not create a new rendering window, \n");
    exit ( EXIT_FAILURE );
  }
  
  glutReshapeFunc ( ResizeFunction );
  glutDisplayFunc ( RenderFunction );
  //  glutIdleFunc    ( IdleFunction );
  //  glutTimerFunc   ( 0 , TimerFunction, 0 );
  glutKeyboardFunc (KeyboardFunction );
  //  glutCloseFunc    ( DestroyFunction );
}

void ResizeFunction ( int width, int height )
{
  currentWindowWidth = width;
  currentWindowHeight = height;

  glViewport ( 0, 0, currentWindowWidth, currentWindowHeight );
  
}

void RenderFunction ( void )
{
  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glutSwapBuffers ( );
  glutPostRedisplay ( );
}

void KeyboardFunction ( unsigned char key, int x, int y )
{
  switch(key) {
  case 27:
    {
      exit(0);
    }
    break;

  default:
    break;
  }
}

void GlewErrorCheck ( GLenum glewReturnCode ) 
{
  switch ( glewReturnCode ) {
  case GLEW_OK:
    return;
    break;

  default: 
    {
      fprintf (stderr,
	       "ERROR: %s\n",
	       glewGetErrorString (glewReturnCode) );
      exit (EXIT_FAILURE);
    } 
    break;
  }
}
