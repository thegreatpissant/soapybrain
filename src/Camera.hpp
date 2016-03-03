#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Entity.hpp"

#include <glm/glm.hpp>

class Camera : public Entity {
  public:
    Camera( ) {};

    Camera( float px, float py, float pz, float ox, float oy, float oz )
        : Entity( px, py, pz, ox, oy, oz ) {};

    Camera (glm::vec3 position, glm::vec3 orien)
        : Entity( position, orien) {};
};
#endif
