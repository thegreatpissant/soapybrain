//#version 330 
//layout(location = 0) in vec4 vPosition;
uniform float angle;
attribute vec4 vPosition;
void main (void) 
{
  mat4 ident = mat4 ( 
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
                      );
  mat4 rotz = mat4 ( cos(angle), -sin(angle), 0.0, 0.0,
                    sin(angle), cos(angle), 0.0, 0.0,
                    0.0, 0.0, 1.0, 0.0,
                    0.0, 0.0, 0.0, 1.0 );
  mat4 rotx = mat4 ( 1.0, 0, 0, 0,
                     0, cos(angle/2.0), -sin(angle/2.0), 0,
                     0, sin(angle/2.0), cos(angle/2.0), 0,
                     0, 0, 0, 1 );

  gl_Position = ident * vPosition;
//  gl_Position *= rotz;
  gl_Position *= rotx;
}
