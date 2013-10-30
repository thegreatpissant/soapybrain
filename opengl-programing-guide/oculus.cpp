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
//  using namespace std;

#include <GL/glew.h>
#include <GL/freeglut.h>

#define GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "common/shader_utils.h"

//  Oculus subsystem
#include "OVR.h"

using namespace OVR;
using namespace OVR::Util::Render;

Ptr<DeviceManager> pManager;
Ptr<HMDDevice> pHMD;
Ptr<SensorDevice> pSensor;
SensorFusion *sFusion;
HMDInfo hmdInfo;
bool hmdInfoLoaded;
StereoConfig SConfig;
float shader_scale;
void Initialize_hmd( );

//  Models
struct model_t
{
    long numVertices;
    std::vector<float> vertices;
    std::vector<GLuint> vaos;
    std::vector<GLuint> buffers;
    int renderPrimitive;
    void Render( )
    {
        glBindVertexArray( vaos[0] );
        glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
        glDrawArrays( renderPrimitive, 0, numVertices );
    }
};

struct model_t ex15_1;
struct model_t ex15_2;

enum Attrib_IDs
{
    vPosition = 0
};

GLint color = 1;
GLint color_loc = 0;
GLfloat hResolution = 640.0f;
GLfloat vResolution = 800.0f;
GLfloat hScreenSize = 0.14976f;
GLfloat vScreenSize = 0.0935f;
GLfloat ipd = 0.064f;
GLfloat eyeScreenDist = 0.041f;
GLfloat lensSeparationDistance = 0.064f;

GLfloat aspect = hResolution / ( 2.0f * vResolution );
GLfloat fov = 2.0f * ( atan( 0.0935f / ( 2.0f * eyeScreenDist ) ) );
GLfloat zNear = 0.3f;
GLfloat zFar = 2000.0f;
glm::mat4 MVP;

GLfloat eyeProjectionShift = 0.0f;
GLfloat projectionCenterOffset = 0.0f;
glm::mat4 projectionCenter;
glm::mat4 projectionLeft;
glm::mat4 projectionRight;

GLint MVP_loc = 0;
glm::mat4 orientation;
GLint orientation_loc = 0;
glm::mat4 omat;

glm::mat4 viewLeft = glm::mat4( 1.0f );
glm::mat4 viewRight = glm::mat4( 1.0f );

GLfloat depth = -60.0f;
GLfloat height = 0.0f;
GLfloat strafe = 0.0f;
GLfloat rotate = 0.0f;
GLsizei deviceWidth = 1280;
GLsizei deviceHeight = 800;
GLsizei screenWidth = 1280;
GLsizei screenHeight = 800;
int RotX = 0, PrevX = 0;

GLuint LoadShaders( ShaderInfo *si );
void ExitOnGLError( const char * );
#define BUFFER_OFFSET( offset ) ( ( void * )( offset ) )

void InitProgram( );
void UpdateView( );
void DrawGrid( );
void GenerateModels( );
void IdleFunction( );
void MouseFunction( int, int, int, int );
void PassiveMotionFunction( int, int );
void GlutKeyboardFunc( unsigned char key, int x, int y )

{
    switch ( key )
    {
    case 27:
    case 'q':
    case 'Q':
        pSensor.Clear( );
        pHMD.Clear( );
        pManager.Clear( );
        delete sFusion;
        System::Destroy( );
        std::cout << "Rift Shutdown" << std::endl;
        std::cout << "-------------" << std::endl;
        exit( 0 );
    case 'w':
    case 'W':
        depth += 1.0f;
        std::cout << "depth= " << depth << std::endl;
        break;
    case 's':
    case 'S':
        depth -= 1.0f;
        std::cout << "depth= " << depth << std::endl;
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
        height = 0.0f;
        depth = -1.0f;
        strafe = 0.0f;
        ipd = 0.064f;
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
        glutFullScreenToggle( );
        break;
    case 'p':
        ipd += 0.001f; // hOffset += 0.01f;
        std::cout << "eyeProjectionShift = " << eyeProjectionShift << std::endl;
        break;
    case 'P':
        ipd -= 0.001f; // hOffset -= 0.01f;
        std::cout << "eyeProjectionShift = " << eyeProjectionShift << std::endl;
        break;
    case '-':
        rotate -= 0.5f;
        break;
    case '+':
    case '=':
        rotate += 0.5f;
        break;
    }
    glUniform1i( color_loc, color );
    UpdateView( );
    glutPostRedisplay( );
}

