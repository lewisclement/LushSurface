#include "entity.hpp"

Entity::Entity(unsigned long ID) {
    this->ID = ID;
    location = new glm::vec3();

    shape = new btSphereShape(0.5f);

    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(80, 50, 80)));
    btScalar mass = 10;
    btVector3 fallInertia(0, 0, 0);
    shape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, shape, fallInertia);

    rigidBody = new btRigidBody(fallRigidBodyCI);
    rigidBody->setFriction(10);
}


Entity::~Entity() {
    delete rigidBody->getMotionState();
    delete rigidBody;
    delete shape;
}

void Entity::setLocation(glm::vec3 Location) {
    location->x = Location.x;
    location->y = Location.y;
    location->z = Location.z;
}

void Entity::actualize() {
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);
    btVector3 origin = trans.getOrigin();

    location->x = origin.getX();
    location->y = origin.getY();
    location->z = origin.getZ();
}

btRigidBody* Entity::getRigidBody() {
    return rigidBody;
}

glm::vec3 * Entity::getLocation() {
    return location;
}
