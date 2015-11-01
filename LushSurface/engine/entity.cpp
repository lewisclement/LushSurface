#include "entity.hpp"

Entity::Entity(unsigned long ID) {
    this->ID = ID;
    location = new glm::vec3();
}


Entity::~Entity() {

}

glm::vec3 * Entity::getLocation() {
    return location;
}
