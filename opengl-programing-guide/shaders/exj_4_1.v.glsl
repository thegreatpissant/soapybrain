//#version 330 

//layout(location = 0) in vec4 vPosition;

uniform mat4 mMVP;
uniform mat4 mCamera;  
uniform mat4 model_matrix;
attribute vec4 vPosition;

void main (void) 
{
  gl_Position =  mMVP * mCamera * model_matrix * vPosition;

}
