/*
 * Todo:
 * - Use Quaterion orientation
 * - Make a pure base class.  Do we use virtual somewhere?
 */

#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <glm/glm.hpp>

struct EntityState {
    glm::vec3 position;
    glm::vec3 orientation;

    EntityState( float px, float py, float pz, float ox, float oy, float oz )
        : position ( glm::vec3( px, py, pz)), orientation( glm::vec3(ox, oy, oz)) {}

    EntityState( glm::vec3 pos , glm::vec3 orien): position (pos), orientation(orien) {};

    EntityState( )
        : position (glm::vec3(0.0f, 0.0f, 0.0f)), orientation(glm::vec3(0.0f, 0.0f, 0.0f)) {};

    void setPosition (glm::vec3 pos)
    {
        position = pos;
    }
    void setOrientation (glm::vec3 orien)
    {
        orientation = orien;
    }
    glm::vec3 getPosition () const
    {
        return this->position;
    }
    glm::vec3 getOrientation () const
    {
        return this->orientation;
    }
    void move( glm::vec3 pos)
    {
        this->position += pos;
    }
    void orient (glm::vec3 orien)
    {
        this->orient(orien);
    }
};

class Entity {
  public:
    EntityState state;

    Entity( float px, float py, float pz, float ox, float oy, float oz ) {
        this->state.setPosition(glm::vec3(px, py, pz));
        this->state.setOrientation(glm::vec3(ox, oy, oz));
    }
    Entity ( glm::vec3 pos, glm::vec3 orien)
    {
        this->state.setPosition(pos);
        this->state.setOrientation(orien);
    }

    Entity( ) {}
};

#endif
