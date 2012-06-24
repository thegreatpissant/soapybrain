/*
  Includes
*/

#include <GL/gl.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include "charset.h"
#include <stdlib.h>
#include <stdio.h>
#define GWH 800
 

#define XCORD 0.0
#define YCORD 0.0
#define ZCORD 0.0
GLfloat xcord = XCORD, ycord = YCORD, zcord = ZCORD;
void initCords ();
GLfloat wcord = 1.0f;
GLfloat move_inc = 0.1f;
GLfloat move_delta = 1.0f * move_inc;
GLfloat xrotate = 0.0, yrotate = 0.0;
GLint xmouse_delta = 0, ymouse_delta = 0;
GLint xmouse_press = 0, ymouse_press = 0;
GLboolean  rotate_obj = false;
GLfloat obj_size = 1.0f;
GLdouble offset[] = { 0.0f, 0.0f, 0.0f};
GLfloat red_array[] = { 1.0, 0.0, 0.0 };
GLfloat blue_array[] = { 0.0, 0.0, 1.0 };
GLfloat green_array[] = { 0.0, 1.0, 0.0 };
GLenum drawMode = GL_POINTS;
GLfloat head_rotation = 0;
int maxCubes = 20;
GLfloat jump = 2.0f;
GLfloat cubeposx = 0.0f;
GLfloat cubeposy = 0.0f;
GLfloat cubeposz = 0.0f;

GLubyte fly [] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x03, 0x80, 0x01, 0xC0, 0x06, 0xC0, 0x03, 0x60,
  0x04, 0x60, 0x06, 0x20, 0x04, 0x30, 0x0C, 0x20,
  0x04, 0x18, 0x18, 0x20, 0x04, 0x0C, 0x30, 0x20,
  0x04, 0x06, 0x60, 0x20, 0x44, 0x03, 0xC0, 0x22,
  0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
  0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
  0x44, 0x01, 0x80, 0x22, 0x44, 0x01, 0x80, 0x22,
  0x66, 0x01, 0x80, 0x66, 0x33, 0x01, 0x80, 0xCC,
  0x19, 0x81, 0x81, 0x98, 0x0C, 0xC1, 0x83, 0x30,
  0x07, 0xE1, 0x87, 0xE0, 0x03, 0x3F, 0xFC, 0xC0,
  0x03, 0x31, 0x8C, 0xC0, 0x03, 0x33, 0xCC, 0xC0,
  0x06, 0x64, 0x26, 0x60, 0x0C, 0xCC, 0x33, 0x30,
  0x18, 0xCC, 0x33, 0x18, 0x10, 0xC4, 0x23, 0x08,
  0x10, 0x63, 0xC6, 0x08, 0x10, 0x30, 0x0C, 0x08,
  0x10, 0x18, 0x18, 0x08, 0x10, 0x00, 0x00, 0x08 };
  
GLfloat cubeColorArray[] = {
  0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 1.0f,
  1.0f, 0.0f, 0.0f,
  1.f, 0.0f, 1.0f,
  0.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f,
  1.0f, 1.0f, 1.0f
};
GLfloat houseColorArray[] = {
  0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 1.0f,
  1.0f, 0.0f, 0.0f,
  1.f, 0.0f, 1.0f,
  0.0f, 1.0f, 1.0f,
  1.0f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f
};
GLfloat houseColorArray2[] = {
  1.f, 0.0f, 0.0f,
  0.8f, 0.0f, 1.0f,
  0.4f, 1.0f, 0.4f,
  0.7f, 1.0f, 0.0f,
  1.0f, 0.1f, 0.5f,
  0.6f, 0.5f, 1.0f,
  0.3f, 0.5f, 1.0f
};
GLfloat ManVertexArray3[] = {
  0.0f, 10.0f, 0.0f, // a
  -2.0f,  9.0f, 0.0f, // b
  2.0f,  9.0f, 0.0f, // c
  0.0f,  3.0f, 0.0f, // d
  -3.0f,  0.0f, 0.0f, // e
  3.0f,  0.0f, 0.0f, // f
  0.0f,  6.0f, 0.0f, // g
  2.0f, 10.0f, 0.0f, // h
  0.0f, 14.0f, 0.0f,
  -2.0f, 14.0f, 0.0f,
  -2.0f, 10.0f, 0.0f
};
static GLubyte manIndicies[] = {
  0, 3,  //  Spine
  6, 1,  //  L Arm
  6, 2,  //  R Arm
  3, 4,  //  L Leg
  3, 5,  //  R leg
  7,8,9,10  // head
};
GLfloat manColorArray[] = {
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f
};


