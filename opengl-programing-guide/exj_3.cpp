/*  exj_3.cpp
 * James A. Feister - thegreatpissant@gmail.com
 * DONE - Break out different model types.
 * DONE - Add a simple render system, yes it is very simple
 * Add an actor a subclass of an entity - Moved to exj_4.cpp
 */

#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <unistd.h>
using namespace std;

//  OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

// 3rd Party
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//  Engine parts
#include "common/shader_utils.h"
#include "Render.h"
#include "Model.h"
#include "Display.h"
#include "Actor.h"
#include "Camera.h"

enum class queue_events {
  STRAFE_LEFT, STRAFE_RIGHT, MOVE_FORWARD, MOVE_BACKWARD, ROTATE_LEFT, ROTATE_RIGHT, MOVE_UP, MOVE_DOWN,
    COLOR_CHANGE,
    APPLICATION_QUIT
    };
queue <queue_events> gqueue;
render Renderer;
render Renderer1;
shared_ptr<camera> Camera;
shared_ptr<entity> Selected;

//  Constants and Vars  
//  @@TODO Should move into a variable system
Display screen0;
int screenWidth = screen0.screenWidth;
int screenHeight = screen0.screenHeight;
glm::mat4 Projection;
glm::mat4 MVP = glm::perspective(45.0f, 4.0f / 3.0f, 1.0f, 100.f);
glm::mat4 camera_matrix = glm::mat4(0.0);
GLint MVP_loc = 0;
GLint camera_loc = 0;
GLint MVP_loc2 = 0;
GLint camera_loc2 = 0;

//  Shader programs
GLuint program, program2;

//  Function Declarations
void Init ();
void UpdateView ();
void PostView ();
void GlutIdle ();
void GlutReshape ( int newWidth, int newHeight );
void GlutDisplay ( void );
void GlutKeyboard ( unsigned char key, int x, int y );
void UpdatePerspective ();
void CleanupAndExit ();
//  Models
void GenerateModels ();
//  Entities
void GenerateEntities ();
//  View items
void InitializeView ();
//  Shaders
void GenerateShaders ();

//  Globalized user vars
GLfloat strafe = 1.0f, height = 0.0f, depth = -3.0f, rotate = 0.0f;
GLint color = 1;
GLint color_loc = 0;
GLint color_loc2 = 0;


// MAIN //
int main ( int argc, char ** argv) {
  glutInit ( &argc, argv );
  glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA );
  glutInitWindowSize ( screenWidth, screenHeight );

  glutCreateWindow (argv[0]);
  if ( glewInit() ) {
    cerr << "Unable to initialize GLEW ... exiting " << endl;
    exit ( EXIT_FAILURE );
  }
  
  //  Load our Application Items
  GenerateModels ();
  GenerateEntities ();
  GenerateShaders ();

  //  Boiler Plate
  glutIdleFunc ( GlutIdle );
  glutReshapeFunc ( GlutReshape );
  glutDisplayFunc ( GlutDisplay );
  glutKeyboardFunc ( GlutKeyboard );
  
  //  Go forth and loop
  glutMainLoop ();
}

void GenerateShaders () {
  //  Shaders
  ShaderInfo shaders [] = {
    { GL_VERTEX_SHADER,   "./shaders/ex15_1.v.glsl" },
    { GL_FRAGMENT_SHADER, "./shaders/ex15_1.f.glsl" },
    { GL_NONE, NULL }
  };
  ShaderInfo shaders2 [] = {
    { GL_VERTEX_SHADER,   "./shaders/exj_3_1.v.glsl"},
    { GL_FRAGMENT_SHADER, "./shaders/exj_3_1.f.glsl"},
    { GL_NONE, NULL }
  };

  program = LoadShaders(shaders);
  glUseProgram (program);
  if ( (color_loc = glGetUniformLocation ( program, "color" )) == -1 ) {
    cerr << "Did not find the color loc\n";
  }
  if ( (MVP_loc = glGetUniformLocation (program, "mMVP" )) == -1 ) {
    cerr << "Did not find the mMVP loc\n";
  }
  if ( (camera_loc = glGetUniformLocation (program, "mCamera" )) == -1 ) {
    cerr << "Did not find the mCamera loc\n";
  }
  glUniform1i ( color_loc, color );
  glUseProgram (0);

  program2 = LoadShaders(shaders2);
  glUseProgram (program2);
  if ( (color_loc2 = glGetUniformLocation ( program2, "color" )) == -1 ) {
    cerr << "2: Did not find the color loc\n";
  }
  if ( (MVP_loc2 = glGetUniformLocation (program2, "mMVP" )) == -1 ) {
    cerr << "2: Did not find the mMVP loc\n";
  }
  if ( (camera_loc2 = glGetUniformLocation (program2, "mCamera" )) == -1 ) {
    cerr << "2: Did not find the mCamera loc\n";
  }
  glUniform1i ( color_loc2, color );
  glUseProgram (0);
}

