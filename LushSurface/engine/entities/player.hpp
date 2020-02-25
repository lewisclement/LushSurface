#ifndef PLAYER_H
#define PLAYER_H

#include "../../pch.hpp"
#include "../entity.hpp"
#include "../world.hpp"

class Player : public Entity {
public:
    explicit Player(unsigned long ID) : Entity(ID) {}
    ~Player();

    void activate();
    void processInput(GLuint deltaTime, glm::vec3 cameraFront);

    void setMovement(short direction, bool movement);
    void setCollision(bool collided);
};

#endif // PLAYER_H