/*
  Display
*/
void UpdateView( )
{
    /* Win32_OculusRoomTiny interpretation of doing this. */
    //  Needed Vars
    float yaw, EyePitch, EyeRoll;
    //  Grab the orientation
    Quatf hmdOrient = sFusion->GetOrientation( );
    //  Up is always positive y
    glm::vec4 up_vector = glm::vec4( 0.0f, 1.0f, 0.0f, 0.0f );
    //  Forward is donw the z
    glm::vec4 forward_vector = glm::vec4( 0.0f, 0.0f, 1.0f, 0.0f );
    //  All orientation is with the display
    hmdOrient.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>( &yaw, &EyePitch, &EyeRoll );
    //  Rotate and position View Camer, using Yaw Pitch and Roll in BodyFrame Cordinates
    glm::mat4 roll_pitch_yaw = glm::rotate( glm::mat4( 1.0f ), yaw, glm::vec3( 0.0f, 1.0f, 0.0f ) );
    roll_pitch_yaw = glm::rotate( roll_pitch_yaw, EyePitch, glm::vec3( 1.0f, 0.0f, 0.0f ) );
    roll_pitch_yaw = glm::rotate( roll_pitch_yaw, EyeRoll, glm::vec3( 0.0f, 0.0f, 1.0f ) );
    up_vector = roll_pitch_yaw * up_vector;
    forward_vector = roll_pitch_yaw * forward_vector;
    //  our eyes
    glm::vec4 eye_pos = glm::vec4( strafe, height, depth, 0.0f );
    float head_base_to_eye_height = 0.15f;
    float head_base_to_eye_protrusion = 0.09f;
    glm::vec4 eye_center_in_head_frame =
        glm::vec4( 0.0f, head_base_to_eye_height, -head_base_to_eye_protrusion, 0.0f );
    glm::vec4 shifted_eye_pos = eye_pos + ( roll_pitch_yaw * eye_center_in_head_frame );
    shifted_eye_pos.y -= eye_center_in_head_frame.y;
    glm::mat4 orientation = glm::lookAt( glm::vec3( shifted_eye_pos.xyz( ) ),
                                         glm::vec3( ( shifted_eye_pos + forward_vector ).xyz( ) ),
                                         glm::vec3( up_vector.xyz( ) ) );

    // Reference:
    // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#Quaternions
    glUniformMatrix4fv( orientation_loc, 1, GL_FALSE, &orientation[0][0] );
}

void PostViewLeft( )
{
    glUniformMatrix4fv( MVP_loc, 1, GL_FALSE, &projectionLeft[0][0] );
}

void PostViewRight( )
{
    glUniformMatrix4fv( MVP_loc, 1, GL_FALSE, &projectionRight[0][0] );
}
Matrix4f lp,rp;

void Display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );

    //  Left Side
    StereoEyeParams leye = SConfig.GetEyeRenderParams( StereoEye_Left );
    glViewport( leye.VP.x, leye.VP.y, leye.VP.w, leye.VP.h );
    lp = leye.Projection;
    glUniformMatrix4fv( MVP_loc, 1, GL_FALSE, &lp.Transposed().M[0][0] );
    /*
    for ( auto i = 0; i < 3; i++)
      for ( auto j = 0; j < 3; j++)
        projectionLeft[i][j] = lp.M[j][i];
    PostViewLeft( );
    */


    ex15_1.Render( );
    ex15_2.Render( );

    //  Right Side
    StereoEyeParams reye = SConfig.GetEyeRenderParams( StereoEye_Right );
    glViewport( reye.VP.x, reye.VP.y, reye.VP.w, reye.VP.h );
    rp = reye.Projection;
    glUniformMatrix4fv( MVP_loc, 1, GL_FALSE, &rp.Transposed().M[0][0]);
    /*
    for ( auto i = 0; i < 3; i++)
      for ( auto j = 0; j < 3; j++)
        projectionRight[i][j] = rp.M[j][i];
    PostViewRight( );
    */


    ex15_1.Render( );
    ex15_2.Render( );

    glBindVertexArray( 0 );
    glFinish( );
    glutSwapBuffers( );
}

void Reshape( int newWidth, int newHeight )
{
    screenWidth = newWidth;
    screenHeight = newHeight;
    UpdateView( );
    glutPostRedisplay( );
}

/*
  Main
*/
int main( int argc, char **argv )
{
    Initialize_hmd( );
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowSize( screenWidth, screenHeight );
    //  glutInitContextVersion(4, 3);
    //  glutInitContextProfile(GLUT_CORE_PROFILE);

    glutCreateWindow( argv[0] );
    if ( glewInit( ) )
    {
        std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
        exit( EXIT_FAILURE );
    }
    else
    {
        std::cout << "Initialized" << std::endl;
    }
    InitProgram( );
    glutIdleFunc( IdleFunction );
    glutMouseFunc( MouseFunction );
    glutDisplayFunc( Display );
    glutReshapeFunc( Reshape );
    glutKeyboardFunc( GlutKeyboardFunc );
    glutMainLoop( );
}

