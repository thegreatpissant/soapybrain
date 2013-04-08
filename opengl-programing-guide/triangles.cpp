//////////////////////////////////////////////////////////////////////
//
//  triangles.cpp
//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <unistd.h>
using namespace std;
#include <GL/glew.h>
#include <GL/glut.h>
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
typedef  struct shaderinfo {
  GLuint shadertype;
  char * filename;
} ShaderInfo;

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
  angle_loc = glGetUniformLocation ( program, "angle" );
  color_loc = glGetUniformLocation ( program, "color" );
  if ( angle_loc == -1 || color_loc == -1 ) {
    std::cout << "Did not find the angle loc\n";
  }
  glUniform1f ( angle_loc, angle );
  glUniform1i ( color_loc, color );
  
  glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);
}

/*
  Display
*/
void display(void)
{
  angle = angle + delta;
  
  glUniform1f ( angle_loc, angle );
  glClear(GL_COLOR_BUFFER_BIT);
  glBindVertexArray(VAOs[Triangles]);
  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
  //  glFlush();
  glFinish ();
  glutPostRedisplay ();
  usleep(10000);
}

/*
  Main
*/
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(512, 512);
  //  glutInitContextVersion(4, 3);
  //  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutCreateWindow(argv[0]);
  if (glewInit()) {
    cerr << "Unable to initialize GLEW ... exiting" << endl;
    exit(EXIT_FAILURE);
  }
  init();
  glutDisplayFunc(display);
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
