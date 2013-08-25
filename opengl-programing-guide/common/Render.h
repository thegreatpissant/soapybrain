#ifndef __RENDER_H__
#define __RENDER_H__

//  Std lib
#include <unordered_map>
#include <vector>

//  Libs
#include <GL/glew.h>
#include <GL/freeglut.h>
// 3rd Party
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//  Engine lib
#include "Model.h"
#include "Shader.h"
#include "Actor.h"

//  Externs <- Remove these
extern int screenWidth, screenHeight;
extern GLuint program;
extern GLint model_matrix_loc;

class Renderer {
 public:
  std::unordered_map <ModelID, shared_ptr<Model>> models;
  std::unordered_map <ShaderID, shared_ptr<Shader>> shaders;
  std::vector <shared_ptr<Actor>> actors;
  Renderer () { 
  };
  void addActor ( shared_ptr<Actor> a ) {
    actors.push_back(a);
  };

  void render () {
    //  Select appropriate shaders for this model
    //  Will Chain standard MVW transforms as well as effects

    //  Unload the shader and continue
    glm::mat4 model_matrix;
    glClear (GL_COLOR_BUFFER_BIT);
    glViewport ( 0, 0, screenWidth, screenHeight );
    glUseProgram(program);
    for ( auto a: actors ) {
      model_matrix = glm::translate (glm::mat4(), glm::vec3 (a->state.position_x, a->state.position_y, a->state.position_z));
      glUniformMatrix4fv( model_matrix_loc, 1, GL_FALSE, &model_matrix[0][0] ); 
      models[a->model_id]->render(a->state);
    }
    glBindVertexArray (0);
    glUseProgram (0);
    glFinish ();
    glutSwapBuffers ();

    };
};

#endif
