/* cube5.cpp
 * Modern OpenGL programming example 
 * http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Introduction
 * 
 */
#include <mpi.h>
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
/* Helpers */
#include "common/shader_utils.h"

GLuint vbo_cube_vertices, vbo_cube_colors;
GLuint ibo_cube_elements;
GLuint program, program2;
GLint attribute_coord3d, attribute_v_color;
GLint uniform_m_transform;
GLint uniform_mvp;


#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800
int screen_width = WINDOW_WIDTH, screen_height = WINDOW_HEIGHT;
/* Function Declars */
int init_resources (void);
void onDisplay (void);
void free_resources (void);
void keyboard_char (unsigned char key, int x, int y);
void idle ();
void onReshape (int width, int height);
struct attributes {
  GLfloat coord3d[3];
  GLfloat v_color[3];
};


int main (int argc, char* argv[])
{
MPI_Init ( 0, 0 );

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize (screen_width, screen_height);
  glutCreateWindow ("cube5");

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
      glutReshapeFunc (onReshape);
      glutKeyboardFunc(keyboard_char);      
      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable (GL_DEPTH_TEST);
      glutMainLoop ();
    }

  free_resources ();
  exit (EXIT_SUCCESS);
MPI_Finalize ();
}

int init_resources (void)
{
  GLint link_ok = GL_FALSE;

  /* Combine into Vertices and Colors into Attributes */
  GLfloat cube_vertices[] = {
    // front
    -1.0, -1.0, 1.0,
    1.0, -1.0, 1.0,
    1.0, 1.0, 1.0,
    -1.0, 1.0, 1.0,
    // back
    -1.0, -1.0, -1.0,
    1.0, -1.0, -1.0,
    1.0, 1.0, -1.0,
    -1.0, 1.0, -1.0,
  };
  GLfloat cube_colors[] = {
    // front colors
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
    // back colors
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 1.0, 1.0,
  };
  GLushort cube_elements[] = {
    // front
    0, 1, 2,
    2, 3, 0,
    // top
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // botom
    4, 0, 3, 
    3, 7, 4, 
    // left
    4, 5, 1, 
    1, 0, 4,
    // right
    3, 2, 6,
    6, 7, 3,
  };
  glGenBuffers (1, &ibo_cube_elements);
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
  glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (cube_elements), cube_elements, GL_STATIC_DRAW);
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

  glGenBuffers (1, &vbo_cube_vertices);
  glBindBuffer (GL_ARRAY_BUFFER, vbo_cube_vertices);
  glBufferData (GL_ARRAY_BUFFER, sizeof (cube_vertices), cube_vertices, GL_STATIC_DRAW);
  glBindBuffer (GL_ARRAY_BUFFER, 0);

  glGenBuffers (1, &vbo_cube_colors);
  glBindBuffer (GL_ARRAY_BUFFER, vbo_cube_colors);
  glBufferData (GL_ARRAY_BUFFER, sizeof (cube_colors), cube_colors, GL_STATIC_DRAW);
  glBindBuffer (GL_ARRAY_BUFFER, 0);


  /* Shaders */
  GLuint vs, fs, vs2;
  if ((vs = create_shader ("./shaders/cube5.v.glsl", GL_VERTEX_SHADER)) == 0) 
    exit (EXIT_FAILURE);
  if ((vs2 = create_shader ("./shaders/cube52.v.glsl", GL_VERTEX_SHADER)) == 0) 
    exit (EXIT_FAILURE);
  if ((fs = create_shader ("./shaders/cube5.f.glsl", GL_FRAGMENT_SHADER)) == 0) 
    exit (EXIT_FAILURE);

  /* Link it together */
  program2 = glCreateProgram ();
  glAttachShader (program2, vs2);
  glAttachShader (program2, fs);
  glLinkProgram (program2);
  glGetProgramiv (program2, GL_LINK_STATUS, &link_ok);
  if (!link_ok)
    {
      fprintf (stderr, "glLinkProgram:");
      print_log (program);
    }
    const char *attribute_name = "coord3d";
  attribute_coord3d = glGetAttribLocation (program2, attribute_name);
  if (attribute_coord3d == -1)
    {
      fprintf (stderr, "Could not bind attribute %s\n", attribute_name);
      exit (EXIT_FAILURE);
    }
  attribute_name = "v_color";
  attribute_v_color = glGetAttribLocation (program2, attribute_name);
  if (attribute_v_color == -1)
    {
      fprintf (stderr, "Could not bind attribute %s\n", attribute_name);
      exit (EXIT_FAILURE);
    }
  const char *uniform_name;
  uniform_name = "mvp";
  uniform_mvp = glGetUniformLocation (program2, uniform_name);
  if (uniform_mvp == -1) 
    {
      fprintf (stderr, "Could not bind uniform %s\n", uniform_name);
      exit (EXIT_FAILURE);
    }

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
  attribute_name = "coord3d";
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
  /*
    uniform_name = "m_transform";
    uniform_m_transform = glGetUniformLocation (program, uniform_name);
    if (uniform_m_transform == -1) 
    {
    fprintf (stderr, "Could not bind uniform %s\n", uniform_name);
    exit (EXIT_FAILURE);
    }
  */
  uniform_name = "mvp";
  uniform_mvp = glGetUniformLocation (program, uniform_name);
  if (uniform_mvp == -1) 
    {
      fprintf (stderr, "Could not bind uniform %s\n", uniform_name);
      exit (EXIT_FAILURE);
    }
  return 1;
}

