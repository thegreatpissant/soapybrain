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

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "common/shader_utils.h"


#include "libovr_nsb/OVR.h"

//  Models
struct model_t {
  long   numVertices;
  vector <float> vertices;
  vector <GLuint> vaos;
  vector <GLuint> buffers;
  int    renderPrimitive;
  void Render () {
    glBindVertexArray(vaos[0]);
    glBindBuffer( GL_ARRAY_BUFFER, buffers[0]);
    glDrawArrays(renderPrimitive, 0, numVertices);
  } 
};

Device *dev;

struct model_t ex15_1;
struct model_t ex15_2;

enum Attrib_IDs { vPosition = 0 };

GLint color = 1;
GLint color_loc = 0;
GLfloat hResolution = 640.0f;
GLfloat vResolution = 800.0f;
GLfloat hScreenSize = 0.14976f;
GLfloat vScreenSize = 0.0935f;
GLfloat eyeScreenDist = 0.041f;
GLfloat aspect = hResolution / (2.0f * vResolution);
GLfloat fov = 2.0f*(atan(0.0935f/(2.0f*eyeScreenDist)));
GLfloat zNear  = 0.3f;
GLfloat zFar   = 2000.0f;
glm::mat4 MVP;
GLfloat ipd  = 0.064f;
GLfloat h = (0.25f * hScreenSize) - (0.5f * 0.064f);
GLfloat hOffset = 4.0f* h / hScreenSize;
GLint MVP_loc = 0;
glm::mat4 omat;
typedef  struct shaderinfo {
  GLuint shadertype;
  const char * filename;
} ShaderInfo;

GLfloat depth = -300.0f;
GLfloat height = 0.0f;
GLfloat strafe = 0.0f;
GLsizei deviceWidth = 1280;
GLsizei deviceHeight = 800;
GLsizei screenWidth = 1280;
GLsizei screenHeight = 800;
int RotX= 0, PrevX = 0;

GLuint LoadShaders(ShaderInfo * si);
void ExitOnGLError ( const char * );
#define BUFFER_OFFSET(offset)  ((void *)(offset))

void Init ();
void UpdateView ();
void UpdateIPD ();
void DrawGrid ();
void GenerateModels ();
void IdleFunction ();
void MouseFunction (int, int, int, int);
void PassiveMotionFunction (int, int);
void GlutKeyboardFunc (unsigned char key, int x, int y )
{
  switch (key) {
  case 27:
  case 'q':
  case 'Q':
    exit (0);
  case 'w':
  case 'W':
    depth += 1.0f;
    cout << "depth= " << depth << endl;
    break;
  case 's':
  case 'S':
    depth -= 1.0f;
    cout << "depth= " << depth << endl;
    break;
  case 'a':
  case 'A':
    strafe -= 0.10f;
  break;
  case 'd':
  case 'D':
    strafe += 0.10f;
  break;
  case ' ':
    height += 0.10f;
  break;
  case 'c':
    height -= 0.10f;
  break;
  case 'e':
  case 'E':
    height= 0.0f;
    depth = -1000.0f;
    strafe = 0.0f;
    ipd  = 0.064f;
    UpdateIPD();
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
  case 'p':
    ipd += 0.001f;  // hOffset += 0.01f;
    UpdateIPD ();
	cout << "hOffset = " << hOffset << endl;
	break;
  case 'P':
    ipd -= 0.001f;  // hOffset -= 0.01f;
    UpdateIPD ();
	cout << "hOffset = " << hOffset << endl;
	break;
  }
  glUniform1i ( color_loc, color );
  UpdateView();
  glutPostRedisplay();
}
/*
 -Display
*/
void UpdateIPD () {

}

glm::mat4 viewLeft = glm::mat4(1.0f);
glm::mat4 viewRight = glm::mat4(1.0f);
void UpdateView () {
  glm::mat4  model = glm::translate( strafe, height, depth); // * omat;

  glm::mat4 htranslate = glm::translate(model, glm::vec3(hOffset, 0.0f, 0.0f));
  glm::mat4 projectionLeft  =  MVP*htranslate;

  htranslate = glm::translate(model, glm::vec3(-hOffset, 0.0f, 0.0f));

  glm::mat4 projectionRight = MVP*htranslate; 
  
  viewLeft =  glm::translate(projectionLeft, glm::vec3(0.5*ipd, 0.0f, 0.0f));
  viewRight = glm::translate(projectionRight, glm::vec3(-0.5*ipd, 0.0f, 0.0f));
}

void PostViewLeft () {
  glUniformMatrix4fv( MVP_loc, 1, GL_FALSE, &viewLeft[0][0] ); 
}

void PostViewRight () {
  glUniformMatrix4fv( MVP_loc, 1, GL_FALSE, &viewRight[0][0] ); 
}

void Display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

//  Left Side
  glViewport (0,0, screenWidth/2.0, screenHeight);  
  PostViewLeft ();
  ex15_1.Render ();
  ex15_2.Render ();	

//  Right Side
  glViewport (screenWidth/2.0, 0, screenWidth/2.0,screenHeight); 
  PostViewRight ();
  ex15_1.Render ();
  ex15_2.Render ();	

  glBindVertexArray (0);
  glFinish ();
  glutSwapBuffers ();
}

