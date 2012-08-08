/*
    chTest.h
    Test of the opengl print string function i will inpliment.
    BASICALLY COPIED FROM THE PROGRAMING GUIDE.
*/

#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>
#define P 1
#define L 2
#define E 3


typedef struct point {
    GLfloat x,y;
    int type;
} point;

point lA[] = {
    {0.0f,0.00f,P},{1.5f,5.0f,P},{3.0f,0.0f,L},{0.75f,2.5f,P},{2.25f,2.5f,E}
};
point lB[] ={
            {0.0f,0.0f,P},{0.0f,5.0f,P},{2.0f,5.0f,P},{2.65f,4.65f,P},{3.0f,4.0f,P},{3.0f,3.0f,P},{2.5f,2.5f,P},
			{1.0f,2.5f,P},{2.5f,2.5f,P},{3.0f,2.0f,P},{3.0f,1.0f,P},{2.65f,0.35f,P},{2.0f,0.0f,P},{0.0f,0.0f,E}
};

point lC[] = {
            {3.0f,1.0f,P},{2.65f,0.35f,P},{2.0f,0.0f,P},{1.0f,0.0f,P},{0.35f,0.35f,P},{0.0f,1.0f,P},{0.0f,4.0f,P},
            {0.35f,4.65f,P},{1.0f,5.0f,P},{2.0f,5.0f,P},{2.65f,4.65f,P},{3.0f,4.0f,E}
};

point lD[] = {
            {0.0f,0.0f,P},{0.0f,5.0f,P},{2.0f,5.0f,P},{2.65f,4.65f,P},{3.0f,4.0f,P},{3.0f,1.0f,P},
            {2.65f,0.35f,P},{2.0,0.0,P},{0.0f,0.0f,E}
};

