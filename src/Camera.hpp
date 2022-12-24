#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Entity.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Camera : public Entity {
private:
    glm::mat4 viewmat;

public:
    Camera( ) : Entity( glm::vec3(1.0f), glm::vec3(1.0f))
    {

    };

    Camera (glm::vec3 position, glm::vec3 orientation )
        : Entity( position, orientation)
    {
    }

    friend std::ostream& operator<<(std::ostream& os, const Camera& ent)
    {
        os << "--Camera--" << std::endl;
        os << "_translation: " << glm::to_string(ent._translation) << std::endl;
        os << "_rotation: " << glm::to_string(ent._rotation) << std::endl;
        os << "_scale: " << glm::to_string(ent._scale) << std::endl;

        return os;
    }

};
#endif
