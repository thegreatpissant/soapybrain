#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Display {
private:
    int screen_width;
    int screen_height;
    float h_screen_size;
    float v_screen_size;
    glm::mat4 Perspective;

  public:
    Display( );
    void Reshape ( int newWidth, int newHeight);
    int getWidth () const;
    int getHeight () const;
    glm::mat4 getPerspective () const;
    void CalculateReshape();
};

#endif