GLfloat cubeVertexArray [] = {
  0.0f, 0.0f, 1.0f,  // 0
  1.0f, 0.0f, 1.0f,  // 1
  1.0f, 1.0f, 1.0f,  // 2
  0.0f, 1.0f, 1.0f,  // 3
  0.0f, 0.0f, 0.0f,   // 4
  1.0f, 0.0f, 0.0f,  // 5
  1.0f, 1.0f, 0.0f,  // 6
  0.0f, 1.0f, 0.0f  // 7
};

GLfloat houseVertexArray[] = {
  4.0f, 0.0f,  // 0
  -4.0f, 0.0f, // 1
  -4.0f, 5.0f, // 2
  -7.0f, 5.0f, // 3
  0.0f, 10.0f, // 4
  7.0f, 5.0f,  // 5
  4.0f, 5.0f   // 6
};
static GLubyte roofIndicies[] = { 4, 3, 5 };
static GLubyte wallIndicies[] = { 2, 1, 0, 6 };
static GLubyte cubeIndicies[] = { 
  0, 1, 2, 3, 
  3, 2, 4, 5, 
  6, 5, 4, 7, 
  6, 0, 1, 7, 
  6, 0, 3, 5, 
  2, 1, 7, 4
};
static GLubyte cubefront[]  = {  0, 1, 2, 3 };
static GLubyte cuberight[]  = {  1, 5, 6, 2 };
static GLubyte cubeback[]   = {  5, 4, 7, 6 };
static GLubyte cubeleft[]   = {  3, 7, 4, 0 };
static GLubyte cubetop[]    = {  6, 7, 3, 2 }; 
static GLubyte cubebottom[] = {  4, 5, 1, 0 };
void drawCube ();
void drawMan  ();
void drawAxis ();
void drawHouse ();