void InitProgram( )
{
    //  Models
    GenerateModels( );

    //  Shaders
    ShaderInfo shaders[] = { { GL_VERTEX_SHADER, "./shaders/oculus.v.glsl" },
                             { GL_FRAGMENT_SHADER, "./shaders/oculus.f.glsl" }, { GL_NONE, NULL } };
    GLuint program = LoadShaders( shaders );
    glUseProgram( program );
    if ( ( color_loc = glGetUniformLocation( program, "color" ) ) == -1 )
    {
        std::cout << "Did not find the color loc\n";
    }
    if ( ( MVP_loc = glGetUniformLocation( program, "mMVP" ) ) == -1 )
    {
        std::cout << "Did not find the mMVP loc\n";
    }
    if ( ( orientation_loc = glGetUniformLocation( program, "mOrientation" ) ) == -1 )
    {
        std::cout << "Did not find the mOrientation loc\n";
    }
    glUniform1i( color_loc, color );

    MVP = glm::perspective( fov, aspect, zNear, zFar );
    ipd = 0.064f;

    //  View
    glClearColor( 0.0, 0.0, 0.0, 1.0 );

    UpdateView( );
    glutPostRedisplay( );
}

void GenerateModels( )
{
    float x = -3.0f;
    float z = 10.0f;

    const int max_vertices = 600;
ex15_1.numVertices = max_vertices;
    ex15_1.vertices.resize( ex15_1.numVertices * 3 );
    for ( int i = 0; i < ex15_1.numVertices; i++, x += 0.01f, z += 0.05f )
    {
        ex15_1.vertices[i * 3] = x;
        ex15_1.vertices[i * 3 + 1] = powf( x, 1 );
        ex15_1.vertices[i * 3 + 2] = z; // z
        /*
        if ( z >= -1.0f )
            z = -10.0f; //(i % 2 ? -8.0f:-12.0f);
        */
    }

    x = -3.0f;
    z = 10.0f;
    ex15_2.numVertices = max_vertices;
    ex15_2.vertices.resize( ex15_2.numVertices * 3 );
    for ( int i = 0; i < ex15_2.numVertices; i++, x += 0.01f, z += 0.05f )
    {
        ex15_2.vertices[i * 3] = x;
        ex15_2.vertices[i * 3 + 1] = powf( x, 3 );
        ex15_2.vertices[i * 3 + 2] = z; // z
        /*
        if ( z >= -1.0f )
            z = -10.0f;
        */
    }

    ex15_1.vaos.resize( 1 );
    glGenVertexArrays( ex15_1.vaos.size( ), &ex15_1.vaos[0] );
    if ( ex15_1.vaos[0] == 0 )
    {
        std::cerr << "ex15_1: Did not get a valid Vertex Attribute Object" << std::endl;
    }
    glBindVertexArray( ex15_1.vaos[0] );
    ex15_1.buffers.resize( 1 );
    glGenBuffers( ex15_1.buffers.size( ), &ex15_1.buffers[0] );
    glBindBuffer( GL_ARRAY_BUFFER, ex15_1.buffers[0] );

    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * ex15_1.vertices.size( ), &ex15_1.vertices[0],
                  GL_STATIC_DRAW );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET( 0 ) );
    glEnableVertexAttribArray( vPosition );
    glBindVertexArray( 0 );
    ex15_1.renderPrimitive = GL_LINES;

    ex15_2.vaos.resize( 1 );
    glGenVertexArrays( ex15_2.vaos.size( ), &ex15_2.vaos[0] );
    if ( ex15_2.vaos[0] == 0 )
    {
        std::cerr << "ex15_2: Did not get a valid Vertex Attribute Object" << std::endl;
    }
    glBindVertexArray( ex15_2.vaos[0] );
    ex15_2.buffers.resize( 1 );
    glGenBuffers( ex15_2.buffers.size( ), &ex15_2.buffers[0] );
    glBindBuffer( GL_ARRAY_BUFFER, ex15_2.buffers[0] );
    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * ex15_2.vertices.size( ), &ex15_2.vertices[0],
                  GL_STATIC_DRAW );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_TRUE, 0, BUFFER_OFFSET( 0 ) );
    glEnableVertexAttribArray( vPosition );
    glBindVertexArray( 0 );
    ex15_2.renderPrimitive = GL_LINES;
}

void DrawGrid( )
{
}

void ExitOnGLError( const char *error_message )
{
    std::cout << error_message << std::endl;
}

void IdleFunction( )
{
    UpdateView( );
    glutPostRedisplay( );
}
void MouseFunction( int x, int y, int j, int k )
{

    //  glutPostRedisplay ();
}

