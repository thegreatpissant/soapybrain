#version 400

layout (location = 0) in vec3 vPosition;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main (void)
{
  gl_Position = Projection * View * Model * vec4(vPosition, 1.0);
}
