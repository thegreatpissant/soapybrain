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
        : position ( glm::vec3( px, py, pz)), orientation( glm::vec3(ox, oy, oz)) {};

    EntityState( glm::vec3 pos , glm::vec3 orien): position (pos), orientation(orien) {};

    EntityState( )
        : position (glm::vec3(0.0f, 0.0f, 0.0f)), orientation(glm::vec3(0.0f, 0.0f, 0.0f)) {};

    void setPosition (glm::vec3 pos);
    void setOrientation (glm::vec3 orien);
    glm::vec3 getPosition () const;
    glm::vec3 getOrientation () const;
    void move( glm::vec3 pos);
    void orient (glm::vec3 orien);
};

class Entity {
public:
    EntityState state;

    Entity( float px, float py, float pz, float ox, float oy, float oz );
    Entity ( glm::vec3 pos, glm::vec3 orien);
    Entity( );

    virtual void Update ();
};

#endif
