
/*  exj_5.cpp
 * James A. Feister - thegreatpissant@gmail.com
 * DONE - Break out different model types.
 * DONE - Add a simple render system, yes it is very simple
 * DONE - Add an actor a subclass of an entity
 * DONE - Use std library to load shaders
 * PROOF - Rendering function in renderer only
 * PROOF - Independent model movement
 * PROOF - very simple scene graph of entities to render
 * TODO - Move the shaders out of here
 * TODO - Move any other OpenGL stuff out of here.
 * Proposed exj_5 - Physics engine
 * Proposed exj_5 - Selection
 * Proposed exj_5 - Display class, Oculus and traditional
 * Proposed exj_5 - Fix input system to be more fluent
 */

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cstdlib>
#include <memory>
using namespace std;

//  OpenGL helpers
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform2.hpp>

//  Engine
#include "Shader.hpp"
#include "Render.hpp"
#include "Model.hpp"
#include "RenderTarget.hpp"
#include "Actor.hpp"
#include "Camera.hpp"
#include "Model_vbotorus.hpp"

enum class queue_events {
    PRINT_ALL,
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
    RENDER_PRIMITIVE_CHANGE,
    CAMERA_MOVEMENT,
    RESET,
    APPLICATION_QUIT
};
queue<queue_events> gqueue;

shared_ptr<RenderTarget> display { new RenderTarget };
shared_ptr<Renderer> renderer;

shared_ptr<Entity> selected;
vector<shared_ptr<Actor>> scene_graph;

//  Constants and Vars
//  @@TODO Should move into a variable system
Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);
Shader ads_vertex_shader(GL_VERTEX_SHADER), ads_fragment_shader(GL_FRAGMENT_SHADER);
shared_ptr<ShaderProgram> default_shader;
shared_ptr<ShaderProgram> diffuse_shader;
shared_ptr<ShaderProgram> ads_shader;
ShaderID global_shader;
GLint global_render_primitive = 0;
GLint max_primitives = 3;
GLint global_model_id = 0;

//  Function Declarations
void Init( );
void GlutIdle( );
void GlutReshape( int newWidth, int newHeight );
void GlutDisplay( );
void GlutKeyboard( unsigned char key, int x, int y );
void GlutMouse( int button, int state, int x, int y );
void GlutMouseMotion( int x, int y);
void CleanupAndExit( );
//  Models
void GenerateModels( );
//  Entities
void GenerateEntities( );
//  Shaders
void GenerateShaders( );
//  Render Primitives
GLint UnmapRenderPrimitive ( int rp );

//  Globalized user vars
GLfloat strafe{ 1.0f }, height{ 0.0f }, depth{ 15.0f }, rotate{ 0.0f };
glm::vec3 initial_camera_position(strafe, height, depth);
shared_ptr<Camera> camera { new Camera( initial_camera_position, glm::vec3(0.0f)) };
glm::vec3 mouse_pos_cur = glm::vec3(0.0f);
glm::vec3 mouse_pos_offset = glm::vec3(0.0f);
bool mouse_engaged = false;

