//////////////////////////////////////////////////////////////////////
//
//  ex15_1.cpp
//
//////////////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <math.h>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "common/shader_utils.h"

//  Models
typedef vector < vector<float> > pointVector_t ;

struct model_t {
  long   numVertices;
  vector <float> vertices;
  vector <GLuint> VAOs;
  vector<GLuint> Buffers;
};

struct model_t ex15_1;

enum Attrib_IDs { vPosition = 0 };

GLint color = 1;
GLuint color_loc = 0;
glm::mat4 MVP = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
GLuint MVP_loc = 0;

typedef  struct shaderinfo {
  GLuint shadertype;
  const char * filename;
} ShaderInfo;

GLfloat zOffset = -1.0f;
GLsizei width = 640;
GLsizei height = 800;

GLuint LoadShaders(ShaderInfo * si);
void ExitOnGLError ( const char * );
#define BUFFER_OFFSET(offset)  ((void *)(offset))

void UpdateView ();
void PostView ();
void DrawGrid ();
void GenerateModels ();
void GlutKeyboardFunc (unsigned char key, int x, int y )
{
  switch (key) {
  case 27:
    exit (0);
  case 'w':
  case 'W':
    zOffset += 0.10f;
    cout << "zOffset= " << zOffset << endl;
    UpdateView ();
    PostView ();
    break;
  case 's':
  case 'S':
    zOffset -= 0.10f;
    cout << "zOffset= " << zOffset << endl;
    UpdateView ();
    PostView ();
    break;
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
  case 'n':
  case 'N':
    color = -1;
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
void UpdateView () {
  /*
    GLfloat zfar = -10.0f;
    GLfloat znear = -1.0f;
    GLfloat Projection[4][4] = {
    { znear/(width/2.0f), 0.0f,                0.0f,                             0.0f },
    { 0.0f,               znear/(height/2.0f), 0.0f,                             0.0f },
    { 0.0f,               0.0f,               -1.0f*((znear+zfar)/(znear-zfar)), (2.0f*(znear*zfar))/(zfar-znear) },
    { 0.0f,               0.0f,               -1.0f,                             0.0f }
    };
    GLfloat Projection[4][4] = {
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f}
    };
  */
  //  Projection = Projection * Translate;
  glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
  glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, zOffset));
  //  glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, 0.0f /*Rotate.y*/, glm::vec3(-1.0f, 0.0f, 0.0f));
  //  glm::mat4 View = glm::rotate(ViewRotate, 0.0f /*Rotate.x*/, glm::vec3(0.0f, 1.0f, 0.0f));  
  glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
  MVP = Projection * ViewTranslate * Model;
}
void PostView () {
  glUniformMatrix4fv( MVP_loc, 1, GL_FALSE, &MVP[0][0] ); 
}
void init(void)
{
  //  Models
  GenerateModels ();

  //  Shaders
  ShaderInfo  shaders[] = {
    { GL_VERTEX_SHADER, "./shaders/ex15_1.v.glsl" },
    { GL_FRAGMENT_SHADER, "./shaders/ex15_1.f.glsl" },
    { GL_NONE, NULL }
  };
  GLuint program = LoadShaders(shaders);
  glUseProgram(program);
  if ( (color_loc = glGetUniformLocation ( program, "color" )) == -1 ) {
    std::cout << "Did not find the color loc\n";
  }
  if ( (MVP_loc = glGetUniformLocation (program, "mMVP" )) == -1 ) {
    std:: cout << "Did not find the mMVP loc\n";
  }
  glUniform1i ( color_loc, color );
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);

  //  View
  glClearColor ( 0.0, 0.0, 0.0, 1.0 );
  UpdateView ();
  PostView ();
}

/*
  Display
*/
void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

//  Left Side
  glViewport (0,0, width,height);  
  glBindVertexArray(ex15_1.VAOs[0]);
  glDrawArrays(GL_POINTS, 0, ex15_1.numVertices);

//  Right Side
  glViewport (width,0, width,height); 
  glBindVertexArray(ex15_1.VAOs[0]);
  glDrawArrays(GL_POINTS, 0, ex15_1.numVertices);

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

void GenerateModels () {

  float x = -3.0f;
  ex15_1.numVertices = 600;
  ex15_1.vertices.resize(ex15_1.numVertices*3);
  for (int i = 0; i < ex15_1.numVertices; i++, x+= 0.01f) {
    ex15_1.vertices[i*3] = x;
    ex15_1.vertices[i*3 + 1]= powf(x,2);
    ex15_1.vertices[i*3 + 2] = 1.0f;
  }

  ex15_1.VAOs.resize(1);
  glGenVertexArrays( ex15_1.VAOs.size(), &ex15_1.VAOs[0] );
  glBindVertexArray( ex15_1.VAOs[0] );

  ex15_1.Buffers.resize(1);
  glGenBuffers(ex15_1.Buffers.size(), &ex15_1.Buffers[0]);
  glBindBuffer(GL_ARRAY_BUFFER, ex15_1.Buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*ex15_1.vertices.size(), &ex15_1.vertices[0], GL_STATIC_DRAW);

}
void DrawGrid () {
  
}
