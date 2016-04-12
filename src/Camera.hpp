#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Entity.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera : public Entity {
private:
    glm::mat4 viewmat;

public:
    Camera( ) {};

    Camera( float px, float py, float pz, float ox, float oy, float oz )
        : Entity( px, py, pz, ox, oy, oz )
    {
        update();
    }

    Camera (glm::vec3 position, glm::vec3 orien)
        : Entity( position, orien)
    {
        update();
    }

    glm::mat4 get_viewmat()
    {
        return viewmat;
    }

    void update ()
    {
        viewmat = glm::lookAt(Entity::position,glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
};
#endif