void Reshape (int newWidth, int newHeight) {
  screenWidth = newWidth;
  screenHeight = newHeight;
  UpdateView ();
  glutPostRedisplay ();
}

/*
  Main
*/
int main(int argc, char** argv)
{
  dev = openRift(0,0);

  if (!dev) {
	cout << "Error opening rift" << endl;
	return 0;
  }
  cout << "Device Info:\n" << endl;
  cout << "\tname:\t" << dev->name << endl;
  cout << "\tlocation:\t" << dev->location << endl;
  cout << "\tvendor:\t" << dev->vendorId << endl;
  cout << "\tproduct:\t" << dev->productId << endl;

  runSensorUpdateThread (dev);

  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(screenWidth,screenHeight);
  //  glutInitContextVersion(4, 3);
  //  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutCreateWindow(argv[0]);
  if (glewInit()) {
    cerr << "Unable to initialize GLEW ... exiting" << endl;
    exit(EXIT_FAILURE);
  }
  Init();
  glutIdleFunc     (IdleFunction);
  glutMouseFunc    (MouseFunction);
  glutDisplayFunc  (Display);
  glutReshapeFunc  (Reshape);
  glutKeyboardFunc (GlutKeyboardFunc);
  glutMainLoop();
}

void Init(void)
{
  //  Models
  GenerateModels ();

  //  Shaders
  ShaderInfo  shaders[] = {
    { GL_VERTEX_SHADER, "./shaders/oculus.v.glsl" },
    { GL_FRAGMENT_SHADER, "./shaders/oculus.f.glsl" },
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

  MVP = glm::perspective(fov, aspect, zNear, zFar);
  ipd  = 0.064f;
  UpdateIPD();

  //  View
  glClearColor ( 0.0, 0.0, 0.0, 1.0 );

  UpdateView ();
  glutPostRedisplay ();

}

void GenerateModels () {
  float x = -3.0f, z = -10.0f;;
  ex15_1.numVertices = 600;
  ex15_1.vertices.resize(ex15_1.numVertices*3);
  for (int i = 0; i < ex15_1.numVertices; i++, x+= 0.01f, z += 0.05f) {
    ex15_1.vertices[i*3] = x;
    ex15_1.vertices[i*3 + 1] = powf(x,1);
    ex15_1.vertices[i*3 + 2] = z; // z
    if ( z >= -1.0f)
      z = -10.0f; //(i % 2 ? -8.0f:-12.0f);
  }

  x = -3.0f;
  z = -10.0f;
  ex15_2.numVertices = 600;
  ex15_2.vertices.resize(ex15_2.numVertices*3);
  for (int i = 0; i < ex15_2.numVertices; i++, x+= 0.01f, z+= 0.05f) {
    ex15_2.vertices[i*3] = x;
    ex15_2.vertices[i*3 + 1] = powf(x,3);
    ex15_2.vertices[i*3 + 2] = z; // z
    if ( z >= -1.0f) 
       z = -10.0f;
  }

  ex15_1.vaos.resize(1);
  glGenVertexArrays( ex15_1.vaos.size(), &ex15_1.vaos[0] );
  if ( ex15_1.vaos[0] == 0 ) {
    cerr << "ex15_1: Did not get a valid Vertex Attribute Object" << endl;
  } 
  glBindVertexArray( ex15_1.vaos[0] );
  ex15_1.buffers.resize(1);
  glGenBuffers(ex15_1.buffers.size(), &ex15_1.buffers[0]);
  glBindBuffer(GL_ARRAY_BUFFER, ex15_1.buffers[0]);

  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*ex15_1.vertices.size(), &ex15_1.vertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);
  glBindVertexArray (0);
  ex15_1.renderPrimitive = GL_POINTS;

  ex15_2.vaos.resize(1);
  glGenVertexArrays( ex15_2.vaos.size(), &ex15_2.vaos[0] );
  if ( ex15_2.vaos[0] == 0 ) {
    cerr << "ex15_2: Did not get a valid Vertex Attribute Object" << endl;
  } 
  glBindVertexArray( ex15_2.vaos[0] );
  ex15_2.buffers.resize(1);
  glGenBuffers(ex15_2.buffers.size(), &ex15_2.buffers[0]);
  glBindBuffer(GL_ARRAY_BUFFER, ex15_2.buffers[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*ex15_2.vertices.size(), &ex15_2.vertices[0], GL_STATIC_DRAW);
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET(0));
  glEnableVertexAttribArray(vPosition);
  glBindVertexArray (0);
  ex15_2.renderPrimitive = GL_POINTS;
}

void DrawGrid () {
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

void IdleFunction () {
  //  glutPostRedisplay ();
  static glm::detail::tquat<float> oquat;
  oquat = glm::detail::tquat<float> (dev->Q[3], dev->Q[0], dev->Q[1], dev->Q[2]);
  //  cout << "quat: " << dev->Q[0] << ", " << dev->Q[1] << ", " << dev->Q[2] << ", " << dev->Q[3] << endl;
  omat = glm::mat4_cast(oquat); 
  UpdateView();
  glutPostRedisplay ();

}
void MouseFunction (int x, int y, int j, int k) {
  
  //  glutPostRedisplay ();
}

void PassiveMotionFunction (int x, int y) {
  RotX = PrevX - x;
  PrevX = x;
}
