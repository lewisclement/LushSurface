#ifndef PLAYER_H
#define PLAYER_H

#include "../../pch.hpp"
#include "../entity.hpp"
#include "../world.hpp"

class Player : public Entity {
public:
    explicit Player(unsigned long ID, World *TempWorld) : Entity(ID) {tempWorld = TempWorld;} ////TempWorld is temporary
    ~Player();

    void keyInput(SDL_KeyboardEvent key);
    void processInput(GLuint deltaTime);

private:
    World *tempWorld; ////Temporary
};

#endif // PLAYER_H
