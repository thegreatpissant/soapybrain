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
#include "Shader.h"
#include "Render.h"
#include "Model.h"
#include "Display.h"
#include "Actor.h"
#include "Camera.h"
#include "Model_vbotorus.h"

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
    FULL_SCREEN,
    APPLICATION_QUIT
};

queue<queue_events> gqueue;
shared_ptr<Display> display { new Display };
shared_ptr<Renderer> renderer;
shared_ptr<Camera> camera;
shared_ptr<Entity> selected;
vector<shared_ptr<Actor>> scene_graph;

//  Constants and Vars
Shader default_vertex_shader(GL_VERTEX_SHADER), default_fragment_shader(GL_FRAGMENT_SHADER);
shared_ptr<ShaderProgram> default_shader;
shared_ptr<ShaderProgram> global_shader;
glm::mat4 VP;
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

//  Oculus Global Stuff
Shader oculus_vertex_shader(GL_VERTEX_SHADER), oculus_fragment_shader(GL_FRAGMENT_SHADER);
shared_ptr<ShaderProgram> oculus_shader;
ovrHmdDesc *hmdDesc;
ovrHmd *hmd;
int hmd_handle = -1;
int num_ovr_devices = 0;
// Function
void GenerateOculusRenderReqs ();
void GenerateOculusDisplayValues ();
//  Texture items
GLsizei TexWidth=640, TexHeight=480;
GLuint framebuffer, texture, oculusTexture;
GLuint renderbuffer;
ovrSizei renderTargetSize;

//  Initialized in GenerateOculusDisplayValues
ovrEyeRenderDesc eyeRenderDesc[2];
ovrSizei recommendTexSize[2];
ovrVector2i windowPosition;
ovrSizei screenResolution;
ovrDistortionMesh meshData;
ovrVector2f uvScaleOffset[2];
struct DistortionVertex {
    float Pos_x;
    float Pos_y;
    float TexR_x;
    float TexR_y;
    float TexG_x;
    float TexG_y;
    float TexB_x;
    float TexB_y;
    unsigned int ColR;
    unsigned int ColG;
    unsigned int ColB;
    unsigned int ColA;
};
DistortionVertex * v[2];
GLuint distortionBUF;
GLuint distortionVAO;
GLint locs[5];


//  Oculus Global Stuff end
// MAIN //
int main( int argc, char **argv ) {

    //  OCULUS STUFF //
    ovr_Initialize( );
    int num_ovr_devices = ovrHmd_Detect( );
    if ( num_ovr_devices > 0 )
    {
        hmd = new ovrHmd[num_ovr_devices];
        hmdDesc = new ovrHmdDesc[num_ovr_devices];

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
            ovrHmd_GetDesc(hmd[i], &hmdDesc[i]);
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

    GenerateOculusDisplayValues ();

    //  OCULUS STUFF END //

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    display->Reshape( screenResolution.w, screenResolution.h);
    glutInitWindowSize( display->getWidth(), display->getHeight() );
    glutInitWindowPosition(windowPosition.x, windowPosition.y);


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
    GenerateOculusRenderReqs( );

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
        default_vertex_shader.SourceFile("../shaders/default.vert");
        default_fragment_shader.SourceFile ("../shaders/default.frag");
        default_vertex_shader.Compile();
        default_fragment_shader.Compile();
        default_shader = shared_ptr<ShaderProgram> { new ShaderProgram };
        default_shader->addShader(default_vertex_shader);
        default_shader->addShader(default_fragment_shader);
        default_shader->link();
        default_shader->unuse();
        default_shader->printActiveUniforms();
    }
    catch (ShaderProgramException excp) {
        cerr << excp.what() << endl;
        exit (EXIT_FAILURE);
    }
    try {
        oculus_vertex_shader.SourceFile("../shaders/oculus.v.glsl");
        oculus_fragment_shader.SourceFile ("../shaders/oculus.f.glsl");
        oculus_vertex_shader.Compile();
        oculus_fragment_shader.Compile();
        oculus_shader = shared_ptr<ShaderProgram> { new ShaderProgram };
        oculus_shader->addShader(oculus_vertex_shader);
        oculus_shader->addShader(oculus_fragment_shader);
        oculus_shader->link();
        oculus_shader->unuse();
        oculus_shader->printActiveUniforms();
    }
    catch (ShaderProgramException excp) {
        cerr << excp.what() << endl;
        exit (EXIT_FAILURE);
    }

    global_shader = default_shader;
}

