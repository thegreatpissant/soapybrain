#include "Display.hpp"

#include <GL/gl.h>

Display::Display()  : screen_width{ 640 }, screen_height{ 480 }, fov{75.0f}
{
    CalculateReshape();
}

void Display::Reshape (int newWidth, int newHeight)
{
    screen_width = newWidth;
    screen_height = newHeight;
    CalculateReshape();
}

glm::mat4 Display::getProjection() const
{
    return Projection;
}
void Display::CalculateReshape()
{
    Projection = glm::perspective( fov, (float)screen_width/(float)screen_height, 0.1f, 1000.0f);
}

int Display::getHeight() const
{
    return screen_height;
}

int Display::getWidth() const
{
    return screen_width;
}
