#include "Actor.h"

void Actor::SetShader(ShaderProgram &nshader_program)
{
    shader_program = nshader_program;
}

ShaderProgram Actor::GetShader()
{
    return shader_program;
}
