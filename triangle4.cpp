/* triangle4.cpp
 * Modern OpenGL programing example 
 * http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Introduction
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/* Use glew.h instead of gl.h to get all of the GL prototypes declared */
#include <GL/glew.h>
#include <GL/glut.h> 
/* glm headers, http://glm.g-truc.net/ */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>
/* Helpers */
#include "common/shader_utils.h"

GLuint vbo_triangle, vbo_triangle_colors;
GLuint program;
GLint attribute_coord3d, attribute_v_color;
GLint uniform_fade;
GLint uniform_m_transform;

#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640

/* Function Declars */
int init_resources (void);
void onDisplay (void);
void free_resources (void);
void keyboard_char (unsigned char key, int x, int y);
void idle ();

struct attributes {
  GLfloat coord3d[3];
  GLfloat v_color[3];
};


int main (int argc, char* argv[])
{
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow ("triangle4");

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
      glutIdleFunc (idle);
      glutKeyboardFunc(keyboard_char);      
      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glutMainLoop ();
    }

  free_resources ();
  exit (EXIT_SUCCESS);
}

int init_resources (void)
{
  GLint link_ok = GL_FALSE;

  /* Combine into Vertices and Colors into Attributes */
  struct attributes triangle_attributes[] = {
    {{  0.0,  0.8,  0.0}, {1.0, 1.0, 0.0}},
    {{ -0.8, -0.8,  0.0}, {0.0, 0.0, 1.0}},
    {{  0.8, -0.8,  0.0},  {1.0, 0.0, 0.0}}
  };
  glGenBuffers (1, &vbo_triangle);
  glBindBuffer (GL_ARRAY_BUFFER, vbo_triangle);
  glBufferData (GL_ARRAY_BUFFER, sizeof (triangle_attributes), triangle_attributes, GL_STATIC_DRAW);
  glBindBuffer (GL_ARRAY_BUFFER, 0);
  /* Shaders */
  GLuint vs, fs;
  if ((vs = create_shader ("./shaders/triangle4.v.glsl", GL_VERTEX_SHADER)) == 0) 
    exit (EXIT_FAILURE);
  if ((fs = create_shader ("./shaders/triangle4.f.glsl", GL_FRAGMENT_SHADER)) == 0) 
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
  const char *attribute_name = "coord3d";
  attribute_coord3d = glGetAttribLocation (program, attribute_name);
  if (attribute_coord3d == -1)
    {
      fprintf (stderr, "Could not bind attribute %s\n", attribute_name);
      exit (EXIT_FAILURE);
    }
  attribute_name = "v_color";
  attribute_v_color = glGetAttribLocation (program, attribute_name);
  if (attribute_v_color == -1)
    {
      fprintf (stderr, "Could not bind attribute %s\n", attribute_name);
      exit (EXIT_FAILURE);
    }
  const char *uniform_name;
  uniform_name = "fade";
  uniform_fade = glGetUniformLocation (program, uniform_name);
  if (uniform_fade == -1)
    {
      fprintf (stderr, "Could not bind uniform %s\n", uniform_name);
      exit (EXIT_FAILURE);
    }
  uniform_name = "m_transform";
  uniform_m_transform = glGetUniformLocation (program, uniform_name);
  if (uniform_m_transform == -1) 
    {
      fprintf (stderr, "Could not bind uniform %s\n", uniform_name);
      exit (EXIT_FAILURE);
    }
  return 1;
}

void onDisplay ()
{
  glClearColor (0.0, 0.0, 0.0, 1.0);
  glClear (GL_COLOR_BUFFER_BIT);
  glUseProgram (program);
  glEnableVertexAttribArray (attribute_coord3d);
  glEnableVertexAttribArray (attribute_v_color);
  glBindBuffer (GL_ARRAY_BUFFER, vbo_triangle);
  glVertexAttribPointer (
			 attribute_coord3d,
			 3,
			 GL_FLOAT,
			 GL_FALSE,
			 sizeof (struct attributes),
			 0);
  glVertexAttribPointer (
			 attribute_v_color,
			 3,
			 GL_FLOAT,
			 GL_FALSE,
			 5 * sizeof (GLfloat),
			 (GLvoid*) (2*sizeof(GLfloat))
			 );
  glDrawArrays (GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray (attribute_v_color);
  glDisableVertexAttribArray (attribute_coord3d);
  glutSwapBuffers ();
}

void free_resources ()
{
  glDeleteProgram (program);
  glDeleteBuffers (1, &vbo_triangle);
}
			 
void idle ()
{
  float move = sinf (glutGet (GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) /5);
  float angle = glutGet (GLUT_ELAPSED_TIME) / 1000.0 * 45;  /* 45 degree per second */
  float cur_fade = sinf (glutGet (GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) /5) / 2 + 0.5;
  glm::vec3 axis_z (0, 0, 1);
  glm::mat4 m_transform = glm::translate (glm::mat4 (1.0f), glm::vec3 (move, 0.0, 0.0)) 
    * glm::rotate (glm::mat4 (1.0f), angle, axis_z);
  glUseProgram (program);
  glUniform1f (uniform_fade, cur_fade);
  glUniformMatrix4fv (uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
  glutPostRedisplay ();
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
