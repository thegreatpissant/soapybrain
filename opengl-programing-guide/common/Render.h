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
//extern GLint model_matrix_loc;

class Renderer {
private:
    ModelID GID;
public:
    std::unordered_map<ModelID, shared_ptr<Model>> models;
    std::unordered_map<ShaderID, shared_ptr<Shader>> shaders;

    Renderer( ):GID(0) {};

    void render( std::vector<shared_ptr<Actor>> &actors ) {

        //  Select appropriate shaders for this model
        //  Will Chain standard MVW transforms as well as effects

        //  Unload the shader and continue
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glViewport( 0, 0, display.screen_width, display.screen_height );
        glUseProgram( program );

        glm::mat4 model_matrix;
        for ( auto a : actors ) {
            //            model_matrix = glm::rotate( glm::mat4(), a->state.orientation_x,
            //                                        glm::vec3( 1.0f, 0.0f, 0.0f ) );
            //            model_matrix = glm::rotate( model_matrix, a->state.orientation_y,
            //                                        glm::vec3(0.0f, 1.0f, 0.0f) );
            //            model_matrix = glm::rotate( model_matrix, a->state.orientation_z,
            //                                        glm::vec3(0.0f, 0.0f, 1.0f) );

            //            model_matrix = glm::translate( model_matrix,
            //                                           glm::vec3( a->state.position_x, a->state.position_y, a->state.position_z ) );
            //            glUniformMatrix4fv( model_matrix_loc, 1, GL_FALSE, &model_matrix[0][0] );
            get_model (a->model_id)->render( a->state );
        }

        glBindVertexArray( 0 );
        glUseProgram( 0 );

        glFinish( );
        glutSwapBuffers( );
    }
    ;

    void add_model( shared_ptr<Model> model ) {
        std::cout << "Renderer added Model id " << GID << " Name " << model->name << std::endl;
        models[GID] = model;
        ++GID;
    }
    shared_ptr<Model> get_model (ModelID mid) {
        if (models.empty())
        {
            std::cerr << "No models available" << std::endl;
            std::exit (EXIT_FAILURE);
        }
        if (models.find (mid) == models.end())
            return models[0];
        return models[mid];
    }

    void init( ) {
        glEnable(GL_DEPTH_TEST);
        glClearColor( 0.2, 0.2, 0.2, 1.0 );
    }
    ;
};

#endif