void onDisplay ()
{
  glClearColor (0.0, 0.0, 0.0, 1.0);
  glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glUseProgram (program);
  glEnableVertexAttribArray (attribute_coord3d);
  glEnableVertexAttribArray (attribute_v_color);

  //  Vertices
  glBindBuffer (GL_ARRAY_BUFFER, vbo_cube_vertices);
  glVertexAttribPointer (
			 attribute_coord3d,
			 3,
			 GL_FLOAT,
			 GL_FALSE,
			 3*sizeof (GLfloat),
			 0);
  //  Colors
  glBindBuffer (GL_ARRAY_BUFFER, vbo_cube_colors);
  glVertexAttribPointer (
			 attribute_v_color,
			 3,
			 GL_FLOAT,
			 GL_FALSE,
			 3*sizeof (GLfloat),
			 (GLvoid*) (2*sizeof(GLfloat))
			 );

  //  Element array
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
  int size;
  glGetBufferParameteriv (GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

  glDrawElements (GL_TRIANGLES, size/sizeof (GLushort), GL_UNSIGNED_SHORT, 0);
  //glDrawArrays (GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray (attribute_v_color);

  glDisableVertexAttribArray (attribute_coord3d);
  glUseProgram (program2);
  glEnableVertexAttribArray (attribute_coord3d);
  glEnableVertexAttribArray (attribute_v_color);

  //  Vertices
  glBindBuffer (GL_ARRAY_BUFFER, vbo_cube_vertices);
  glVertexAttribPointer (
			 attribute_coord3d,
			 3,
			 GL_FLOAT,
			 GL_FALSE,
			 3*sizeof (GLfloat),
			 0);
  //  Colors
  glBindBuffer (GL_ARRAY_BUFFER, vbo_cube_colors);
  glVertexAttribPointer (
			 attribute_v_color,
			 3,
			 GL_FLOAT,
			 GL_FALSE,
			 3*sizeof (GLfloat),
			 (GLvoid*) (2*sizeof(GLfloat))
			 );

  //  Element array
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
  glGetBufferParameteriv (GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

  glDrawElements (GL_TRIANGLES, size/sizeof (GLushort), GL_UNSIGNED_SHORT, 0);
  //glDrawArrays (GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray (attribute_v_color);
  glDisableVertexAttribArray (attribute_coord3d);

  glutSwapBuffers ();
}

void free_resources ()
{
  glDeleteProgram (program);
  glDeleteBuffers (1, &vbo_cube_vertices);
  glDeleteBuffers (1, &vbo_cube_colors);
  glDeleteBuffers (1, &ibo_cube_elements);
}
			 
void idle ()
{
  glm::mat4 model = glm::translate (glm::mat4 (1.0f), glm::vec3 (0.0, 0.0, -4.0));
  glm::mat4 view = glm::lookAt (glm::vec3 (0.0, 2.0, 0.0), glm::vec3 (0.0, 0.0, -4.0), glm::vec3 (0.0, 1.0, 0.0));
  glm::mat4 projection = glm::perspective (45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
  float angle = glutGet (GLUT_ELAPSED_TIME) / 1000.0 * 45;  // 45 degree per second
  glm::vec3 axis_y (0, 1.0, 0);
  glm::vec3 axis_z (0, 0, 1.0);
  glm::mat4 anim = glm::rotate (glm::mat4 (1.0f), angle, axis_y);
  glm::mat4 mvp = projection * view * model * anim;
  float move = sinf (glutGet (GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) /5);
  glm::mat4 anim2 = glm::translate (glm::mat4 (1.0f), glm::vec3 (move, 0.0, 0.0)) 
    * glm::rotate (glm::mat4 (1.0f), angle, axis_z);
  //  glm::mat4 anim2 = glm::rotate (glm::mat4 (1.0f), angle, axis_z);
  glm::mat4 mvp2 = projection * view * model * anim2;
  
  /*  
      float move = sinf (glutGet (GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) /5);
      float angle = glutGet (GLUT_ELAPSED_TIME) / 1000.0 * 45;  // 45 degree per second
      glm::vec3 axis_y (0, 1.0, 0);
      glm::mat4 m_transform = glm::translate (glm::mat4 (1.0f), glm::vec3 (move, 0.0, 0.0)) 
      * glm::rotate (glm::mat4 (1.0f), angle, axis_y);
  */
  glUseProgram (program);
  //  glUniformMatrix4fv (uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
  glUniformMatrix4fv (uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
  glUseProgram (program2);
  glUniformMatrix4fv (uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp2));
glutPostRedisplay ();
}
void onReshape (int width, int height)
{
  screen_width = width;
  screen_height = height;
  glViewport (0, 0, screen_width, screen_height);
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
