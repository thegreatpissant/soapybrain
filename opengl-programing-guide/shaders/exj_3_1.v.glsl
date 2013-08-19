//#version 330 

//layout(location = 0) in vec4 vPosition;

uniform mat4 mMVP;
uniform mat4 mCamera;  
attribute vec4 vPosition;

void main (void) 
{
  gl_Position =   1.4f * mMVP * mCamera * vPosition;

}
