/* Render md5 models, specifically those from doom3 */

#include "../md5reader/md5reader.h"

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <X11/Xlib.h>

void  init_view () 
{
  glClearColor (0.0, 0.0, 0.0, 1.0);
  glShadeModel (GL_SMOOTH);
  glEnagle (GL_DEPTH_TEST);
}

void glut_reshape_func (int w, int h)
{
  glViewPort (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (60, 1.0, 0.5, 120);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
}

void glut_mouse_func (int button, int state, int x, int y)
{
}

void glut_idle_func ()
{
}

int main (int argc, char ** argv)
{

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (GWH, GWH);
  glutInitWinodwPosition (200, 100);
  
  int window = glutCreateWindow ("Model Render");
  init_view();
  glutDisplayFunc (glut_display_func);
  glutReshapeFunc (glut_reshape_func);
  glutMouseFunc   (glut_mouse_func);
  glutIdleFunc    (glut_idle_func);

  glutMainLoop ();
  return 0;
}
