#include "Entity.h"

void Entity::setPosition (glm::vec3 pos)
{
    position = pos;
}
void Entity::setOrientation (glm::vec3 orien)
{
    orientation = orien;
}
glm::vec3 Entity::getPosition () const
{
    return this->position;
}
glm::vec3 Entity::getOrientation () const
{
    return this->orientation;
}
void Entity::move( glm::vec3 pos)
{
    this->position += pos;
}
void Entity::orient (glm::vec3 orien)
{
    this->orientation += orien;
}

Entity::Entity( float px, float py, float pz, float ox, float oy, float oz ) {
    setPosition(glm::vec3(px, py, pz));
    setOrientation(glm::vec3(ox, oy, oz));
}
Entity::Entity ( glm::vec3 pos, glm::vec3 orien)
{
    setPosition(pos);
    setOrientation(orien);
}

Entity::Entity( )
{

}

void Entity::Update()
{

}
