#ifndef ENTITY_H
#define ENTITY_H

#include "../pch.hpp"

class Entity {
public:
    Entity(unsigned long ID);
    ~Entity();

private:
    unsigned long ID = 0;
    glm::dvec3 location;
};

#endif