// MAIN //
int main( int argc, char **argv ) {
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    glutInitContextVersion (3, 1);
    glutInitContextFlags (GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

    display = shared_ptr<RenderTarget> {new RenderTarget() };
    glutInitWindowSize( display->getWidth(), display->getHeight() );

    glutCreateWindow( argv[0] );
    if ( glewInit( ) ) {
        cerr << "Unable to initialize GLEW ... exiting " << endl;
        exit( EXIT_FAILURE );
    }

    //  Initialize common systems

    //  Renderer
    renderer = shared_ptr<Renderer> {new Renderer()};
    renderer->set_target(display);
    renderer->set_camera(camera);
    renderer->init();

    //  Load our Application Items
    //  For now order counts
    GenerateShaders( );
    GenerateModels( );

    //  This scene specific items
    GenerateEntities( );

    //  Boiler Plate
    glutDisplayFunc( GlutDisplay );
    glutOverlayDisplayFunc(NULL);
    glutReshapeFunc( GlutReshape );
    glutKeyboardFunc( GlutKeyboard );
    glutMouseFunc( GlutMouse );
    glutMotionFunc( GlutMouseMotion );
    glutPassiveMotionFunc(NULL);
    glutVisibilityFunc(NULL);
    glutEntryFunc(NULL);
    glutSpecialFunc(NULL);
    glutSpaceballMotionFunc(NULL);
    glutSpaceballRotateFunc(NULL);
    glutSpaceballButtonFunc(NULL);
    glutButtonBoxFunc(NULL);
    glutDialsFunc(NULL);
    glutTabletMotionFunc(NULL);
    glutTabletButtonFunc(NULL);
    glutMenuStatusFunc(NULL);
    glutIdleFunc( GlutIdle );

    //  Go forth and loop
    glutMainLoop( );
}

void GenerateShaders( ) {
    //  Shaders
    string shader_dir(__SOAPYBRAIN_SHADER_DIR__);
    try {
        vertex_shader.SourceFile(shader_dir + string("default.vert"));
        fragment_shader.SourceFile(shader_dir + string("default.frag"));
        vertex_shader.Compile();
        fragment_shader.Compile();
        default_shader = shared_ptr<ShaderProgram> { new ShaderProgram };
        default_shader->setName("Default Shader");
        default_shader->addShader(vertex_shader.GetHandle());
        default_shader->addShader(fragment_shader.GetHandle());
        default_shader->link();
        default_shader->unuse();
        default_shader->printActiveUniforms();
    }
    catch (ShaderProgramException excp) {
        cerr << "Diffusion shader exception: " << excp.what () << endl;
        exit (EXIT_FAILURE);
    }
    ShaderID default_shader_id = renderer->add_shader(default_shader);

    /* 
    ads_shader->setName("Ads Shader");
    ads_vertex_shader.SourceFile( shader_dir + string("ads_shading.vert"));
    ads_fragment_shader.SourceFile( shader_dir + string ("ads_shading.frag"));
    */
    global_shader = default_shader_id;
}

void GlutReshape( int newWidth, int newHeight )
{
    display->Reshape(newWidth, newHeight);
}


void GlutDisplay( )
{
    //  Set values in the shader
    renderer->render( scene_graph );
    glutSwapBuffers( );
}

void GlutMouse( int button, int state, int x, int y )
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouse_pos_cur.x = x;
        mouse_pos_cur.y = y;
        mouse_pos_offset.x = 0.0f;
        mouse_pos_offset.y = 0.0f;
        mouse_engaged = true;
    } 
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        mouse_engaged = false;
        mouse_pos_cur.x = 0;
        mouse_pos_cur.y = 0;
        mouse_pos_offset.x = 0;
        mouse_pos_offset.y = 0;
    }
}
void GlutMouseMotion( int y, int x)
{
   if (mouse_engaged) {
        mouse_pos_offset.x = mouse_pos_cur.x - x;
        mouse_pos_offset.y = mouse_pos_cur.y - y;
        mouse_pos_cur.x = x;
        mouse_pos_cur.y = y;
        gqueue.push(queue_events::CAMERA_MOVEMENT);
   }
}

void entity_update()
{
    for (auto element = scene_graph.begin(); element != scene_graph.end(); ++element){
        (*element)->_rotation.y += .1f;
    }
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
        gqueue.push( queue_events::MOVE_FORWARD );
        break;
    case 'w':
    case 'W':
        gqueue.push( queue_events::MOVE_BACKWARD );
        break;
    case 'j':
    case 'J':
        gqueue.push( queue_events::MOVE_UP );
        break;
    case '-':
        gqueue.push( queue_events::PITCH_UP );
        break;
    case '+':
        gqueue.push( queue_events::PITCH_DOWN );
        break;
    case 'k':
    case 'K':
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
    case 'r':
    case 'R':
        gqueue.push( queue_events::RENDER_PRIMITIVE_CHANGE );
        break;
    case 'z':
    case 'Z':
        gqueue.push( queue_events::RESET );
        break;
    case 'p':
    case 'P':
        gqueue.push( queue_events::PRINT_ALL);
        break;
    }
}

const glm::vec3 back_movement(0.0f, 0.0f, -1.0f);
const glm::vec3 forward_movement(0.0f, 0.0f, 1.0f);
const glm::vec3 left_movement( 0.3f, 0.0f, 0.0f);
const glm::vec3 right_movement( -0.3f, 0.0f, 0.0f);
const glm::vec3 up_movement(0.0f, 1.0f, 0.0f);
const glm::vec3 down_movement(0.0f, -1.0f, 0.0f);

