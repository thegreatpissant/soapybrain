#version 330

layout(location = 0) in vec4 vPosition;

uniform mat4 MVP;

//in vec4 vPosition;

void main (void)
{
  gl_Position =  MVP * vPosition;
}
