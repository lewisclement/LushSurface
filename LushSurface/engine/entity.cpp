#include "entity.hpp"

Entity::Entity(unsigned long ID) {
    this->ID = ID;
    location = new glm::vec3();
}


Entity::~Entity() {

}

void Entity::setLocation(glm::vec3 Location) {
    location->x = Location.x;
    location->y = Location.y;
    location->z = Location.z;
}

glm::vec3 * Entity::getLocation() {
    return location;
}
