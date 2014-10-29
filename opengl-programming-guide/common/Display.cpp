#include "Display.h"

#include <GL/gl.h>

Display::Display()  : screen_width{ 640 }, screen_height{ 480 }
{
    CalculateReshape();
    mRenderer = shared_ptr<Renderer> {new Renderer()};
    init();
}

void Display::Reshape (int newWidth, int newHeight)
{
    screen_width = newWidth;
    screen_height = newHeight;
    CalculateReshape();
}

glm::mat4 Display::getPerspective() const
{
    return Perspective;
}
void Display::CalculateReshape()
{
//    glViewport( 0, 0, screen_width, screen_height );
    Perspective = glm::perspective( 75.0f, (float)screen_width/screen_height, 0.1f, 100.0f);
}

shared_ptr<Camera> Display::setCamera(shared_ptr<Camera> mc)
{
    shared_ptr<Camera> oc = mCamera;
    mCamera = mc;
    return oc;
}

shared_ptr<Renderer> Display::setRenderer(shared_ptr<Renderer> mr)
{
    shared_ptr<Renderer> orr = mRenderer;
    mRenderer = mr;
    return orr;
}

shared_ptr<Renderer> Display::getRenderer()
{
    return mRenderer;
}

void Display::init()
{
   mRenderer->init();
}

int Display::getHeight() const
{
    return screen_height;
}

int Display::getWidth() const
{
    return screen_width;
}

void Display::Render(vector<shared_ptr<Actor> > scenegraph)
{
    //  Set render state
    static const GLfloat one = 1.0f;
    static const glm::vec3 clear_color = glm::vec3(0.3, 0.3, 0.3);

    glClearBufferfv (GL_COLOR, 0, &clear_color[0]);
    glClearBufferfv (GL_DEPTH, 0, &one);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    //  Render Objects
    mRenderer->Render(scenegraph);

    //  Cleanup
    glBindVertexArray( 0 );
}
