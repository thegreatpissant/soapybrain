//#version 330 
//layout(location = 0) in vec4 vPosition;

uniform float angle;

uniform mat4 mRot;
uniform mat4 mProj;
attribute vec4 vPosition;

void main (void) 
{
  mat4 ident = mat4 ( 
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
                      );

  gl_Position = vPosition * mRot * mProj;
}