void InitializeView () {
  //  View
  glClearColor ( 0.0, 0.0, 0.0, 1.0 );

  UpdateView ();
  PostView ();
}

void GlutReshape ( int newWidth, int newHeight ) {
  screenWidth = newWidth;
  screenHeight = newHeight;
  UpdatePerspective ();
  glutPostRedisplay ();
}

void GlutDisplay ( void ) {
  glClear (GL_COLOR_BUFFER_BIT);
  glViewport ( 0, 0, screenWidth, screenHeight );
  //
  //  Eventually Call on rederer to render Entities
  //
  glUseProgram(program);
  Renderer1.Render ();
  glBindVertexArray (0);

  glUseProgram(program2);
  Renderer.Render ();
  glBindVertexArray (0);  

  glUseProgram (0);
  glFinish ();
  glutSwapBuffers ();

}

void GlutKeyboard ( unsigned char key, int x, int y ) {
  switch ( key ) {
  default:
    break;
  case 27:
  case 'q':
  case 'Q':
    gqueue.push ( queue_events::APPLICATION_QUIT );
    break;
  case 'h':
  case 'H':
    gqueue.push ( queue_events::ROTATE_LEFT );
    break;
  case 'l':
  case 'L':
    gqueue.push ( queue_events::ROTATE_RIGHT );
    break;
  case 'a':
  case 'A':
    gqueue.push ( queue_events::STRAFE_LEFT );
    break;
  case 'd':
  case 'D':
    gqueue.push ( queue_events::STRAFE_RIGHT );
    break;
  case 's':
  case 'S':
    gqueue.push ( queue_events::MOVE_BACKWARD );
    break;
  case 'w':
  case 'W':
    gqueue.push ( queue_events::MOVE_FORWARD );
    break;
  case 'k':
  case 'K':
    gqueue.push ( queue_events::MOVE_UP );
    break;
  case 'j':
  case 'J':
    gqueue.push ( queue_events::MOVE_DOWN );
    break;
  case 'c':
  case 'C':
    gqueue.push ( queue_events::COLOR_CHANGE );
    break;
  }
}

void GlutIdle () {
  //  Pump the events loop
  while ( !gqueue.empty () ) {
    switch ( gqueue.front() ) {
    case queue_events::MOVE_FORWARD :
      Selected->position_z += 1.0f;
      break;
    case queue_events::MOVE_BACKWARD :
      Selected->position_z -= 1.0f;
      break;
    case queue_events::STRAFE_RIGHT :
      Selected->position_x += 1.0f;
      break;
    case queue_events::STRAFE_LEFT :
      Selected->position_x -= 1.0f;
      break;
    case queue_events::ROTATE_RIGHT :
      Selected->orientation_x += 0.5f;
      break;
    case queue_events::ROTATE_LEFT :
      Selected->orientation_x -= 0.5f;
      break;
    case queue_events::MOVE_UP :
      Selected->position_y += 0.5f;
      break;
    case queue_events::MOVE_DOWN :
      Selected->position_y -= 0.5f;
      break;
    case queue_events::COLOR_CHANGE :
      color = (color >= 4 ? 1 : color + 1 );
      break;
    case queue_events::APPLICATION_QUIT :
      CleanupAndExit ();
    }
    gqueue.pop ();
  }
  UpdateView ();
  PostView ();
  glutPostRedisplay ();
  //usleep ( 1000000 );
}

