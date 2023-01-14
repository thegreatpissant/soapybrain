#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <vector>
#include <memory>

#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class RenderTarget {
private:
    //	Set
    int screen_width;
    int screen_height;
    float fov;

    //	Calculated
    glm::mat4 Projection;

    void CalculateReshape();

  public:
    RenderTarget( );
    void Reshape ( int newWidth, int newHeight);

    int getWidth () const;
    int getHeight () const;
    glm::mat4 getProjection () const;

    int framebuffer_id;

    void setFrameBufferID(int id);
};

#endif

/**
Display durring a render update request by game engine.
Holds State with Properties of Display Target and Render Target

* Single Monitor
- Recieve request with Scenegraph
- Query Camera for position and orientation
- Process Scenegraph for viewable objects
- Set Render Target (Direct to output)
- Set Renderer Viewport Matrix (Monitor)
- Set Renderer Camera Position Matrix
+ Render objects
    - Select shader for object
    - Select model for object
    - Execute render for object
- Execute Buffer Flip

* HMD
- Recieve request with Scenegraph
- Query Camera for position and orientation
- Query Rift for position and orientation
- Process Scenegraph for viewable objects
+ For each EYE
    - Set Render Target (EYE texture)
    - Set Renderer Viewport Matrix (EYE)
    - Set Renderer Camera Position Matrix (EYE)
    + Render objects
        - Load shader for object
        - Load model for object
        - Execute render for object
+ For each EYE
    - Load Shader
    - Set Render Target (EYE.MESH)
    - Render EYE texture to EYE.MESH
- Execute Buffer Flip

**/
