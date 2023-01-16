#include "Entity.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

Entity::~Entity () {
}

void Entity::setPosition (glm::vec3 pos)
{
    this->_translation = pos;
}

void Entity::setOrientation (glm::vec3 angles)
{
    this->_rotation = angles;
}

std::ostream& operator<<(std::ostream& os, const Entity& ent)
{
    os << "Position: " << glm::to_string(ent._translation) << std::endl;
    os << "Rotation: " << glm::to_string(ent._rotation) << std::endl;
    return os;
}

glm::mat4 Entity::getTransform()
{
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), this->_translation);
    glm::mat4 rotationX = glm::rotate(translation, this->_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationY = glm::rotate(rotationX, this->_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    return glm::rotate(rotationY, this->_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
}
void Entity::move(glm::vec3 offset)
{
    this->_translation += offset;
}

void Entity::relativeMove(glm::vec3 offset)
{
    glm::mat4 transform = getTransform();
    glm::mat4 relative = glm::translate(glm::mat4(1.0f), offset);
    relative = glm::rotate(relative, this->_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    relative = glm::rotate(relative, this->_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    relative = glm::rotate(relative, this->_rotation.z, glm::vec3(1.0f, 0.0f, 1.0f));
    this->_translation = (transform * relative)[3];
}

void Entity::rotate(glm::vec3 angles)
{
    this->_rotation += angles;
}

Entity::Entity ( glm::vec3 pos, glm::vec3 orientation)
{
    Entity();
    setPosition(pos);
    setOrientation(orientation);
}

Entity::Entity( )
{
    this->_scale = glm::mat4(1.0f);
    this->_rotation = glm::vec3(1.0f);
    this->_translation = glm::vec3(0.0f);
}

void Entity::Update()
{

}
