//#version 330 
//layout(location = 0) in vec4 vPosition;

uniform float angle;

uniform vec4 vPos2;

uniform mat4 mRot;

attribute vec4 vPosition;

void main (void) 
{
  mat4 ident = mat4 ( 
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
                      );

  gl_Position = vPosition;
  gl_Position *= mRot;

}
