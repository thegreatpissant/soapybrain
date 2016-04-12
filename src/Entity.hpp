/*
 * Todo:
 * - Use Quaterion orientation
 * - Make a pure base class.  Do we use virtual somewhere?
 */

#ifndef __ENTITY_H__
#define __ENTITY_H__

//  StdLib
#include <memory>
using std::shared_ptr;

//  3rd Party
#include <glm/glm.hpp>

class Entity {
public:
    glm::vec3 position;
    glm::vec3 orientation;

    Entity( float px, float py, float pz, float ox, float oy, float oz );
    Entity( glm::vec3 pos , glm::vec3 orien);

    Entity( );
    void setPosition (glm::vec3 pos);
    void setOrientation (glm::vec3 orien);
    glm::vec3 getPosition () const;
    glm::vec3 getOrientation () const;
    void move( glm::vec3 pos);
    void orient (glm::vec3 orien);

    virtual void Update ();
    ~Entity();
};

#endif