void PassiveMotionFunction( int x, int y )
{
    RotX = PrevX - x;
    PrevX = x;
}
void Initialize_hmd( )
{
    System::Init( );
    sFusion = new SensorFusion( );

    pManager = *DeviceManager::Create( );
    pHMD = *pManager->EnumerateDevices<HMDDevice>( ).CreateDevice( );
    if ( pHMD )
    {
        hmdInfoLoaded = pHMD->GetDeviceInfo( &hmdInfo );
        std::cout << "Monitor Name: " << hmdInfo.DisplayDeviceName << std::endl;
        std::cout << "Product Name: " << hmdInfo.ProductName << std::endl;
        std::cout << "Version: " << hmdInfo.Version << std::endl;
        std::cout << "HResolution: " << hmdInfo.HResolution << std::endl;
        std::cout << "VResolution: " << hmdInfo.VResolution << std::endl;
        std::cout << "HscreenSize: " << hmdInfo.HScreenSize << std::endl;
        std::cout << "VScreenSize: " << hmdInfo.VScreenSize << std::endl;
        std::cout << "VScreenCenter: " << hmdInfo.VScreenCenter << std::endl;
        std::cout << "EyeToScreenDistance: " << hmdInfo.EyeToScreenDistance << std::endl;
        std::cout << "LensSeparationDistance: " << hmdInfo.LensSeparationDistance << std::endl;
        std::cout << "InterpupillaryDistace: " << hmdInfo.InterpupillaryDistance << std::endl;
        std::cout << "DistortionK[0]: " << hmdInfo.DistortionK[0] << std::endl;
        std::cout << "DistortionK[1]: " << hmdInfo.DistortionK[1] << std::endl;
        std::cout << "DistortionK[2]: " << hmdInfo.DistortionK[2] << std::endl;
        std::cout << std::endl;

        SConfig.SetFullViewport( Viewport( 0, 0, deviceWidth, deviceHeight ) );
        SConfig.SetStereoMode( Stereo_LeftRight_Multipass );
        SConfig.SetHMDInfo( hmdInfo );
        if ( hmdInfo.HScreenSize > 0.0f )
        {
            if ( hmdInfo.HScreenSize > 0.140f ) // 7"
                SConfig.SetDistortionFitPointVP( -1.0f, 0.0f );
            else
                SConfig.SetDistortionFitPointVP( 0.0f, 1.0f );
        }
        shader_scale = SConfig.GetDistortionScale( );
        std::cout << "Scene Distortion Scale: " << shader_scale << std::endl;
        SConfig.Set2DAreaFov( DegreeToRad( 85.0f ) );

        pSensor = *pHMD->GetSensor( );
    }
    else
    {
        pSensor = *pManager->EnumerateDevices<SensorDevice>( ).CreateDevice( );
    }

    hResolution = (GLfloat)hmdInfo.HResolution;
    vResolution = (GLfloat)hmdInfo.VResolution;
    hScreenSize = (GLfloat)hmdInfo.HScreenSize;
    vScreenSize = (GLfloat)hmdInfo.VScreenSize;
    eyeScreenDist = (GLfloat)hmdInfo.EyeToScreenDistance;
    ipd = hmdInfo.InterpupillaryDistance;
    lensSeparationDistance = hmdInfo.LensSeparationDistance;
    std::cout << "Got a sensor and hmd info" << std::endl;

    if ( pSensor )
    {
        std::cout << "Got a sensor" << std::endl;
        sFusion->AttachToSensor( pSensor );
    }

    aspect = ( 0.5f * hResolution ) / vResolution;
    fov = 2.0f * ( atan( 0.0935f / ( 2.0f * eyeScreenDist ) ) );

    GLfloat viewCenter = hScreenSize * 0.25f;
    eyeProjectionShift = viewCenter - ( lensSeparationDistance * 0.5f );
    projectionCenterOffset = 4.0f * eyeProjectionShift / hScreenSize;

    projectionCenter = glm::perspective( fov, aspect, 0.3f, 1000.0f );
    projectionLeft = glm::translate( projectionCenter, glm::vec3( projectionCenterOffset, 0, 0 ) );
    projectionRight =
        glm::translate( projectionCenter, glm::vec3( -projectionCenterOffset, 0, 0 ) );

    viewLeft = glm::translate( glm::mat4( 1.0f ), glm::vec3( ipd * 0.5f, 0, 0 ) ) * viewCenter;
    viewRight = glm::translate( glm::mat4( 1.0f ), glm::vec3( ipd * -0.5f, 0, 0 ) ) * viewCenter;

    zNear = 0.3f;
    zFar = 2000.0f;
    depth = -60.0f;
    height = 0.0f;
    strafe = 0.0f;

    RotX = 0;
    PrevX = 0;
}
