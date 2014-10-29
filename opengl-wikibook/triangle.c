
/* triangle.c
 * Modern OpenGL programming example 
 * http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Introduction
 * 
 */

#include <stdio.h>
#include <stdlib.h>
/* Use glew.h instead of gl.h to get all of the GL prototypes declared */
#include <GL/glew.h>
#include <GL/glut.h> 
#include "common/shader_utils.h"

GLuint vbo_triangle;
GLuint program;
GLint attribute_coord2d;

  /* vertices */
  GLfloat triangle_vertices[] = {
    0.0,  0.8,
    -0.8, -0.8,
    0.8, -0.8
  };
 
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

/* Function Declars */
int init_resources (void);
void onDisplay (void);
void free_resources (void);
void keyboard_char (unsigned char key, int x, int y);

int main (int argc, char* argv[])
{
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow ("triangle");

  /* Init glew */
  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK)
    {
      fprintf (stderr, "Error: %s\n", glewGetErrorString (glew_status));
      exit (EXIT_FAILURE);
    }

  /* Check version of OpenGL */
  if (!GLEW_VERSION_2_0) {
    fprintf (stderr, "Error: your graphics card does not support OpenGL 2.0\n");
    exit (EXIT_FAILURE);
  }

  if (1 == init_resources() )
    {
      glutDisplayFunc (onDisplay);
      glutKeyboardFunc(keyboard_char);      
      glutMainLoop ();
    }

  free_resources ();
  exit (EXIT_SUCCESS);
}

int init_resources (void)
{
  GLint link_ok = GL_FALSE;
  
  /* Shaders */
  GLuint vs, fs;
  if ((vs = create_shader ("./shaders/triangle.v.glsl", GL_VERTEX_SHADER)) == 0) 
    exit (EXIT_FAILURE);
  if ((fs = create_shader ("./shaders/triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) 
    exit (EXIT_FAILURE);

  /* Link it together */
  program = glCreateProgram ();
  glAttachShader (program, vs);
  glAttachShader (program, fs);
  glLinkProgram (program);
  glGetProgramiv (program, GL_LINK_STATUS, &link_ok);
  if (!link_ok)
    {
      fprintf (stderr, "glLinkProgram:");
      print_log (program);
    }
  const char *attribute_name = "coord2d";
  attribute_coord2d = glGetAttribLocation (program, attribute_name);
  if (attribute_coord2d == -1)
    {
      fprintf (stderr, "Could not bind attribute %s\n", attribute_name);
      exit (EXIT_FAILURE);
    }

  return 1;
}

void onDisplay ()
{
  glClearColor (0.0, 0.0, 0.0, 1.0);
  glClear (GL_COLOR_BUFFER_BIT);
  glUseProgram (program);
  glEnableVertexAttribArray (attribute_coord2d);
  glVertexAttribPointer ( attribute_coord2d,
			  2,
			  GL_FLOAT,
			  GL_FALSE,
			  0,
			  triangle_vertices
			  ); 
  glDrawArrays (GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray (attribute_coord2d);
  glutSwapBuffers ();
}

void free_resources ()
{
  glDeleteProgram (program);
}
			 
void keyboard_char (unsigned char key, int x, int y)
/*
 * Handles all Character key events.
 */
{
  switch(key) {
  case 27:
    exit(0);
    break;
  default:
    break;
  }
}