void GlutReshape( int newWidth, int newHeight )
{
//    display->Reshape(newWidth, newHeight);
    glutReshapeWindow(display->getWidth(), display->getHeight());
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

    //  Render to texture
    //  Attach the texture and depth buffer to the framebuffer
    glGenFramebuffers (1, &framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, oculusTexture, 0);
    glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
    glEnable(GL_DEPTH_TEST);
    //  Set render state
    static const GLfloat one = 1.0f;
    static const glm::vec3 clear_color = glm::vec3(0.0, 0.0, 0.0);

    glClearBufferfv (GL_COLOR, 0, &clear_color[0]);
    glClearBufferfv (GL_DEPTH, 0, &one);

    for (int eyeIndex = 0; eyeIndex < ovrEye_Count; eyeIndex++)
    {
        ovrEyeType  eye     = hmdDesc->EyeRenderOrder[eyeIndex];
//        ovrPosef    eyePose = ovrHmd_BeginEyeRender(*hmd, eye);
//        ovrQuatf       orientation = Quatf(eyePose.Orientation);
//        ovrMatrix4f    proj        = ovrMatrix4f_Projection(hmdDesc->DefaultEyeFov[eye], 0.01f, 10000.0f, true);

//        ovrMatrix4f
//                view = ovrMatrix4f(orientation.Inverted()) * Matrix4f::Translation(-EyePos);

        int vpw = recommendTexSize[eye].w;
        if (eye == 0 )
            glViewport(0,0, vpw, recommendTexSize[eye].h);
        if (eye == 1 )
            glViewport ( vpw, 0, vpw, recommendTexSize[eye].h);

        glm::mat4 r_matrix =
                glm::rotate( glm::mat4 (), camera->getOrientation()[0], glm::vec3( 1.0f, 0.0f, 0.0f ) );
        //            glm::rotate( glm::mat4 (), -eyePitch, glm::vec3( 1.0f, 0.0f, 0.0f ) );
        r_matrix =
                glm::rotate( r_matrix, camera->getOrientation()[1], glm::vec3( 0.0f, 1.0f, 0.0f ) );
        //            glm::rotate( r_matrix, yaw, glm::vec3( 0.0f, 1.0f, 0.0f ) );
        r_matrix =
                glm::rotate( r_matrix, camera->getOrientation()[2], glm::vec3( 0.0f, 0.0f, 1.0f ) );
        //            glm::rotate( r_matrix, -eyeRoll, glm::vec3( 0.0f, 0.0f, 1.0f ) );
        glm::vec4 cr = r_matrix * glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
        camera_matrix = glm::lookAt(
                    camera->getPosition(),
                    camera->getPosition() + glm::vec3( cr.x, cr.y, cr.z ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
        glm::mat4 model = glm::mat4(1.0f);
        VP = display->getPerspective() * camera_matrix;

        //  Set values in the shader
        global_shader->use();
        global_shader->setUniform("VP", VP );
        global_shader->setUniform("M", model);

        global_shader->setUniform("color", color);

        display->Render( scene_graph );
//        ovrHmd_EndEyeRender(*hmd,eye,eyePose, );

    }

    global_shader->unuse();
    oculus_shader->use();

    glBindTexture (GL_TEXTURE_2D, oculusTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //  Render to texture cont...
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor (1.0f, 0.0f, 1.0f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    oculus_shader->setUniform("EyeToSourceUVScale", uvScaleOffset[0].x, uvScaleOffset[0].y);
    oculus_shader->setUniform("EyeToSourceUVOffset", uvScaleOffset[1].x, uvScaleOffset[1].y);

    glViewport(0, 0, recommendTexSize[0].w, recommendTexSize[0].h);

    glBindVertexArray(distortionVAO);
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, distortionBUF);
    for (int i = 0; i < 5; i++)
    {
        glEnableVertexAttribArray(locs[i]);
    }
    glDrawElements(GL_TRIANGLES, meshData.IndexCount, GL_UNSIGNED_SHORT, NULL);
    glFinish( );
    glutSwapBuffers( );

    oculus_shader->unuse();

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
    case 'f':
    case 'F':
        gqueue.push( queue_events::FULL_SCREEN );
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
        case queue_events::FULL_SCREEN:
            glutFullScreenToggle();
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
    scene_graph.clear();
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
    for ( int i = 0; i < 10; i++, a += 5.0f ) {
        shared_ptr<Actor> actor = shared_ptr<Actor>{ new Actor(a, 0.0f, /*a*/0.0f, a, 0.0f, 0.0f, 0 ) };
        actor->setShader(global_shader);
        scene_graph.push_back(actor);
    }
    //  Selected Entity
    selected = camera;
}

void GenerateOculusDisplayValues ()
{
    //  Get our eye render information

    eyeRenderDesc[0] = ovrHmd_GetRenderDesc(*hmd, ovrEye_Left, hmdDesc->DefaultEyeFov[0]);
    eyeRenderDesc[1] = ovrHmd_GetRenderDesc(*hmd, ovrEye_Right, hmdDesc->DefaultEyeFov[1]);

    //  Generate Textures
    recommendTexSize[0] = ovrHmd_GetFovTextureSize( *hmd, ovrEye_Left, hmdDesc->DefaultEyeFov[0], 1.0f);
    recommendTexSize[1] = ovrHmd_GetFovTextureSize( *hmd, ovrEye_Right, hmdDesc->DefaultEyeFov[1], 1.0f);

    renderTargetSize.w = recommendTexSize[0].w + recommendTexSize[1].w;
    renderTargetSize.h = max (recommendTexSize[0].h, recommendTexSize[1].h);

    windowPosition = hmdDesc->WindowsPos;
    screenResolution = hmdDesc->Resolution;
}
void GenerateOculusRenderReqs ()
{
    glGenTextures (1, &oculusTexture);
    glBindTexture (GL_TEXTURE_2D, oculusTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, renderTargetSize.w, renderTargetSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    ovrRecti viewports[2];
    ovrVector2i eyevp;
    eyevp.x = 0;
    eyevp.y = 0;
    viewports[0].Pos = eyevp;
    viewports[0].Size = recommendTexSize[0];
    eyevp.x = recommendTexSize[0].w;
    viewports[1].Pos = eyevp;
    viewports[1].Size = recommendTexSize[1];

    for ( int eyeNum = 0; eyeNum < 2; eyeNum++)
    {
        // Allocate & generate distortion mesh vertices.
        ovrHmd_CreateDistortionMesh(*hmd, eyeRenderDesc[eyeNum].Eye, eyeRenderDesc[eyeNum].Fov,
                                    hmdDesc->DistortionCaps, &meshData);
        ovrHmd_GetRenderScaleAndOffset(eyeRenderDesc[eyeNum].Fov,
                                       recommendTexSize[eyeNum], viewports[eyeNum],
                                       /*(ovrVector2f*)*/ &uvScaleOffset[eyeNum]);
        // Now parse the vertex data and create a render ready vertex buffer from it
        DistortionVertex * pVBVerts = new DistortionVertex[meshData.VertexCount];
        v[eyeNum] = pVBVerts;
        ovrDistortionVertex * ov  = meshData.pVertexData;
        for ( unsigned vertNum = 0; vertNum < meshData.VertexCount; vertNum++ )
        {
            v[eyeNum]->Pos_x= ov->Pos.x;
            v[eyeNum]->Pos_y = ov->Pos.y;
            v[eyeNum]->TexR_x = (*(ovrVector2f*)&ov->TexR).x;
            v[eyeNum]->TexR_y = (*(ovrVector2f*)&ov->TexR).y;
            v[eyeNum]->TexG_x = (*(ovrVector2f*)&ov->TexG).x;
            v[eyeNum]->TexG_y = (*(ovrVector2f*)&ov->TexG).y;
            v[eyeNum]->TexB_x = (*(ovrVector2f*)&ov->TexB).x;
            v[eyeNum]->TexB_y = (*(ovrVector2f*)&ov->TexB).y;
            v[eyeNum]->ColR = v[eyeNum]->ColG = v[eyeNum]->ColB = (unsigned int)( ov->VignetteFactor * 255.99f );
            v[eyeNum]->ColA = (unsigned int)( ov->TimeWarpFactor * 255.99f );
            v[eyeNum]++; ov++;
        }

        glGenVertexArrays(1,&distortionVAO);
        glBindVertexArray(distortionVAO);

        glGenBuffers(1, &distortionBUF);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, distortionBUF);

        GLint prog = oculus_shader->getHandle();

        locs[0] = glGetAttribLocation(prog, "Position");
        locs[1] = glGetAttribLocation(prog, "Color");
        locs[2] = glGetAttribLocation(prog, "TexCoord0");
        locs[3] = glGetAttribLocation(prog, "TexCoord1");
        locs[4] = glGetAttribLocation(prog, "TexCoord2");

        glVertexAttribPointer(locs[0], 2, GL_FLOAT, false, sizeof(DistortionVertex), reinterpret_cast<char*>(0)+offsetof(DistortionVertex, Pos_x));
        glVertexAttribPointer(locs[1], 4, GL_UNSIGNED_BYTE, true, sizeof(DistortionVertex), reinterpret_cast<char*>(0)+offsetof(DistortionVertex, ColR));
        glVertexAttribPointer(locs[2], 2, GL_FLOAT, false, sizeof(DistortionVertex), reinterpret_cast<char*>(0)+offsetof(DistortionVertex, TexR_x));
        glVertexAttribPointer(locs[3], 2, GL_FLOAT, false, sizeof(DistortionVertex), reinterpret_cast<char*>(0)+offsetof(DistortionVertex, TexG_x));
        glVertexAttribPointer(locs[4], 2, GL_FLOAT, false, sizeof(DistortionVertex), reinterpret_cast<char*>(0)+offsetof(DistortionVertex, TexB_x));
        for (int i = 0; i < 5; i++)
        {
            glEnableVertexAttribArray(locs[i]);
        }
        glBindVertexArray ( 0 );

//        //Register this mesh with the renderer
//        DistortionData.MeshVBs[eyeNum] = *pRender->CreateBuffer();
//        DistortionData.MeshVBs[eyeNum]->Data ( Buffer_Vertex, pVBVerts,
//                                               sizeof(DistortionVertex) * meshData.VertexCount );
//        DistortionData.MeshIBs[eyeNum] = *pRender->CreateBuffer();
//        DistortionData.MeshIBs[eyeNum]->Data ( Buffer_Index, meshData.pIndexData,
//                                               sizeof(unsigned short) * meshData.IndexCount );
//        delete [] pVBVerts;
        ovrHmd_DestroyDistortionMesh( &meshData );
    }


    //  Create a depth buffer for our framebuffer
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, renderTargetSize.w, renderTargetSize.h);

}
