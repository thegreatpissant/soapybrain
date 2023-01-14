#include "RenderTarget.hpp"

#include <GL/gl.h>

RenderTarget::RenderTarget()  : screen_width{ 640 }, screen_height{ 480 }, fov{75.0f}, framebuffer_id{0}
{
    CalculateReshape();
}

void RenderTarget::Reshape (int newWidth, int newHeight)
{
    screen_width = newWidth;
    screen_height = newHeight;
    CalculateReshape();
}

glm::mat4 RenderTarget::getProjection() const
{
    return Projection;
}
void RenderTarget::CalculateReshape()
{
    Projection = glm::perspective( fov, (float)screen_width/(float)screen_height, 0.1f, 1000.0f);
}

int RenderTarget::getHeight() const
{
    return screen_height;
}

int RenderTarget::getWidth() const
{
    return screen_width;
}

void RenderTarget::setFrameBufferID(int new_framebuffer_id)
{
    framebuffer_id = new_framebuffer_id;
}