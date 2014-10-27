//////////////////////////////////////////////////////////////////////
//
// ex_oculus.cpp
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <cstdlib>

using namespace std;

//  OpenGL
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

// 3rd Party
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>

#include "OVR_CAPI.h"

//  Engine parts
#include "common/Shader.h"
#include "common/Render.h"
#include "common/Model.h"
#include "common/Display.h"
#include "common/Actor.h"
#include "common/Camera.h"
#include "common/Model_vbotorus.h"

enum class queue_events {
    STRAFE_LEFT,
    STRAFE_RIGHT,
    MOVE_FORWARD,
    MOVE_BACKWARD,
    YAW_LEFT,
    YAW_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    PITCH_UP,
    PITCH_DOWN,
    COLOR_CHANGE,
    MODEL_CHANGE,
    APPLICATION_QUIT
};

queue<queue_events> gqueue;
shared_ptr<Display> display { new Display };
shared_ptr<Renderer> renderer;
shared_ptr<Camera> camera;
shared_ptr<Entity> selected;
vector<shared_ptr<Actor>> scene_graph;

//  Constants and Vars
Shader oculus_vertex_shader(GL_VERTEX_SHADER), oculus_fragment_shader(GL_FRAGMENT_SHADER);
ShaderProgram oculus_shader;
ShaderProgram * global_shader;
glm::mat4 MVP;
glm::mat4 camera_matrix;
glm::mat3 NormalMatrix;
glm::mat4 ModelViewMatrix;

//  Function Declarations
void Init( );
void GlutIdle( );
void GlutReshape( int newWidth, int newHeight );
void GlutDisplay( );
void GlutKeyboard( unsigned char key, int x, int y );
void CleanupAndExit( );
//  Models
void GenerateModels( );
//  Entities
void GenerateEntities( );
//  Shaders
void GenerateShaders( );

//  Globalized user vars
GLfloat strafe{ 1.0f }, height{ 0.0f }, depth{ -15.0f }, rotate{ 0.0f };
int color = 0;
float dir = 1.0f;
float xpos = 2.0f;
float ypos = 0.0f;

//  Oculus Stufffffff
ovrHmdDesc *hmdDesc;
ovrHmd *hmd;
int hmd_handle = -1;
int num_ovr_devices = 0;

// MAIN //
int main( int argc, char **argv ) {

    //  OCULUS STUFF //
    ovr_Initialize( );
    int num_ovr_devices = ovrHmd_Detect( );
    if ( num_ovr_devices > 0 )
    {
        hmdDesc = new ovrHmdDesc[num_ovr_devices];
        hmd = new ovrHmd[num_ovr_devices];
    }
    else
    {
        std::cout << "No devices found." << std::endl;
        exit (EXIT_FAILURE);
    }
    for ( auto i = 0; i < num_ovr_devices; ++i )
    {
        hmd[i] = ovrHmd_Create( i );
        if ( hmd[i] )
        {
            hmd_handle = i;
            break;
        }
        else
        {
            std::cerr << " Could not enumerate device #" << i << std::endl;
        }
    }
    if (hmd_handle == -1)
    {
        std::cerr << "Could not Set up an Oculus Device" << std::endl;
        exit (EXIT_FAILURE);
    }

    if ( false == ovrHmd_StartSensor( hmd[hmd_handle], ovrSensorCap_Orientation, ovrSensorCap_Orientation ) )
    {
        std::cerr << "Required Sensor Capabilities not available.\n";
        exit (EXIT_FAILURE);
    }
    //  OCULUS STUFF END //

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowSize( display->getWidth(), display->getHeight() );

    glutCreateWindow( argv[0] );
    if ( glewInit( ) ) {
        cerr << "Unable to initialize GLEW ... exiting " << endl;
        exit( EXIT_FAILURE );
    }

    //  Initialize common systems

    //  Camera
    camera = shared_ptr<Camera>{ new Camera( strafe, height, depth, 0.0f, 0.0f,
                                             0.0f ) };
    renderer = display->getRenderer();
    display->setCamera (camera);

    //  Load our Application Items
    GenerateModels( );
    GenerateShaders( );

    //  This scene specific items
    GenerateEntities( );

    //  Boiler Plate
    glutIdleFunc( GlutIdle );
    glutReshapeFunc( GlutReshape );
    glutDisplayFunc( GlutDisplay );
    glutKeyboardFunc( GlutKeyboard );

    //  Go forth and loop
    glutMainLoop( );
}

