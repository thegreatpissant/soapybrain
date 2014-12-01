#version 130

vec4 fColor;
uniform int color;

void main () {
  if (color == 2) {
    fColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );
  } else if (color == 3) {
    fColor = vec4 ( 0.0, 1.0, 0.0, 1.0 );
  } else if (color == 1) {
    fColor = vec4 ( 0.0, 0.0, 1.0, 0.0 );
  } else {
    fColor = vec4 ( 1.0 , 1.0, 1.0, 1.0 );
  }

  gl_FragColor = fColor;
}
