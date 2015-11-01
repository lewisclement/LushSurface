#ifndef ENTITY_H
#define ENTITY_H

#include "../pch.hpp"

class Entity {
public:
    Entity(unsigned long ID);
    ~Entity();

    glm::vec3 * getLocation();

private:
    unsigned long ID = 0;

protected:
    glm::vec3 *location;
};

#endif