void CleanupAndExit () {
  exit ( EXIT_SUCCESS );
}

void GenerateModels () {
  shared_ptr <simple_equation_model_t> tmp = shared_ptr <simple_equation_model_t> {new simple_equation_model_t};
  int ext {0};

  tmp->numVertices = 600;
  tmp->vertices.resize(tmp->numVertices*3);
  float x = 0.0f, z = 0.0f;
  for (int i = 0; i < tmp->numVertices; i++, x+= 0.01f, z += 0.05f) {
    tmp->vertices[i*3] = x;
    tmp->vertices[i*3 + 1]= powf(x,1);
    tmp->vertices[i*3 + 2] = z;
    if ( z >= -1.0f)
      z = -10.0f; //(i % 2 ? -8.0f:-12.0f);
  }
  tmp->name = "ex15_" + to_string(ext++);
  cout << "name: " << tmp->name << endl;
  tmp->renderPrimitive = GL_POINTS;
  tmp->SetupRenderModel ();
  Renderer1.models.push_back(tmp);

  for (auto power_to : {1.0f, 1.2f, 1.4f, 1.6f, 1.8f, 2.1f, 2.2f, 2.3f, 3.5f, 4.0f} ) {
    tmp = shared_ptr <simple_equation_model_t> { new simple_equation_model_t };
    x = -3.0f;
    z = 0.0f;
    tmp->numVertices = 600;
    tmp->vertices.resize(tmp->numVertices*3);
    for (int i = 0; i < tmp->numVertices; i++, x+= 0.01f, z+= 0.05f) {
      tmp->vertices[i*3] = x;
      tmp->vertices[i*3 + 1]= powf(x,power_to);
      tmp->vertices[i*3 + 2] = 0.0f; //z;
      if ( z >= -1.0f) 
	z = 0.0f;
    }
    tmp->name = "ex15_" + to_string(ext++);
    cout << "name: " << tmp->name << endl;
    tmp->renderPrimitive = GL_POINTS;
    tmp->SetupRenderModel ();
    Renderer.models.push_back(tmp);
  }
}

void GenerateEntities () {

  //  Camera
  Camera = shared_ptr <camera> { new camera (strafe, height, depth, 0.0f, 0.0f, 0.0f) };

  //  Actors
  
  //  Selected Entity
  Selected = Camera;
}

void UpdateView () {
  camera_matrix = glm::translate (glm::mat4(), glm::vec3 ( Camera->position_x, Camera->position_y, Camera->position_z ) );
  camera_matrix = glm::rotate (camera_matrix, Camera->orientation_x, glm::vec3 (0.0f, 1.0f, 0.0f) );
}

void PostView() {
  glUseProgram (program);
  glUniformMatrix4fv( MVP_loc, 1, GL_FALSE, &MVP[0][0] ); 
  glUniformMatrix4fv( camera_loc, 1, GL_FALSE, &camera_matrix[0][0] ); 
  glUniform1i ( color_loc, color );

  glUseProgram (program2);
  glUniformMatrix4fv( MVP_loc2, 1, GL_FALSE, &MVP[0][0] ); 
  glUniformMatrix4fv( camera_loc2, 1, GL_FALSE, &camera_matrix[0][0] ); 
  glUniform1i ( color_loc2, color );

  glUseProgram (0);
}

void UpdatePerspective () {
  //  @@TODO Adjustable FOV and Aspect ration
  GLfloat hResolution = screenWidth;  //  640.0f;
  GLfloat vResolution = screenHeight; //  800.0f;
  //  GLfloat hScreenSize = 0.14976f;
  //  GLfloat vScreenSize = 0.0935f;
  GLfloat eyeScreenDist = 0.041f;
  GLfloat aspect = hResolution / (2.0f * vResolution);
  GLfloat fov = 2.0f*(atan(0.0935f/(2.0f*eyeScreenDist)));
  GLfloat zNear  = 0.3f;
  GLfloat zFar   = 1000.0f;
  Projection = glm::perspective( fov, aspect, zNear, zFar );
}