void GenerateShaders( ) {
    //  Shaders


    try {
        oculus_vertex_shader.SourceFile("../shaders/oculus.v.glsl");
        oculus_fragment_shader.SourceFile ("../shaders/oculus.f.glsl");
        oculus_vertex_shader.Compile();
        oculus_fragment_shader.Compile();
        oculus_shader.addShader(oculus_vertex_shader);
        oculus_shader.addShader(oculus_fragment_shader);
        oculus_shader.link();
        oculus_shader.unuse();
        oculus_shader.printActiveUniforms();
    }
    catch (ShaderProgramException excp) {
        cerr << excp.what() << endl;
        exit (EXIT_FAILURE);
    }

    global_shader = &oculus_shader;
}

void GlutReshape( int newWidth, int newHeight )
{
    display->Reshape(newWidth, newHeight);
}


void GlutDisplay( )
{
    //  OCULUS STUFF
    ovrFrameTiming frameTiming = ovrHmd_BeginFrameTiming (hmd[hmd_handle], 0);
    ovrSensorState ss = ovrHmd_GetSensorState( hmd[hmd_handle], frameTiming.ScanoutMidpointSeconds );
    float yaw, eyePitch, eyeRoll;
    if ( ss.StatusFlags & ( ovrStatus_OrientationTracked ) )
    {
        ovrPosef pose = ss.Predicted.Pose;
        //pose.Orientation.GetEulerAngles< Axis_Y, Axis_X, Axis_Z> (&yaw, &eyePitch, &eyeRoll);
        yaw = pose.Orientation.y;
        eyePitch = pose.Orientation.x;
        eyeRoll = pose.Orientation.z;
//        std::cout << "yaw: " << yaw << " eyePitch: " << eyePitch << " eyeRoll: " << eyeRoll << std::endl;
    }
    //  OCULUS STUFF END

    glm::mat4 r_matrix =
//            glm::rotate( glm::mat4 (), camera->getOrientation()[0], glm::vec3( 1.0f, 0.0f, 0.0f ) );
            glm::rotate( glm::mat4 (), -eyePitch, glm::vec3( 1.0f, 0.0f, 0.0f ) );
    r_matrix =
//            glm::rotate( r_matrix, camera->getOrientation()[1], glm::vec3( 0.0f, 1.0f, 0.0f ) );
            glm::rotate( r_matrix, yaw, glm::vec3( 0.0f, 1.0f, 0.0f ) );
    r_matrix =
//            glm::rotate( r_matrix, camera->getOrientation()[2], glm::vec3( 0.0f, 0.0f, 1.0f ) );
            glm::rotate( r_matrix, -eyeRoll, glm::vec3( 0.0f, 0.0f, 1.0f ) );
    glm::vec4 cr = r_matrix * glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
    camera_matrix = glm::lookAt(
                camera->getPosition(),
                camera->getPosition() + glm::vec3( cr.x, cr.y, cr.z ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
    glm::mat4 model = glm::mat4(1.0f);
    model *= glm::rotate(-35.0f, glm::vec3(1.0f,0.0f,0.0f));
    model *= glm::rotate(35.0f, glm::vec3(0.0f,1.0f,0.0f));
    ModelViewMatrix = camera_matrix *  model;
    MVP = display->getPerspective() * ModelViewMatrix;

    //  Set values in the shader
    global_shader->use();
    global_shader->setUniform("MVP", MVP );

    global_shader->setUniform("color", color);

    display->Render( scene_graph );

    global_shader->unuse();

    glFinish( );
    glutSwapBuffers( );

    //  OCULUS STUFF
    ovrHmd_EndFrameTiming(hmd[hmd_handle]);
    //  OCULUS STUFF END
}

void GlutKeyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    default:
        break;
    case 27:
    case 'q':
    case 'Q':
        gqueue.push( queue_events::APPLICATION_QUIT );
        break;
    case 'h':
    case 'H':
        gqueue.push( queue_events::YAW_LEFT );
        break;
    case 'l':
    case 'L':
        gqueue.push( queue_events::YAW_RIGHT );
        break;
    case 'a':
    case 'A':
        gqueue.push( queue_events::STRAFE_LEFT );
        break;
    case 'd':
    case 'D':
        gqueue.push( queue_events::STRAFE_RIGHT );
        break;
    case 's':
    case 'S':
        gqueue.push( queue_events::MOVE_BACKWARD );
        break;
    case 'w':
    case 'W':
        gqueue.push( queue_events::MOVE_FORWARD );
        break;
    case 'k':
    case 'K':
        gqueue.push( queue_events::MOVE_UP );
        break;
    case '-':
        gqueue.push( queue_events::PITCH_UP );
        break;
    case '+':
        gqueue.push( queue_events::PITCH_DOWN );
        break;
    case 'j':
    case 'J':
        gqueue.push( queue_events::MOVE_DOWN );
        break;
    case 'c':
    case 'C':
        gqueue.push( queue_events::COLOR_CHANGE );
        break;
    case 'm':
    case 'M':
        gqueue.push( queue_events::MODEL_CHANGE );
        break;
    }
}

