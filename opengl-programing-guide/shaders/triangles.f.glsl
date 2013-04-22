out vec4 fColor;
uniform int color;
void main () {
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
}
