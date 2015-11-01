#ifndef PLAYER_H
#define PLAYER_H

#include "../../pch.hpp"
#include "../entity.hpp"

class Player : public Entity {
public:
    explicit Player(unsigned long ID) : Entity(ID) {}
    ~Player();

    void keyInput(SDL_KeyboardEvent key);
    void processInput(GLuint deltaTime);

private:
};

#endif // PLAYER_H
