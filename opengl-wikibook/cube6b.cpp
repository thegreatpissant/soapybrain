/* cube6.cpp
 * Modern OpenGL programming example 
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
#include "res_texture.c"

GLuint vbo_cube_vertices, vbo_cube_texcoords;
GLuint ibo_cube_elements;
GLuint program;
GLint attribute_coord3d, attribute_texcoord;
GLint uniform_m_transform;
GLint uniform_mvp;
GLuint texture_id;
GLint uniform_mytexture;

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

int main (int argc, char* argv[])
{
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize (screen_width, screen_height);
  glutCreateWindow ("cube6");

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
}

int init_resources (void)
{
  GLint link_ok = GL_FALSE;
  GLfloat cube_vertices[] = {
    //  front
    -1.0, -1.0, 1.0,
    1.0, -1.0, 1.0,
    1.0, 1.0, 1.0,
    -1.0, 1.0, 1.0,
    // top
    -1.0,  1.0,  1.0,
    1.0,  1.0,  1.0,
    1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,
    // back
    1.0, -1.0, -1.0,
    -1.0, -1.0, -1.0,
    -1.0,  1.0, -1.0,
    1.0,  1.0, -1.0,
    // bottom
    -1.0, -1.0, -1.0,
    1.0, -1.0, -1.0,
    1.0, -1.0,  1.0,
    -1.0, -1.0,  1.0,
    // left
    -1.0, -1.0, -1.0,
    -1.0, -1.0,  1.0,
    -1.0,  1.0,  1.0,
    -1.0,  1.0, -1.0,
    // right
    1.0, -1.0,  1.0,
    1.0, -1.0, -1.0,
    1.0,  1.0, -1.0,
    1.0,  1.0,  1.0,
  };
  GLushort cube_elements[] = {
    // front
    0,  1,  2,
    2,  3,  0,
    // top
    4,  5,  6,
    6,  7,  4,
    // back
    8,  9, 10,
    10, 11,  8,
    // bottom
    12, 13, 14,
    14, 15, 12,
    // left
    16, 17, 18,
    18, 19, 16,
    // right
    20, 21, 22,
    22, 23, 20,
  };
  GLfloat cube_texcoords[2*4*6] = {
    // front
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
  };
  for (int i = 1; i < 6; i++)
    {
      memcpy (&cube_texcoords[i*4*2], &cube_texcoords[0], 2*4*sizeof (GLfloat));
    }
	    
  glGenBuffers (1, &vbo_cube_texcoords);
  glBindBuffer (GL_ARRAY_BUFFER, vbo_cube_texcoords);
  glBufferData (GL_ARRAY_BUFFER, sizeof (cube_texcoords), cube_texcoords, GL_STATIC_DRAW);

  glGenBuffers (1, &ibo_cube_elements);
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
  glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (cube_elements), cube_elements, GL_STATIC_DRAW);
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

  glGenBuffers (1, &vbo_cube_vertices);
  glBindBuffer (GL_ARRAY_BUFFER, vbo_cube_vertices);
  glBufferData (GL_ARRAY_BUFFER, sizeof (cube_vertices), cube_vertices, GL_STATIC_DRAW);
  glBindBuffer (GL_ARRAY_BUFFER, 0);

  /* Texture */
  glGenTextures (1, &texture_id);
  glBindTexture (GL_TEXTURE_2D, texture_id);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D (GL_TEXTURE_2D,
		0,
		GL_RGB,
		res_texture.width,
		res_texture.height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		res_texture.pixel_data);

  /* Shaders */
  GLuint vs, fs;
  if ((vs = create_shader ("./shaders/cube6.v.glsl", GL_VERTEX_SHADER)) == 0) 
    exit (EXIT_FAILURE);
  if ((fs = create_shader ("./shaders/cube6.f.glsl", GL_FRAGMENT_SHADER)) == 0) 
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
  attribute_name = "texcoord";
  attribute_texcoord = glGetAttribLocation (program, attribute_name);
  if (attribute_texcoord == -1)
    {
      fprintf (stderr, "Could Not bind attribute %s\n", attribute_name);
      exit (EXIT_FAILURE);
    }
  const char *uniform_name = "mvp";
  uniform_mvp = glGetUniformLocation (program, uniform_name);
  if (uniform_mvp == -1) 
    {
      fprintf (stderr, "Could not bind uniform %s\n", uniform_name);
      exit (EXIT_FAILURE);
    }
  glUseProgram (program);
  glEnableVertexAttribArray (attribute_coord3d);
  glEnableVertexAttribArray (attribute_texcoord);

  //  Vertices
  glBindBuffer (GL_ARRAY_BUFFER, vbo_cube_vertices);
  glVertexAttribPointer (
			 attribute_coord3d,
			 3,
			 GL_FLOAT,
			 GL_FALSE,
			 3*sizeof (GLfloat),
			 0);
  //  Texture
  glBindBuffer (GL_ARRAY_BUFFER, vbo_cube_texcoords);
  glVertexAttribPointer (
			 attribute_texcoord,
			 2,
			 GL_FLOAT,
			 GL_FALSE,
			 0,
			 0
			 );
  glActiveTexture (GL_TEXTURE0);
  return 1;

}

void onDisplay ()
{
  glClearColor (0.0, 0.0, 0.0, 1.0);
  glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glBindTexture (GL_TEXTURE_2D, texture_id);
  glUniform1i (uniform_mytexture, 0);

  //  Element array
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
  int size;
  glGetBufferParameteriv (GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  //  glDrawElements (GL_TRIANGLES, size/sizeof (GLushort), GL_UNSIGNED_SHORT, 0);
  //  glDrawArrays (GL_TRIANGLES, 0, 3);
  glDrawElements (GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
  glutSwapBuffers ();
}

void free_resources ()
{ 
  glDisableVertexAttribArray (attribute_coord3d);
  glDisableVertexAttribArray (attribute_texcoord);
  glDeleteProgram (program);
  glDeleteBuffers (1, &vbo_cube_vertices);
  glDeleteBuffers (1, &ibo_cube_elements);
  glDeleteTextures (1, &texture_id);
}
			 
void idle ()
{
  glm::mat4 model = glm::translate (glm::mat4 (1.0f), glm::vec3 (0.0, 0.0, -4.0));
  glm::mat4 view = glm::lookAt (glm::vec3 (0.0, 2.0, 0.0), glm::vec3 (0.0, 0.0, -4.0), glm::vec3 (0.0, 1.0, 0.0));
  glm::mat4 projection = glm::perspective (45.0f, 1.0f*screen_width/screen_height, 0.1f, 10.0f);
  float angle = glutGet (GLUT_ELAPSED_TIME) / 1000.0 * 45;  // 45 degree per second
  glm::vec3 axis_y (0, 1.0, 0);
  glm::mat4 anim = \
    glm::rotate(glm::mat4(1.0f), angle*3.0f, glm::vec3(1, 0, 0)) *  // X axis
    glm::rotate(glm::mat4(1.0f), angle*2.0f, glm::vec3(0, 1, 0)) *  // Y axis
    glm::rotate(glm::mat4(1.0f), angle*4.0f, glm::vec3(0, 0, 1));   // Z axis
  glm::mat4 mvp = projection * view * model * anim;
  //  float move = sinf (glutGet (GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) /5);
  
  glUseProgram (program);
  glUniformMatrix4fv (uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
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
