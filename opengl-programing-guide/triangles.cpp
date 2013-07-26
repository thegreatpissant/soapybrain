//////////////////////////////////////////////////////////////////////
//
//  triangles.cpp
//
//////////////////////////////////////////////////////////////////////
#include <string.h>
#include <iostream>
#include <unistd.h>
using namespace std;
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common/shader_utils.h"
//#include "LoadShaders.h"
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };
GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];
const GLuint  NumVertices = 6;
GLfloat delta = 0.05f;
GLfloat angle = 60;
GLint angle_loc = 0;
GLint color = 1;
GLint color_loc = 0;
glm::vec4 vl(0.2f);
GLint v_loc = 0;
GLfloat Rotation [4][4];
GLint Rotation_loc = 0;
GLint Projection_loc = 0;

GLsizei width = 640;
GLsizei height = 800;

void ExitOnGLError ( const char * );
#define BUFFER_OFFSET(offset)  ((void *)(offset))

void Physics () {
  //  Update rotation matrix
  Rotation[0][0] = cos(angle);
  Rotation[1][1] = cos(angle); 
  Rotation[0][1] = sin(angle);
  Rotation[1][0] = -sin(angle);
}
 void GlutKeyboardFunc (unsigned char key, int x, int y )
{
  switch (key) {
  case 27:
    exit (0);
  case 'r':
  case 'R':
    color = 1;
    break;
  case 'g':
  case 'G':
    color = 2;
    break;
  case 'b':
  case 'B':
    color = 3;
    break;
  case 'w':
  case 'W':
    color = -1;
    break;
  case 'c':
  case 'C':
    delta *= -1.0;
    break;
  case 'f':
  case 'F':
    glutFullScreenToggle ();
    break;
  }
  glUniform1i ( color_loc, color );
  glutPostRedisplay();
}
/*
  Init
*/
void init(void)
{
  glClearColor ( 0.5, 0.5, 0.5, 1.0 );
  glGenVertexArrays(NumVAOs, VAOs);
  glBindVertexArray(VAOs[Triangles]);
  GLfloat  vertices[NumVertices][3] = {
    {-0.90f,-0.90,3.0f },  // Triangle 1
    {0.85,-0.90, 3.0f },
    {-0.90,0.85,3.0f },
    {0.90,-0.85,3.0f },  // Triangle 2
    {0.90,0.90,3.0f },
    {-0.85,0.90,3.0f }
  };
  angle = 0.0;
  for ( int i = 0; i < 4; i++) {
    for ( int j = 0; j < 4; j++ )
	if ( i == j )
           Rotation [i][j] = 1.0f;
        else 
           Rotation [i][j] = 0.0f;
  }
  Physics ();
 /*
  GLfloat zfar = -10.0f;
  GLfloat znear = -1.0f;
  GLfloat Projection[4][4] = {
    { znear/(width/2.0f), 0.0f,                0.0f,                             0.0f },
    { 0.0f,               znear/(height/2.0f), 0.0f,                             0.0f },
    { 0.0f,               0.0f,               -1.0f*((znear+zfar)/(znear-zfar)), (2.0f*(znear*zfar))/(zfar-znear) },
    { 0.0f,               0.0f,               -1.0f,                             0.0f }
  };
  */
  GLfloat Projection[4][4] = {
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 1.0f},
    {0.0f, 0.0f, 0.0f, 0.0f}
  };

  glGenBuffers(NumBuffers, Buffers);
  glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  ShaderInfo  shaders[] = {
    { GL_VERTEX_SHADER, "./shaders/triangles.v.glsl" },
    { GL_FRAGMENT_SHADER, "./shaders/triangles.f.glsl" },
    { GL_NONE, NULL }
  };

  GLuint program = LoadShaders(shaders);

  glUseProgram(program);
  if ( (angle_loc = glGetUniformLocation ( program, "angle" )) == -1 ) {
    std::cout << "Did not find the angle loc\n";
  }    
  if ( (color_loc = glGetUniformLocation ( program, "color" )) == -1 ) {
    std::cout << "Did not find the color loc\n";
  }
  if ( (v_loc     = glGetUniformLocation ( program, "vPos2" )) == -1 ) {
    std::cout << "Did not find the vPos2 loc\n";
  }
  if ( (Rotation_loc = glGetUniformLocation (program, "mRot" )) == -1 ) {
    std:: cout << "Did not find the mRot loc\n";
  }
  if ( (Projection_loc = glGetUniformLocation (program, "mProj" )) == -1 ) {
    std:: cout << "Did not find the mProj loc\n";
  }
 
  glUniform1f ( angle_loc, angle );
  glUniform1i ( color_loc, color );
  glUniform3fv ( v_loc, 3, &vl[0] );
  glUniformMatrix4fv( Rotation_loc, 1, GL_FALSE, &Rotation[0][0] );
  glUniformMatrix4fv( Projection_loc, 1, GL_FALSE, &Projection[0][0] );
  
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);
}

/*
  Display
*/
void display(void)
{
  angle = angle + delta;
  vl[1] = 1.0f;
  Physics ();
  glUniformMatrix4fv( Rotation_loc, 1, GL_FALSE, &Rotation[0][0] );
  glUniform3fv ( v_loc , 3, &vl[0] );
  glClear(GL_COLOR_BUFFER_BIT);
  glBindVertexArray(VAOs[Triangles]);

  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
  glFinish ();
  glutPostRedisplay ();
  usleep(10000);
}

void Reshape (int newWidth, int newHeight) {
  width = newWidth / 2.0;
  height = newHeight;
}
/*
  Main
*/
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(1280,720);
  //  glutInitContextVersion(4, 3);
  //  glutInitContextProfile(GLUT_CORE_PROFILE);

 glutCreateWindow(argv[0]);
  if (glewInit()) {
    cerr << "Unable to initialize GLEW ... exiting" << endl;
    exit(EXIT_FAILURE);
  }
  init();
  glutDisplayFunc(display);
  glutReshapeFunc (Reshape);
  glutKeyboardFunc ( GlutKeyboardFunc );
  glutMainLoop();
}


void ExitOnGLError ( const char * error_message ) {
  cout << error_message << endl;
}


// Shader fragment
/*
  #version 430 core
  out vec4 fColor;
  void main () {
  fColor = vec4 ( 0.0, 0.0, 1.0, 1.0 );
  }
*/
