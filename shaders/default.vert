#version 130

//layout(location = 0) in vec4 vPosition;

uniform mat4 VP;
uniform mat4 M;
in vec4 vPosition;

void main (void)
{
  gl_Position =  VP * M * vPosition;
}
