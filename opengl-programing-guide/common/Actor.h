#ifndef __ACTOR_H__
#define __ACTOR_H__

#include "Entity.h"
#include "Model.h"
#include "Shader.h"

class Actor : public Entity {
private:
    ShaderProgram shader_program;
  public:
    ModelID model_id;
    Actor( ) : Entity( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f ), model_id{ 0 } {};
    Actor( float px, float py, float pz, float ox, float oy, float oz,
           ModelID mid = 0 )
        : Entity( px, py, pz, ox, oy, oz ), model_id{ mid } {};
};

#endif

