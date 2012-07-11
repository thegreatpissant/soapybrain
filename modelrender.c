/* Render md5 models, specifically those from doom3 */

#include "../md5reader/md5reader.h"

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

#define GWH 800

void glut_display_func (void)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity ();


  glEnd ();
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

void glut_mouse_func (int button, int state, int x, int y)
{
}

void glut_idle_func ()
{
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
  pskeleton mySkeleton;
  mySkeleton = md5mesh_loadfile (md5mesh_file);
      
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (GWH, GWH);
  glutInitWindowPosition (200, 100);
  
  int window = glutCreateWindow ("Model Render");
  init_view();
  glutDisplayFunc (glut_display_func);
  glutReshapeFunc (glut_reshape_func);
  glutMouseFunc   (glut_mouse_func);
  glutIdleFunc    (glut_idle_func);

  glutMainLoop ();
  skeletonCleanUp (mySkeleton);
  return 0;
}
