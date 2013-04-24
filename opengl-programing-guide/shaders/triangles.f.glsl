vec4 fColor;
uniform int color;
void main () {
  if (color == 1) {
    fColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );
} else if (color == 2) {
    fColor = vec4 ( 0.0, 1.0, 0.0, 1.0 );
} else if (color == 3) {
    fColor = vec4 ( 0.0, 0.0, 1.0, 0.0 );
} else {
    fColor = vec4 ( 1.0 , 1.0, 1.0, 1.0 );
}
/*
 switch (color) {
  case 1:
    fColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );
    break;
  case 2:
    fColor = vec4 ( 0.0, 1.0, 0.0, 1.0 );
    break;
  case 3:
    fColor = vec4 ( 0.0, 0.0, 1.0, 0.0 );
    break;
  default: 
    fColor = vec4 ( 1.0 , 1.0, 1.0, 1.0 );
    break;
  }
*/
	gl_FragColor = fColor;
}
