//////////////////////////////////////////////////////////////////////
//
//  oculus.cpp
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

//  Oculus subsystem
#include <OVR.h>
using namespace OVR;
Ptr<DeviceManager> pManager;
Ptr<HMDDevice>     pHMD;
Ptr<SensorDevice>  pSensor;
SensorFusion SFusion;
void Initialize_hmd ();

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
GLfloat eyeProjectionShift = 0.0f;
GLfloat projectionCenterOffset = 0.0f;
glm::mat4 projectionCenter;
glm::mat4 projectionLeft ;
glm::mat4 projectionRight ;

GLint MVP_loc = 0;
glm::mat4 omat;
typedef  struct shaderinfo {
  GLuint shadertype;
  const char * filename;
} ShaderInfo;
glm::mat4 viewLeft = glm::mat4(1.0f);
glm::mat4 viewRight = glm::mat4(1.0f);

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
	cout << "eyeProjectionShift = " << eyeProjectionShift << endl;
	break;
  case 'P':
    ipd -= 0.001f;  // hOffset -= 0.01f;
	cout << "eyeProjectionShift = " << eyeProjectionShift << endl;
	break;
  }
  glUniform1i ( color_loc, color );
  UpdateView();
  glutPostRedisplay();
}

/*
  Display
*/
void UpdateView () {
  //  Vector3f accel = SFusion.GetAcceleration (); 
  //  cout << "Acceleration: x:" << accel.x << ", y:" << accel.y << ", z:" << accel.z << endl;
  Quatf orient = SFusion.GetOrientation ();
  //  cout << "Orientation x:" << orient.x << ", y:" << orient.y << ", z:" << orient.z << endl;
}

void PostViewLeft () {
  glUniformMatrix4fv( MVP_loc, 1, GL_FALSE, &projectionLeft[0][0] ); 
}

void PostViewRight () {
  glUniformMatrix4fv( MVP_loc, 1, GL_FALSE, &projectionRight[0][0] ); 
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
  System::Init( Log::ConfigureDefaultLog (LogMask_All));
  Initialize_hmd ();
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

  System::Destroy ();
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
void Initialize_hmd ( ) {

  pManager = *DeviceManager::Create ();
  pHMD     = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();
  pSensor  = *pHMD->GetSensor ();
  if (pSensor)
    SFusion.AttachToSensor (pSensor);

  HMDInfo hmd;
  if (pHMD->GetDeviceInfo (&hmd)) {
    cout << "Monitor Name = " << hmd.DisplayDeviceName << endl;
    cout << "Eye Distace = " << hmd.InterpupillaryDistance << endl;
    cout << "DistortionK[0] = " << hmd.DistortionK[0] << endl;
    cout << "DistortionK[1] = " << hmd.DistortionK[1] << endl;
    cout << "DistortionK[2] = " << hmd.DistortionK[2] << endl;
    cout << "DistortionK[3] = " << hmd.DistortionK[3] << endl;
    cout << endl;
  }

  hResolution = (GLfloat) hmd.HResolution;
  vResolution = (GLfloat) hmd.VResolution;
  hScreenSize = (GLfloat) hmd.HScreenSize;
  vScreenSize = (GLfloat) hmd.VScreenSize;
  eyeScreenDist = (GLfloat) hmd.EyeToScreenDistance;

  aspect = (0.5f * hResolution) / vResolution;
  fov = 2.0f*(atan(0.0935f/(2.0f*eyeScreenDist)));

  ipd = hmd.InterpupillaryDistance;
  GLfloat viewCenter = hScreenSize * 0.25f;
  eyeProjectionShift = viewCenter - (hmd.LensSeparationDistance * 0.5f);
  projectionCenterOffset = 4.0f * eyeProjectionShift / hScreenSize;

  projectionCenter = glm::perspective (fov, aspect, 0.3f, 1000.0f);
  projectionLeft =  glm::translate(projectionCenter, glm::vec3(projectionCenterOffset, 0, 0));
  projectionRight = glm::translate(projectionCenter, glm::vec3(-projectionCenterOffset, 0, 0));

  viewLeft = glm::translate(glm::mat4 (1.0f), glm::vec3(ipd * 0.5f, 0, 0)) * viewCenter;
  viewRight = glm::translate(glm::mat4 (1.0f), glm::vec3(ipd * -0.5f, 0, 0)) * viewCenter; 

  zNear  = 0.3f;
  zFar   = 2000.0f;
  depth  = -300.0f;
  height = 0.0f;
  strafe = 0.0f;
  
  RotX = 0;
  PrevX = 0;

}

