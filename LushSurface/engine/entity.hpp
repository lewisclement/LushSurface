#ifndef ENTITY_H
#define ENTITY_H

#include "../pch.hpp"

class Entity {
public:
    Entity(unsigned long ID);
    ~Entity();

    void setLocation(glm::vec3 Location);
    void actualize();

    btRigidBody* getRigidBody();

    glm::vec3 * getLocation();

private:
    unsigned long ID = 0;
    btCollisionShape* shape;

protected:
    glm::vec3 *location;
    btRigidBody* rigidBody;
};

#endif