void GlutIdle( )
{
    //  Pump the events loop
    while ( !gqueue.empty( ) ) {
        switch ( gqueue.front( ) ) {
        case queue_events::PRINT_ALL:
            std::cout << "Printing camera" << std::endl;
            std::cout << *camera << std::endl;
            std::cout << "Printing Models" << std::endl;
            for (int i = 0; i < scene_graph.size(); i++)
                std::cout << *scene_graph[i] << std::endl;
            break;

        case queue_events::MOVE_FORWARD:
            selected->relativeMove(forward_movement);
            break;
        case queue_events::MOVE_BACKWARD:
            selected->relativeMove(back_movement);
            break;
        case queue_events::STRAFE_RIGHT:
            selected->relativeMove(right_movement);
            break;
        case queue_events::STRAFE_LEFT:
            selected->relativeMove(left_movement);
            break;
        case queue_events::YAW_RIGHT:
            selected->rotate(up_movement);
            break;
        case queue_events::YAW_LEFT:
            selected->rotate(down_movement);
            break;
        case queue_events::MOVE_UP:
            selected->move(up_movement);
            break;
        case queue_events::MOVE_DOWN:
            selected->move(down_movement);
            break;
        case queue_events::PITCH_UP:
            selected->rotate(right_movement);
            break;
        case queue_events::PITCH_DOWN:
            selected->rotate(left_movement);
            break;
        case queue_events::COLOR_CHANGE:
//            color = ( color >= 4 ? 1 : color + 1 );
            break;
        case queue_events::RENDER_PRIMITIVE_CHANGE:
            global_render_primitive += 1;
            global_render_primitive %= max_primitives;
            for ( int i = 0; i < renderer->models.size(); i++)
                renderer->models[i]->renderPrimitive = UnmapRenderPrimitive(global_render_primitive);
            break;
        case queue_events::MODEL_CHANGE:
            global_model_id += 1;
            global_model_id %= renderer->models.size();
            for (auto element = scene_graph.begin(); element != scene_graph.end(); ++element){
                (*element)->model_id = global_model_id;
            }
            break;
        case queue_events::CAMERA_MOVEMENT:
            camera->rotate(mouse_pos_offset * -0.001f);
            break;
        case queue_events::RESET:
            camera->setPosition(initial_camera_position);
            camera->setOrientation(glm::vec3(0.0f));
            break;
        case queue_events::APPLICATION_QUIT:
            CleanupAndExit( );
        }
        gqueue.pop( );
    }
    entity_update();
    renderer->render(scene_graph);
    glutPostRedisplay( );
}

void CleanupAndExit( )
{
    exit( EXIT_SUCCESS );
}

void GenerateModels( ) {
    std::string task = "Generating Models";
    std::cout << task << std::endl;
    int ext = 0;
    shared_ptr<Simple_equation_model_t> tmp;
    shared_ptr<VBOTorus> tmpt;

    //  Generate Torus
    tmpt = shared_ptr<VBOTorus> { new VBOTorus (0.7f, 0.3f, 50, 50) };
    tmpt->name = "vbo_torus";
    tmpt->renderPrimitive = UnmapRenderPrimitive(global_render_primitive);
    tmpt->shader = global_shader;
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
        tmp->renderPrimitive = UnmapRenderPrimitive(global_render_primitive);;
        tmp->setup_render_model( );
        tmp->shader = global_shader;
        renderer->add_model( tmp );
    }
    cout << task << " - Done" << endl;
}

void GenerateEntities( ) {
   std::string task = "Generating Scene Entities";
   std::cout << task << std::endl;
   //  Actors
    GLfloat a = 0.1f;
    for ( int i = 0; i < 4; i++) {
        shared_ptr<Actor> actor = shared_ptr<Actor>{ 
            new Actor( 
                glm::vec3( a+(1.5f*i), 0.0f, 0.0f),
                glm::vec3( a+(1.5f*i), 0.0f, 0.0f), 
                global_model_id 
            )
        };
        scene_graph.push_back(actor);
    }
    std::cout << task << " - Done" << std::endl;
    selected = camera;
}

int UnmapRenderPrimitive (int rp)
{
    switch (rp) {
    case 0:
        return GL_POINTS;
    case 1:
        return GL_LINES;
    case 2:
        return GL_TRIANGLES;
    default:
        return GL_POINTS;
    }
}
