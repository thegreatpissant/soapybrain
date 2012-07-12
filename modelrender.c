/* Render md5 models, specifically those from doom3 */

#include "../md5reader/md5reader.h"

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include "charset.h"
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define GWH 800

pskeleton mySkeleton;
#define ZCORD -50.0
#define XCORD 0.0
#define YCORD 0.0
GLfloat zCord = ZCORD;
GLfloat xCord = XCORD;
GLfloat yCord = YCORD;
GLint xmouse_press = 0;
GLint ymouse_press = 0;
GLfloat x_rotate = 0.0, y_rotate = 0.0;
GLboolean rotate_obj = GL_FALSE;

void glut_display_func (void)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity ();
  glTranslatef (xCord, yCord, zCord);
 
  glPushMatrix (); {
    glRotatef (y_rotate, 0.0, 1.0, 0.0);
    glRotatef (x_rotate, 1.0, 0.0, 0.0);
   
    glBegin (GL_POINTS);
    glColor3f( 1.0, 1.0f, 1.0f);
    int dxi;
    for (dxi = 0; dxi < mySkeleton->numJoints; dxi++)
      {
	glVertex3f (mySkeleton->joints[dxi]->posX,
		    mySkeleton->joints[dxi]->posY,
		    mySkeleton->joints[dxi]->posZ);
      }

    glEnd ();
  }  glPopMatrix ();
  glutSwapBuffers ();
}

void  init_view () 
{
  glClearColor (0.0, 0.0, 0.0, 1.0);
  glShadeModel (GL_SMOOTH);
  glEnable (GL_DEPTH_TEST);
}

void glut_reshape_func (int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (60, 1.0, 0.5, 120);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}

void glut_idle_func ()
{
}

void glut_mouse_func ( int button, int state, int x, int y)
{
  switch (button)
    {
    case GLUT_DOWN:
      xmouse_press = x;
      ymouse_press = y;
      rotate_obj = GL_TRUE;
      break;
    case GLUT_UP:
      rotate_obj = GL_FALSE;
      break;
    default:
      break;
    }
  glutPostRedisplay ();
}

void glut_motion_func (int x, int y)
{
  x_rotate += (GLfloat)((xmouse_press - x) *0.5);
  y_rotate += (GLfloat)((ymouse_press - y) *0.5);
  glutPostRedisplay ();
}

void glut_keyboard_func (unsigned char key, int x, int y)
{
  switch (key)
    {
    case 27:
      exit (0);
    case 's':
      zCord -= 1.0f;
      break;
    case 'w':
      zCord += 1.0f;
      break;
    case 'a':
      xCord -= 1.0f;
      break;
    case 'd':
      xCord += 1.0f;
      break;
    case 't':
      yCord += 1.0f;
      break;
    case 'g':
      yCord -= 1.0f;
      break;
    default:
      break;
    }
  glutPostRedisplay ();
}

int main (int argc, char ** argv)
{

  if (argc <= 1)
    {
      fprintf (stderr, "File name not specified\n");
      exit (EXIT_FAILURE);
    }
  FILE * md5mesh_file;
  md5mesh_file = fopen (argv[1], "r");
  if (md5mesh_file == NULL)
    {
      fprintf (stderr, "Opening file \"%s\", failed\n", argv[1]);
      exit (EXIT_FAILURE);
    }

  mySkeleton = md5mesh_loadfile (md5mesh_file);
      
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (GWH, GWH);
  glutInitWindowPosition (200, 100);
  
  glutCreateWindow ("Model Render");
  init_view();
  glutDisplayFunc (glut_display_func);
  glutReshapeFunc (glut_reshape_func);
  glutIdleFunc    (glut_idle_func);
  glutKeyboardFunc(glut_keyboard_func);
  glutMouseFunc   (glut_mouse_func);
  glutMotionFunc  (glut_motion_func);
  glutMainLoop ();
  skeletonCleanUp (mySkeleton);
  return 0;
}
