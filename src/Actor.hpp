#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "Entity.hpp"
#include "Model.hpp"
#include "Shader.hpp"

class Actor : public Entity {
private:
    ShaderProgram shader_program;
  public:
    ModelID model_id;
    Actor( ) : Entity( glm::vec3(0.0f), glm::vec3(0.0f)), model_id{ 0 } {};
    Actor(  glm::vec3 position, glm::vec3 orientation, 
           ModelID mid = 0 )
        : Entity( position, orientation ), model_id{ mid } {};
};

#endif

