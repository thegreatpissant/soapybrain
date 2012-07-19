/*
  Includes
*/
// This is the planets example.

#include <GL/gl.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include "charset.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define GWH 800
 

#define XCORD 0.0
#define YCORD 0.0
#define ZCORD -200.0
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
GLfloat rot = 0;

void MainMenuDisplay(void)
/*
 * Main Menu screen.
 */
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
    glTranslatef ( xcord, ycord, zcord);
    glRotatef( yrotate, 1.0, 0.0, 0.0);
    glRotatef( xrotate, 0, 1.0, 0);
    glRotatef( yrotate, 1.0, 0, 0);
    glPushMatrix ();
    {
      glColor3f (1.0, 1.0, 0.0);
      glScalef (10.6, 10.6, 10.6);
      glCallList (1);
    } glPopMatrix ();
    glPushMatrix ();
    {
      glTranslatef (100, 0.0, 0.0);
      glColor3f (0.0, 0.0, 1.0);
      glCallList (1);
      glPushMatrix ();
      {
	glRotatef (xrotate*6, 0.0, 1.0,0);
	glTranslatef (10, 0, 0);
	glScalef (0.2703, 0.2703, 0.2703);
	glColor3f (0.5, 0.5, 0.5);
	glCallList (1);
      }glPopMatrix ();
    }glPopMatrix ();
  glFlush();
  glutSwapBuffers();
}

void init_viewList()
{
  glNewList (1, GL_COMPILE);
  glutSolidSphere( 1, 30, 30);
  glEndList ();
}

/*
 * Whole view for the entire Window.
 */
void init_view(void)
{
  glClearColor(0.0,0.0,0.0,1.0);
  glShadeModel(GL_SMOOTH);
  glEnable (GL_DEPTH_TEST);
  glFrontFace (GL_CCW);
  glEnable (GL_CULL_FACE);
  glCullFace (GL_FRONT|GL_BACK);
  init_viewList();
}

void reshape (int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  //  glViewport (w/2, h, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  //  gluOrtho2D (-(0.5f*w), (GLdouble) 0.5f*w, -(0.5f*h), (GLdouble) 0.5*h);
  //  gluOrtho2D (-10.0f, 10.0f, -10.0f, 10.0f);
  gluPerspective (60, 1.0, 0.5, 12000.0);
  //  glOrtho    ( -150.0f, 150.0f, -150.0f, 150.0f, -150.0f, 150.0f);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
} 


void keyboard_char (unsigned char key, int x, int y);
void keyboard_special (int key, int x, int y);
void mouse( int button, int state, int x, int y);
void motion_function (int x, int y);
void idlefunction ();
void init_viewList();
void mainMenu (int);
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
  glutCreateMenu(mainMenu);
  glutAddMenuEntry ("one", 1);
  glutAddMenuEntry ("two", 2);
  glutAttachMenu (GLUT_RIGHT_BUTTON);
  //  createGLUTMenus ();
  init_view();
  glutDisplayFunc(MainMenuDisplay);
  glutReshapeFunc (reshape);
  glutKeyboardFunc(keyboard_char);
  glutSpecialFunc(keyboard_special);
  glutMotionFunc (motion_function);
  glutMouseFunc(mouse);
  glutIdleFunc(idlefunction);

  glutMainLoop();
  return 0;
}
void mainMenu (int value) 
{
  printf ("Button clicked with value: %d\n", value);
}
void idlefunction () 
{
	usleep (20);
		xrotate += 0.01;
	if (xrotate == 360)
	  xrotate = 0;
  glutPostRedisplay();		
}
/*
 * 	Mouse Events.
 */
void motion_function (int x, int y)
{
  if (rotate_obj == true)
    {
      //      printf ("x %d, y %d\n", x,y);
      xrotate = x - xmouse_press;
      yrotate = y - ymouse_press;
      //  printf ("xrotate: %f, yrotate: %f \n", xrotate,yrotate);
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
    zcord += 20.0;
#ifdef DEBUG_KEYS
    printf("Down\n");
#endif
    break;
  case GLUT_KEY_UP :
    zcord -= 20.0;
#ifdef DEBUG_KEYS
    printf("Up\n");
#endif
    break;
  case GLUT_KEY_LEFT:
    xcord -= move_delta;
#ifdef DEBUG_KEYS
    printf ("Key up\n");
#endif
    break;
  case GLUT_KEY_RIGHT:
    xcord += move_delta;
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
    printf ("Maxcubes : %d\n",maxCubes);
    break;
  case 'c':
    maxCubes -= 1;
    if (maxCubes < 1)
      maxCubes = 1;
    break;
  case 'a': // left
    xcord -= move_delta;
    break;
  case 'd': // right
    xcord += move_delta;
    break;
  case 'w': // 2D UP
    ycord += move_delta;
    break;
  case 's': // 2D DOWN
    ycord -= move_delta;
    break;
  case 'r': // Reset cords
    xcord = XCORD, ycord = YCORD, zcord = ZCORD;    
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
