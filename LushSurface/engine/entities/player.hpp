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
    void processInput(GLuint deltaTime);

    void setMovement(short direction, bool movement);

private:
    World *tempWorld; ////Temporary
};

#endif // PLAYER_H
