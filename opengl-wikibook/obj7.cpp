/**
 * cube6.cpp
 * Modern OpenGL programing example 
 * http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Introduction
 * 
 */
/**
 *  This code is broken
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

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

GLuint vbo_mesh_vertices;
GLuint program;
GLuint ibo_mesh_normals;
GLint  attribute_v_cord, attribute_v_normal;
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
void load_obj (const char *filename, vector<glm::vec4> &vertices, vector<glm::vec3> &normals, vector<GLushort> &elements);

int main (int argc, char* argv[])
{
  if (argc <= 1) {
    fprintf (stderr, "Need a filename\n");
    exit (EXIT_FAILURE);
  }
  
  vector<glm::vec4> obj_vertices;
  vector<glm::vec3> obj_normals;
  vector<GLushort> obj_elements;
  load_obj (argv[1], obj_vertices, obj_normals, obj_elements);

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

  // Shaders
  GLuint vs, fs;
  if ((vs = create_shader ("./shaders/obj7.v.glsl", GL_VERTEX_SHADER)) == 0) 
    exit (EXIT_FAILURE);
  if ((fs = create_shader ("./shaders/obj7.f.glsl", GL_FRAGMENT_SHADER)) == 0) 
    exit (EXIT_FAILURE);

  // Link it together
  /*
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
  const char *uniform_name = "mvp";
  uniform_mvp = glGetUniformLocation (program, uniform_name);
  if (uniform_mvp == -1) 
    {
      fprintf (stderr, "Could not bind uniform %s\n", uniform_name);
      exit (EXIT_FAILURE);
    }
  */
  return 1;
}

void onDisplay ()
{
  glClearColor (0.0, 0.0, 0.0, 1.0);
  glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glUseProgram (program);

  glEnableVertexAttributeArray (attribute_v_cord);
  //  Vertices
  glBindBuffer (GL_ARRAY_BUFFER, vbo_mesh_vertices);
  glVertexAttribPointer (
			 attribute_v_cord
			 4,
			 GL_FLOAT,
			 GL_FALSE,
			 0,
			 0);
  glBindBuffer (GL_ARRAY_BUFFER, vbo_mesh_normals);
  glVertexAttribPointer (
			 attribute_v_normal,
			 3,
			 GL_FLOAT,
			 GL_FALSE,
			 0,
			 0);

  //  Element array
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibo_mesh_elements);
  int size;
  glGetBufferParameteriv (GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements (GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
  glDisableVertexAttribArray (attribute_v_cord);
  glutSwapBuffers ();
}

void free_resources ()
{
  glDeleteProgram (program);
  glDeleteBuffers (1, &vbo_cube_vertices);
  glDeleteBuffers (1, &ibo_cube_elements);
  glDeleteTextures (1, &texture_id);
}
			 
void idle ()
{
  glm::mat4 model = glm::translate (glm::mat4 (1.0f), glm::vec3 (0.0, 0.0, -4.0));
  glm::mat4 view = glm::lookAt (
				glm::vec3 (0.0, 2.0, 4.0),
				glm::vec3 (0.0, 0.0, 0.0),
				glm::vec3 (0.0, 1.0, 0.0));
  glm::mat4 projection = glm::perspective (45.0f, 1.0f*screen_width/screen_height, 0.1f, 100.0f);
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
void load_obj (const char *filename, vector<glm::vec4> &vertices, vector<glm::vec3> &normals, vector<GLushort> &elements) {
  ifstream in(filename, ios::in);
  if (!in) {
    cerr << "Cannot open " << filename << endl;
    exit (1);
  }
  string line;
  while (getline (in, line)) {
    if (line.substr (0,2) == "v ") {
      istringstream s(line.substr(2));
      glm::vec4 v; 
      s >> v.x;
      s >> v.y;
      s >> v.z;
      v.w = 1.0f;
      vertices.push_back (v);
    }
    else if (line.substr (0,2) == "f ") {
      istringstream s(line.substr (2));
      GLushort a, b, c;
      s >> a;
      s >> b;
      s >> c;
      a--;
      b--;
      c--;
      elements.push_back (a);
      elements.push_back (b);
      elements.push_back (c);
    }
    else if (line[0] == '#') {
      //  Ignore this line
    }
    else {
      // Ignore this line
    }
  }
  normals.resize (mesh->vertices.size (), glm::vec3 (0.0, 0.0, 0.0));
  for (unsigned int i = 0; i < elements.size (); i+=3) {
    GLushort ia = elements[i];
    GLushort ib = elements[i+1];
    GLushort ic = elements[i+2];
    glm::vec3 normal = glm::normalize (glm::cross(
         glm::vec3 (vertices[ib]) - glm::vec3 (vertices[ia]),
	 glm::vec3 (vertices[ic]) - glm::vec3 (vertices[ia])));
    normals[ia] = normals[ib] = normals[ic] = normal;
  }
}

