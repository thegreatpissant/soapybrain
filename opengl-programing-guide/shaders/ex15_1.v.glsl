//#version 330 

layout(location = 0) in vec4 vPosition;

uniform mat4 mMVP;
//attribute vec4 vPosition;

void main (void) 
{
  gl_Position = vPosition * mMVP;
}
