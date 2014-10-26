#include "Entity.h"

void EntityState::setPosition (glm::vec3 pos)
{
    position = pos;
}
void EntityState::setOrientation (glm::vec3 orien)
{
    orientation = orien;
}
glm::vec3 EntityState::getPosition () const
{
    return this->position;
}
glm::vec3 EntityState::getOrientation () const
{
    return this->orientation;
}
void EntityState::move( glm::vec3 pos)
{
    this->position += pos;
}
void EntityState::orient (glm::vec3 orien)
{
    this->orient(orien);
}

Entity::Entity( float px, float py, float pz, float ox, float oy, float oz ) {
    this->state.setPosition(glm::vec3(px, py, pz));
    this->state.setOrientation(glm::vec3(ox, oy, oz));
}
Entity::Entity ( glm::vec3 pos, glm::vec3 orien)
{
    this->state.setPosition(pos);
    this->state.setOrientation(orien);
}

Entity::Entity( )
{

}

void Entity::Update()
{

}
