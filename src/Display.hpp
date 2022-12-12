#ifndef __DISPLAY_H__
#define __DISPLAY_H__

//  STDLIB
#include <vector>
#include <memory>
using std::vector;
using std::shared_ptr;

//	Engine Includes
#include "Camera.hpp"

//  Third Part
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Display {
private:
    //	Set
    int screen_width;
    int screen_height;

    //	Calculated
    glm::mat4 Perspective;

    void CalculateReshape();

  public:
    Display( );
    void init ();
    void Reshape ( int newWidth, int newHeight);

    int getWidth () const;
    int getHeight () const;
    glm::mat4 getPerspective () const;
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

* Oculus Rift
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