const glm::vec3 back_movement(0.0f, 0.0f, 1.0f);
const glm::vec3 forward_movement(0.0f, 0.0f, -1.0f);
const glm::vec3 left_movement( -0.3f, 0.0f, 0.0f);
const glm::vec3 right_movement( 0.3f, 0.0f, 0.0f);
const glm::vec3 up_movement(0.0f, 1.0f, 0.0f);
const glm::vec3 down_movement(0.0f, -1.0f, 0.0f);

void GlutIdle( )
{
    //  Pump the events loop
    while ( !gqueue.empty( ) ) {
        switch ( gqueue.front( ) ) {
        case queue_events::MOVE_FORWARD:
            selected->move (forward_movement);
            break;
        case queue_events::MOVE_BACKWARD:
            selected->move (back_movement);
            break;
        case queue_events::STRAFE_RIGHT:
            selected->move (right_movement);
            break;
        case queue_events::STRAFE_LEFT:
            selected->move(left_movement);
            break;
        case queue_events::YAW_RIGHT:
            selected->orient(up_movement);
            break;
        case queue_events::YAW_LEFT:
            selected->orient(down_movement);
            break;
        case queue_events::MOVE_UP:
            selected->move(up_movement);
            break;
        case queue_events::MOVE_DOWN:
            selected->move(down_movement);
            break;
        case queue_events::PITCH_UP:
            selected->orient(right_movement);
            break;
        case queue_events::PITCH_DOWN:
            selected->orient(left_movement);
            break;
        case queue_events::COLOR_CHANGE:
            color = ( color >= 4 ? 1 : color + 1 );
            break;
        case queue_events::MODEL_CHANGE:
            break;
        case queue_events::APPLICATION_QUIT:
            CleanupAndExit( );
        }
        gqueue.pop( );
    }

    glutPostRedisplay( );
}

void CleanupAndExit( )
{
  //  Oculus Stuff
  ovrHmd_Destroy( hmd[hmd_handle] );
  ovr_Shutdown( );
  //  Oculus Stuff End

  exit( EXIT_SUCCESS );
}

void GenerateModels( ) {
    int ext = 0;
    shared_ptr<Simple_equation_model_t> tmp;
    shared_ptr<VBOTorus> tmpt;

    //  Generate Torus
    tmpt = shared_ptr<VBOTorus> { new VBOTorus (0.7f, 0.3f, 50, 50) };
    tmpt->name = "vbo_torus";
    renderer->add_model( tmpt );

    //  Generate Some equation model
    for ( auto power_to :
    { 1.0f, 1.2f, 1.4f, 1.6f, 1.8f, 2.1f, 2.2f, 2.3f, 3.5f, 4.0f } ) {
        tmp =
                shared_ptr<Simple_equation_model_t>{ new Simple_equation_model_t };
        float x = 0.0f;
        float z = 0.0f;
        tmp->numVertices = 600;
        tmp->vertices.resize( tmp->numVertices * 3 );
        for ( int i = 0; i < tmp->numVertices; i++, x += 0.1f, z += 0.05f ) {
            tmp->vertices[i * 3] = x;
            tmp->vertices[i * 3 + 1] = powf( x, power_to );
            tmp->vertices[i * 3 + 2] = 0.0f; // z;
            if ( z >= -1.0f )
                z = 0.0f;
        }
        tmp->name = "ex15_" + to_string( ext++ );
        tmp->renderPrimitive = GL_POINTS;
        tmp->setup_render_model( );
        renderer->add_model( tmp );
    }

}

void GenerateEntities( ) {
   //  Actors
    GLfloat a = 0.0f;
    for ( int i = 0; i < 1; i++, a += 10.0f ) {
        scene_graph.push_back( shared_ptr<Actor>{ new Actor(
                                                  a, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, i ) } );
    }
    //  Selected Entity
    selected = camera;
}
