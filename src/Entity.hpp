/*
 * Todo:
 * - Use Quaterion orientation
 * - Make a pure base class.  Do we use virtual somewhere?
 */

#ifndef __ENTITY_H__
#define __ENTITY_H__

//  StdLib
#include <iostream>

//  3rd Party
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class Entity {
public:
    glm::mat4 _scale;
    glm::vec3 _rotation;
    glm::vec3 _translation;
    
    friend std::ostream& operator<<(std::ostream& os, const Entity& ent);
    
    Entity( glm::vec3 pos , glm::vec3 orien);

    Entity( );
    void setPosition(glm::vec3 pos);
    void setOrientation(glm::vec3 angles);
    glm::mat4 getTransform();
    void move( glm::vec3 translation);
    void rotate(glm::vec3 angles);

    virtual void Update();
    ~Entity();
};

#endif
