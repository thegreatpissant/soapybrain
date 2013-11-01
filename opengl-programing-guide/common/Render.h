#ifndef __RENDER_H__
#define __RENDER_H__

//  Std lib
#include <unordered_map>
#include <vector>

//  Libs
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glew.h>

// 3rd Party
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//  Engine lib
#include "Display.h"
#include "Model.h"
#include "Shader.h"
#include "Actor.h"
#include "Camera.h"

//  Externs <- Remove these
extern Display display;
extern GLuint program;
extern GLint model_matrix_loc;

class Renderer {
  public:
    std::unordered_map<ModelID, shared_ptr<Model>> models;
    std::unordered_map<ShaderID, shared_ptr<Shader>> shaders;

    Renderer( ) {};

    void render( std::vector<shared_ptr<Actor>> &actors ) {

        //  Select appropriate shaders for this model
        //  Will Chain standard MVW transforms as well as effects

        //  Unload the shader and continue
        glClear( GL_COLOR_BUFFER_BIT );
        glViewport( 0, 0, display.screen_width, display.screen_height );
        glUseProgram( program );

        glm::mat4 model_matrix;
        for ( auto a : actors ) {
	  model_matrix = glm::rotate( glm::mat4(), a->state.orientation_x,
				       glm::vec3( 1.0f, 0.0f, 0.0f ) );
	  model_matrix = glm::rotate( model_matrix, a->state.orientation_y,
				      glm::vec3(0.0f, 1.0f, 0.0f) );
	  model_matrix = glm::rotate( model_matrix, a->state.orientation_z,
				      glm::vec3(0.0f, 0.0f, 1.0f) );

	  model_matrix = glm::translate( model_matrix,
					 glm::vec3( a->state.position_x, a->state.position_y, a->state.position_z ) );
	  glUniformMatrix4fv( model_matrix_loc, 1, GL_FALSE, &model_matrix[0][0] );
	  models[a->model_id]->render( a->state );
        }

        glBindVertexArray( 0 );
        glUseProgram( 0 );

        glFinish( );
        glutSwapBuffers( );
    }
    ;

    void add_model( ModelID mid, shared_ptr<Model> model ) {
        models[mid] = model;
    }

    void init( ) { glClearColor( 0.0, 0.0, 0.0, 1.0 ); }
    ;
};

#endif