point lE[] = {
            {3.0f,0.0f,P},{0.0f,0.0f,P},{0.0f,5.0f,P},{3.0f,5.0f,L},{0.0f,2.5f,P},{2.0f,2.5f,E}
};
point lF[] = {
            {0.0f,0.0f,P},{0.0f,5.0f,P},{3.0f,5.0f,L},{0.0f,2.5f,P},{2.0f,2.5f,E}
};
point lG[] = {
            {2.0f,1.5f,P},{2.0f,2.0f,P},{3.0f,2.0f,P},{3.0f,1.0f,P},{ 2.65f,0.35f,P},{2.0f,0.0f,P},{1.0f,0.0f,P},
            {0.35f,0.35f,P},{0.0f,1.0f,P},{0.0f,4.0f,P},{0.35f,4.65f,P},{1.0f,5.0f,P},{2.0f,5.0f,P},{2.65f,4.65f,P},{3.0f,4.0f,E}
};
point lH[] = {
            {0.0f,0.0f,P},{0.0f,5.0f,L},{3.0f,0.0f,P},{3.0f,5.0f,L},{0.0f,2.5f,P},{3.0f,2.5f,E}
};
point lI[] = {
            {0.0f,0.0f,P},{3.0f,0.0f,L},{0.0f,5.0f,P},{3.0f,5.0f,L},{1.5f,5.0f,P},{1.5f,0.0f,E}
};
point lJ[] = {
            {3.0f,5.0f,P},{3.0f,1.0f,P},{2.65f,0.35f,P},{2.0f,0.0f,P},{1.0f,0.0f,P},{0.35f,0.35f,P},{0.0f,1.0f,E}
};
point lK[] = {
            {0.0f,0.0f,P},{0.0f,5.0f,L},{3.0f,5.0f,P},{0.0f,2.5f,P},{3.0f,0.0f,E}
};
point lL[] = {
            {0.0f,5.0f,P},{0.0f,0.0f,P},{3.0f,0.0f,E}
};
point lM[] = {
            {0.0f,0.0f,P},{0.0f,5.0f,P},{1.5f,3.0f,P},{3.0f,5.0f,P},{3.0f,0.0f,E}
};
point lN[] = {
            {0.0f,0.0f,P},{0.0f,5.0f,P},{3.0f,0.0f,P},{3.0f,5.0f,E}
};
point lO[] = {
            {0.0f,1.0f,P},{0.0f,4.0f,P},{0.35f,4.65f,P},{1.0f,5.0f,P},{2.0f,5.0f,P},{2.65f,4.65f,P},{3.0f,4.0f,P},
            {3.0f,1.0f,P},{2.65f,0.35f,P},{2.0f,0.0f,P},{1.0f,0.0f,P},{0.35f,0.35f,P},{0.0f,1.0f,E}
};
point lP[] = {
            {0.0f,0.0f,P},{0.0f,2.5f,P},{0.0f,5.0f,P},{2.0f,5.0f,P},{2.65f,4.65f,P},{3.0f,4.0f,P},{3.0f,3.0f,P},
            {2.65f,2.5f,P},{0.0f,2.5f,E}
};
point lQ[] = {
            {0.0f,1.0f,P},{0.0f,4.0f,P},{0.35f,4.65f,P},{1.0f,5.0f,P},{2.0f,5.0f,P},{2.65f,4.65f,P},{3.0f,4.0f,P},
            {3.0f,1.0f,P},{2.65f,0.35f,P},{2.0f,0.0f,P},{1.0f,0.0f,P},{0.35f,0.35f,P},{0.0f,1.0f,L},{2.0f,1.0f,P},{3.0f,0.0f,E}
};
point lR[] = {
            {0.0f,0.0f,P},{0.0f,5.0f,P},{2.0f,5.0f,P},{2.65f,4.65f,P},{3.0f,4.0f,P},{3.0f,3.0f,P},{2.65f,2.5f,P},
            {0.0f,2.5f,L},{2.65f,2.5f,P},{3.0f,0.0f,E}
};
point lS[] = {
            {0.0f,1.0f,P},{0.35f,0.35f,P},{1.0f,0.0f,P},{2.0f,0.0f,P},{2.65f,0.35f,P},{3.0f,1.0f,P},{3.0f,2.0f,P},{2.65f,2.5f,P},{0.35f,2.5f,P},
            {0.0f,3.0f,P},{0.0f,4.0f,P},{0.35f,4.65f,P},{1.0f,5.0f,P},{2.0f,5.0f,P},{2.65f,4.65f,P},{3.0f,4.0f,E}
};
point lT[] = {
            {0.0f,5.0f,P},{3.0f,5.0f,L},{1.5f,0.0f,P},{1.5f,5.0f,E}
};
point lU[] = {
            {0.0f,5.0f,P},{0.0f,1.0f,P},{0.35f,0.35f,P},{1.0f,0.0f,P},{2.0f,0.0f,P},{2.65f,0.35f,P},{3.0f,1.0f,P},{3.0f,5.0f,E}
};
point lV[] = {
            {0.0f,5.0f,P},{1.5f,0.0f,P},{3.0f,5.0f,E}
};
point lW[] = {
            {0.0f,5.0f,P},{0.5f,0.0f,P},{1.5f,3.0f,P},{2.5f,0.0f,P},{3.0f,5.0f,E}
};
point lX[] = {
            {0.0f,5.0f,P},{3.0f,0.0f,L},{0.0f,0.0f,P},{3.0f,5.0f,E}
};
point lY[] = {
            {0.0f,5.0f,P},{1.5f,2.5f,P},{3.0f,5.0f,L},{1.5f,2.5f,P},{1.5f,0.0f,E}
};
point lZ[] = {
            {0.0f,5.0f,P},{3.0f,5.0f,P},{0.0f,0.0f,P},{3.0f,0.0f,E}
};
point l0[] = {
            {0.0f,1.0f,P},{0.0f,4.0f,P},{0.35f,4.65f,P},{1.0f,5.0f,P},{2.0f,5.0f,P},{2.65f,4.65f,P},{3.0f,4.0f,P},
            {3.0f,1.0f,P},{2.65f,0.35f,P},{2.0f,0.0f,P},{1.0f,0.0f,P},{0.35f,0.35f,P},{0.0f,1.0f,L},
	    {0.35f,0.35f,P},{2.65f,4.65f,E}
};
point l1[] = {
	    {1.5f,5.0f,P}, {1.5f,0.0f,E}
};
point l2[] = {
	    {0.0f,4.0f,P},{0.35f,4.65f,P},{1.0f,5.0f,P},{2.0f,5.0f,P},{2.65f,4.65f,P},{3.0f,4.0f,P},{2.65f,3.35f,P},
	    {0.0f,0.0f,P},{3.0f,0.0f,E}
};
point l3[] = {
	    {0.0f,4.0f,P},{0.35f,4.65f,P},{1.0f,5.0f,P},{2.0f,5.0f,P},{2.65f,4.65f,P},{3.0f,4.0f,P},{3.0f,3.0f,P},
	    {2.65f,2.5f,P},{1.0f,2.5f,L},{2.65f,2.5f,P},{3.0f,2.0f,P},{3.0f,1.0f,P},{2.65f,0.35f,P},{2.0f,0.0f,P},
	    {1.0f,0.0f,P},{0.35f,0.35f,P},{0.0f,1.0f,E}
};
point l4[] = {
	    {1.0f,5.0f,P},{0.0f,2.5f,P},{3.0f,2.5f,L},{2.0f,5.0f,P},{2.0f,0.0f,E}
};
point l5[] = {
	    {3.0f,5.0f,P},{0.0f,5.0f,P},{0.0f,2.5f,P},{2.65f,2.5f,P},{3.0f,2.0f,P},{3.0f,1.0f,P},{2.65f,0.35f,P},
	    {2.0f,0.0f,P},{1.0f,0.0f,P},{0.35f,0.35f,P},{0.0f,1.0f,E}
};
point l6[] = {
	    {2.65f,5.0f,P},{0.35f,2.5f,P},{0.0f,2.0f,P},{0.0f,1.0f,P},{0.35f,0.35f,P},{1.0f,0.0f,P},{2.0f,0.0f,P},
	    {2.65f,0.35f,P},{3.0f,1.0f,P},{3.0f,2.0f,P},{2.65f,2.5f,P},{0.35f,2.5f,E}
};
point l7[] = {
	    {0.0f,5.0f,P},{3.0f,5.0f,P},{0.0f,0.0f,E}
};
point l8[] = {
	    {0.35f,0.35f,P},{0.0f,1.0f,P},{0.0f,2.0f,P},{0.35f,2.5f,P},{2.65f,2.5f,P},{3.0f,2.0f,P},{3.0f,1.0f,P},
	    {2.65f,0.35f,P},{2.0f,0.0f,P},{1.0f,0.0f,P},{0.35f,0.35f,L},
	    {0.35f,2.5f,P},{0.0f,3.0f,P},{0.0f,4.0f,P},{0.35f,4.65f,P},{1.0f,5.0f,P},{2.0f,5.0f,P},{2.65f,4.65f,P},
	    {3.0f,4.0f,P},{3.0f,3.0f,P},{2.5f,2.65f,E},
	    
};
point l9[] = {
	    {2.65f,2.5f,P},{0.35f,2.5f,P},{0.0f,3.0f,P},{0.0f,4.0f,P},{0.35f,4.65f,P},{1.0f,5.0f,P},{2.0f,5.0f,P},
	    {2.65f,4.65f,P},{3.0f,4.0f,P},{2.65f,2.5f,P},{0.35f,0.0f,E}
};
point lPERIOD[] = {
            {1.35f,0.35f,P},{1.35f,0.65f,P},{1.65f,0.65f,P},{1.65f,0.35f,P},{1.35f,0.35f,E}
};
point lQUESTION[] = {
            {1.35f,0.35f,P},{1.35f,0.65f,P},{1.65f,0.65f,P},{1.65f,0.35f,P},{1.35f,0.35f,L},
            {1.5f,1.5f,P},{1.5f,3.0f,P},{2.0f,3.0f,P},{2.65f,3.35f,P},{3.0f,4.0f,P},
            {2.65f,4.65f,P},{2.0f,5.0f,P},{1.0f,5.0f,P},{0.35f,4.65f,P},{0.0f,4.0f,P},{0.35f,3.35f,E}
};
point lEXCLIMATION[] = {
            {1.5f,5.0f,P},{1.5f,1.5f,L},
            {1.35f,0.35f,P},{1.35f,0.65f,P},{1.65f,0.65f,P},{1.65f,0.35f,P},{1.35f,0.35f,E}
};

