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
GLuint color_loc = 0;
glm::vec4 vl(0.2f);
GLuint v_loc = 0;
GLfloat Rotation[4][4];
GLuint Rotation_loc = 0;
typedef  struct shaderinfo {
  GLuint shadertype;
  char * filename;
} ShaderInfo;

GLsizei width = 640;
GLsizei height = 800;

GLuint LoadShaders(ShaderInfo * si);
void ExitOnGLError ( const char * );
#define BUFFER_OFFSET(offset)  ((void *)(offset))

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
    delta *= -1;
    break;
  case 'f':
  case 'F':
    glutFullScreenToggle ();
    break;
  }
  glUniform1i ( color_loc, color );
}
/*
  Init
*/
void init(void)
{
  glClearColor ( 0.5, 0.5, 0.5, 1.0 );
  glGenVertexArrays(NumVAOs, VAOs);
  glBindVertexArray(VAOs[Triangles]);
  GLfloat  vertices[NumVertices][2] = {
    {-0.90f,-0.90 },  // Triangle 1
    {0.85,-0.90 },
    {-0.90,0.85 },
    {0.90,-0.85 },  // Triangle 2
    {0.90,0.90 },
    {-0.85,0.90 }
  };
  memset ( Rotation, 0, sizeof (Rotation) );
  Rotation[0][0] = 1.0f;
  Rotation[1][1] = 1.0f;
  Rotation[2][2] = 1.0f;
  Rotation[3][3] = 1.0f;
  
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
    std:: cout << "Did not find the rotation matrix\n";
  }
 
  glUniform1f ( angle_loc, angle );
  glUniform1i ( color_loc, color );
  glUniform3fv ( v_loc, 3, &vl[0] );
  glUniformMatrix4fv( Rotation_loc, sizeof(Rotation), GL_TRUE, &Rotation[0][0] );
  
  glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);
}

/*
  Display
*/
void display(void)
{
  angle = angle + delta;
  vl[1] = 1.0f;

  glUniform1f ( angle_loc, angle );
  glUniform3fv ( v_loc , 3, &vl[0] );
  glClear(GL_COLOR_BUFFER_BIT);
  glBindVertexArray(VAOs[Triangles]);

  /*  
      glViewport (0,0, width,height);
      glDrawArrays(GL_TRIANGLES, 0, NumVertices);
      glViewport (width,0, width,height); 
  */
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

GLuint LoadShaders(ShaderInfo * si) {

  GLuint vertshader = create_shader ( si->filename, si->shadertype );
  si++;
  GLuint fragshader = create_shader ( si->filename, si->shadertype );
  GLuint program = glCreateProgram ();
  glAttachShader (program, vertshader);
  glAttachShader (program, fragshader);
  glLinkProgram (program);
  GLint link_ok;
  glGetProgramiv (program, GL_LINK_STATUS , &link_ok);
  if (!link_ok) {
    cerr << "glLinkProgram: ";
    print_log (program);
    return 0;
  }
  return program;
}

// Shader fragment
/*
  #version 430 core
  out vec4 fColor;
  void main () {
  fColor = vec4 ( 0.0, 0.0, 1.0, 1.0 );
  }
*/
