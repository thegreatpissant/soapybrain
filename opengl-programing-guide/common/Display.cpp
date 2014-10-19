#include "Display.h"

#include <GL/gl.h>

Display::Display()  : screen_width{ 640 }, screen_height{ 480 }
{
    CalculateReshape();
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
    glViewport( 0, 0, screen_width, screen_height );
    Perspective = glm::perspective( 75.0f, (float)screen_width/screen_height, 0.1f, 100.0f);
}

int Display::getHeight() const
{
    return screen_height;
}

int Display::getWidth() const
{
    return screen_width;
}