point lMINUS[] = {
	    {0.0f, 2.5f,P}, {3.0f, 2.5f,E}
};

void drawLetter(point *C)
{
    glBegin(GL_LINE_STRIP);
    for(;;C++) {
        switch(C->type) {
            case P:
                glVertex2fv(&C->x);
                break;
            case L:
                glVertex2fv(&C->x);
                glEnd();
                glBegin(GL_LINE_STRIP);
                break;
            case E:
                glVertex2fv(&C->x);
                glEnd();
                glTranslatef(3.75,0.0,0.0);
                return;
        }
    }
}

void FontInit()
{
    GLuint base;

    base = glGenLists(256);
    glListBase(base);
    glNewList(base+'A', GL_COMPILE); drawLetter(lA);
    glEndList();
    glNewList(base+'B', GL_COMPILE); drawLetter(lB);
    glEndList();
    glNewList(base+'C', GL_COMPILE); drawLetter(lC);
    glEndList();
    glNewList(base+'D', GL_COMPILE); drawLetter(lD);
    glEndList();
    glNewList(base+'E', GL_COMPILE); drawLetter(lE);
    glEndList();
    glNewList(base+'F', GL_COMPILE); drawLetter(lF);
    glEndList();
    glNewList(base+'G', GL_COMPILE); drawLetter(lG);
    glEndList();
    glNewList(base+'H', GL_COMPILE); drawLetter(lH);
    glEndList();
    glNewList(base+'I', GL_COMPILE); drawLetter(lI);
    glEndList();
    glNewList(base+'J', GL_COMPILE); drawLetter(lJ);
    glEndList();
    glNewList(base+'K', GL_COMPILE); drawLetter(lK);
    glEndList();
    glNewList(base+'L', GL_COMPILE); drawLetter(lL);
    glEndList();
    glNewList(base+'M', GL_COMPILE); drawLetter(lM);
    glEndList();
    glNewList(base+'N', GL_COMPILE); drawLetter(lN);
    glEndList();
    glNewList(base+'O', GL_COMPILE); drawLetter(lO);
    glEndList();
    glNewList(base+'P', GL_COMPILE); drawLetter(lP);
    glEndList();
    glNewList(base+'Q', GL_COMPILE); drawLetter(lQ);
    glEndList();
    glNewList(base+'R', GL_COMPILE); drawLetter(lR);
    glEndList();
    glNewList(base+'S', GL_COMPILE); drawLetter(lS);
    glEndList();
    glNewList(base+'T', GL_COMPILE); drawLetter(lT);
    glEndList();
    glNewList(base+'U', GL_COMPILE); drawLetter(lU);
    glEndList();
    glNewList(base+'V', GL_COMPILE); drawLetter(lV);
    glEndList();
    glNewList(base+'W', GL_COMPILE); drawLetter(lW);
    glEndList();
    glNewList(base+'X', GL_COMPILE); drawLetter(lX);
    glEndList();
    glNewList(base+'Y', GL_COMPILE); drawLetter(lY);
    glEndList();
    glNewList(base+'Z', GL_COMPILE); drawLetter(lZ);
    glEndList();
    glNewList(base+'0', GL_COMPILE); drawLetter(l0);
    glEndList();
    glNewList(base+'1', GL_COMPILE); drawLetter(l1);
    glEndList();
    glNewList(base+'2', GL_COMPILE); drawLetter(l2);
    glEndList();
    glNewList(base+'3', GL_COMPILE); drawLetter(l3);
    glEndList();
    glNewList(base+'4', GL_COMPILE); drawLetter(l4);
    glEndList();
    glNewList(base+'5', GL_COMPILE); drawLetter(l5);
    glEndList();
    glNewList(base+'6', GL_COMPILE); drawLetter(l6);
    glEndList();
    glNewList(base+'7', GL_COMPILE); drawLetter(l7);
    glEndList();
    glNewList(base+'8', GL_COMPILE); drawLetter(l8);
    glEndList();
    glNewList(base+'9', GL_COMPILE); drawLetter(l9);
    glEndList();
    glNewList(base+'.', GL_COMPILE); drawLetter(lPERIOD);
    glEndList();
    glNewList(base+'?', GL_COMPILE); drawLetter(lQUESTION);
    glEndList();
    glNewList(base+'!', GL_COMPILE); drawLetter(lEXCLIMATION);
    glEndList();
    glNewList(base+'-', GL_COMPILE); drawLetter(lMINUS);
    glEndList();

    glNewList(base+' ',GL_COMPILE);  glTranslatef(2.0,0.0,0.0);
    glEndList();
}

void printGLSTRING( char *s, float x, float y ,float z, float pnt)
{
  float fpnt = pnt/10.0f;
    glPushMatrix();
        glTranslatef(x,y,z);
        glScalef(fpnt,fpnt,0.0f);
        GLsizei len = strlen(s);
        glCallLists(len, GL_BYTE, (GLbyte *) s);
    glPopMatrix();
}
