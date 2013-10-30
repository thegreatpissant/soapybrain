//#version 330 

//layout(location = 0) in vec4 vPosition;

uniform mat4 mMVP;
uniform mat4 mOrientation;
attribute vec4 vPosition;

void main (void) 
{
gl_Position =  mMVP * (mOrientation * vPosition);
//gl_Position =  mOrientation * vPosition;

}