void MainMenuDisplay(void)
/*
 * Main Menu screen.
 */
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix ();
  {

    //    glTranslatef (-xcord, -ycord, -zcord);
    if (rotate_obj)
      {
	glRotatef (yrotate, 1.0, 0.0, 0.0);
	glRotatef (xrotate, 0.0, 1.0, 0.0);
      }
    //  glPushMatrix(); {
      glTranslatef (-xcord, -ycord, -zcord);
      int numCubesX = 0;
      int numCubesY = 0;
      int numCubesZ = 0;
      //      GLfloat tcubeposx = cubeposx;
      //      GLfloat tcubeposy = cubeposy;
      //      GLfloat tcubeposz = cubeposz;
      GLfloat tcubeposx = 0;
      GLfloat tcubeposy = 0;
      GLfloat tcubeposz = 0;
      for (numCubesZ = 0; numCubesZ < maxCubes; numCubesZ += 1, tcubeposz += jump)
	{
	  for (numCubesY = 0; numCubesY < maxCubes; numCubesY += 1, tcubeposy += jump)
	    {
	      for (numCubesX = 0; numCubesX < maxCubes; numCubesX+=1, tcubeposx += jump)
		{
		  glPushMatrix ();
		  {
		    glTranslatef (tcubeposx, tcubeposy, tcubeposz);
		    drawCube ();
		  } glPopMatrix ();
		}
	      tcubeposx = 0;
	    }
	  tcubeposy = 0;
	}
      //    } glPopMatrix ();
    //  drawMan ();
    //  drawAxis ();
    //  drawHouse ();
    //  glutWireSierpinskiSponge ( 5 , offset, 1.0f );
    //  glutWireRhombicDodecahedron( );
    //  glutWireTapot (obj_size);
    //  glutWireTorus( 0.5f , 1.0f, 60, 60 );
    //  glutWireTorus( 3.0f , 10.0f, 60, 60 );
    //  glLineStipple (1, 0x0101);
  } glPopMatrix();
  glFinish();
  glutSwapBuffers();
}
void drawAxis ()
{
  glPushMatrix ();
  {
    //  Draw the axis
    glBegin(GL_LINES);
    glColor3f  (1.0f, 0.0f, 0.0f);
    glVertex3f (-1.0f, 0.0f, 0.0f);
    glVertex3f (2.0f, 0.0f, 0.0f);
    glColor3f  (0.0f, 1.0f, 0.0f);
    glVertex3f (0.0f, -1.0f, 0.0f);
    glVertex3f (0.0f, 2.0f, 0.0f);
    glColor3f  (0.0f, 0.0f, 1.0f);
    glVertex3f (0.0f, 0.0f, -1.0f);
    glVertex3f (0.0f, 0.0f, 2.0f);
    glEnd();
  } glPopMatrix ();
}
void drawHouse ()
{
  glEnable (GL_POLYGON_STIPPLE);
  glPolygonStipple (fly);  
  glVertexPointer (2, GL_FLOAT, 0, houseVertexArray);
  glColorPointer (3, GL_FLOAT, 0, houseColorArray);
  glPushMatrix ();
  {
    glDrawElements (GL_POLYGON, 3, GL_UNSIGNED_BYTE, roofIndicies);
    glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, wallIndicies);
  } glPopMatrix ();
  glDisable (GL_POLYGON_STIPPLE);
}
void drawMan ()
{
  //  Jimmys Man
  glVertexPointer (3, GL_FLOAT, 0, ManVertexArray3);
  glColorPointer  (3, GL_FLOAT, 0, manColorArray);
  glPushMatrix(); {
    glDrawElements (GL_LINES, 2, GL_UNSIGNED_BYTE, &(manIndicies[0]));
    glDrawElements (GL_LINES, 2, GL_UNSIGNED_BYTE, &(manIndicies[2]));
    glDrawElements (GL_LINES, 2, GL_UNSIGNED_BYTE, &(manIndicies[4]));
    glDrawElements (GL_LINES, 2, GL_UNSIGNED_BYTE, &(manIndicies[6]));
    glDrawElements (GL_LINES, 2, GL_UNSIGNED_BYTE, &(manIndicies[8]));
    //  glDrawElements (GL_LINE_LOOP, 4, GL_UNSIGNED_BYTE, &(manIndicies[10]));  glPopMatrix();
    glTranslatef (ManVertexArray3[0]-1.0f, ManVertexArray3[1], ManVertexArray3[2]-1.0f);
    glScalef (2.0f, 2.0f, 2.0f);
    drawCube ();
  }  glPopMatrix ();
}

void drawCube () 
{
  //  Cube
  glVertexPointer (3, GL_FLOAT, 0, cubeVertexArray);
  glColorPointer  (3, GL_FLOAT, 0, cubeColorArray);
  glPushMatrix(); {
    glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, cubefront);
    glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, cuberight);
    glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, cubeback);
    glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, cubeleft);
    glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, cubetop);
    glDrawElements (GL_POLYGON, 4, GL_UNSIGNED_BYTE, cubebottom);
  }  glPopMatrix();
}


/*
 * Whole view for the entire Window.
 */
void init_view(void)
{
  glClearColor(0.0,0.0,0.0,1.0);
  glShadeModel(GL_SMOOTH);
  glEnableClientState (GL_VERTEX_ARRAY);
  glEnableClientState (GL_COLOR_ARRAY);
  glPointSize (0.0f);
  //  glEnable(GL_LINE_STIPPLE);
  glPolygonMode (GL_FRONT, GL_FILL);
  glPolygonMode (GL_BACK,  GL_LINE);
  glFrontFace (GL_CCW);
  glEnable (GL_CULL_FACE);
  glCullFace (GL_BACK);
  initCords ();

}

void initCords ()
{
  xcord = maxCubes*jump/2.0f;
  ycord = xcord;
  zcord = ycord;
}
void reshape (int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  //  gluOrtho2D (-(0.5f*w), (GLdouble) 0.5f*w, -(0.5f*h), (GLdouble) 0.5*h);
  //  gluOrtho2D (-10.0f, 10.0f, -10.0f, 10.0f);
  glOrtho    ( -50.0f, 50.0f, -50.0f, 50.0f, -50.0f, 50.0f);
} 


void keyboard_char (unsigned char key, int x, int y);
void keyboard_special (int key, int x, int y);
void mouse( int button, int state, int x, int y);
void motion_function (int x, int y);

int main (int argc, char ** argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  //  Window size    width, height
  glutInitWindowSize(GWH, GWH);
  //                     posx, posy
  glutInitWindowPosition(200, 100);
  //  Create new window.
  glutCreateWindow("One");
  init_view();
  glutDisplayFunc(MainMenuDisplay);
  glutReshapeFunc (reshape);
  glutKeyboardFunc(keyboard_char);
  glutSpecialFunc(keyboard_special);
  glutMotionFunc (motion_function);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}

/*
 * 	Mouse Events.
 */
void motion_function (int x, int y)
{
  if (rotate_obj == true)
    {
      printf ("x %d, y %d\n", x,y);
      xrotate = x - xmouse_press;
      yrotate = y - ymouse_press;
      printf ("xrotate: %f, yrotate: %f \n", xrotate,yrotate);
    }
  glutPostRedisplay();		
}

void mouse( int button, int state, int x, int y)
{
  switch (button) {
  case  GLUT_DOWN:
    xmouse_press = x;
    ymouse_press = y;
    rotate_obj = true;
#ifdef DEBUG_KEYS
    printf("Mouse GLUT_DOWN pressed\n");
    printf ("xmouse_press: %d, ymouse_press: %d \n", xmouse_press, ymouse_press);
#endif
    break;
  case  GLUT_UP:
    rotate_obj = false;
#ifdef DEBUG_KEYS
    printf ("Mouse GLUT_UP pressed\n");
    printf ("xmouse_press: %d, ymouse_press: %d \n", xmouse_press, ymouse_press);
#endif
    break;
  }
  glutPostRedisplay();		
}
void keyboard_special (int key, int x, int y)
/*
 * Handles all special key events.
 */
{
  switch(key) {
  case GLUT_KEY_DOWN :
    zcord += move_delta;
    initCords ();
#ifdef DEBUG_KEYS
    printf("Down\n");
#endif
    break;
  case GLUT_KEY_UP :
    zcord -= move_delta;
    initCords ();
#ifdef DEBUG_KEYS
    printf("Up\n");
#endif
    break;
  case GLUT_KEY_LEFT:
    xcord -= move_delta;
    initCords ();
#ifdef DEBUG_KEYS
    printf ("Key up\n");
#endif
    break;
  case GLUT_KEY_RIGHT:
    xcord += move_delta;
    initCords ();
#ifdef DEBUG_KEYS
    printf ("Key right\n");
#endif
    break;
  case GLUT_KEY_PAGE_UP:
    obj_size += 0.05;
#ifdef DEBUG_KEYS
    printf ("Key PGUP\n");
#endif
    break;
  case GLUT_KEY_PAGE_DOWN:
    obj_size -= 0.05;
#ifdef DEBUG_KEYS
    printf ("Key PGDWN\n");
#endif
    break;
  default: 
    break;
  }
#ifdef DEBUG_KEYS
  printf ("Key code: %d\n", key);
#endif
  glutPostRedisplay();		
}

void keyboard_char (unsigned char key, int x, int y)
/*
 * Handles all Character key events.
 */
{
  switch(key) {
  case 27:
    exit(0);
    break;
  case 32:
    break;
  case 'C':
    maxCubes += 1;
    initCords ();
    break;
  case 'c':
    maxCubes -= 1;
    if (maxCubes < 1)
      maxCubes = 1;
    initCords ();
    break;
  case 'a': // left
    xcord -= move_delta;
    initCords ();
    break;
  case 'd': // right
    xcord += move_delta;
    initCords ();
    break;
  case 'w': // 2D UP
    ycord += move_delta;
    initCords ();
    break;
  case 's': // 2D DOWN
    ycord -= move_delta;
    initCords ();
    break;
  case 'r': // Reset cords
    xcord = XCORD, ycord = YCORD, zcord = ZCORD;    
    initCords ();
    yrotate = 0.0f; xrotate = 0.0f;
    jump = 2;
    break;
  case 'J': // increase our jump
    jump += 0.1;
    break;
  case 'j': // decrease our jump
    jump -= 0.1;
    break;
  case 49: 
    break;
  case 50:
    break;
  case 51:
    break;
  case 52:
    break;
  case 53:
    break;
  default:
    break;
  }
#ifdef DEBUG_KEYS
  printf("Keyboard char: %c, Key code: %d\n", key, key); 
#endif
  glutPostRedisplay();
}
