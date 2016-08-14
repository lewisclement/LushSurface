#ifndef GAME_H
#define GAME_H

#include "../pch.hpp"

#include "world.hpp"
#include "view.hpp"
#include "entities/player.hpp"

class Game {
public:
    Game();
    ~Game();

    bool tick(GLuint deltaTime);

    void keyInput(SDL_KeyboardEvent key);
    void mouseInput(GLint relX, GLint relY);

    glm::vec3 getPlayerPos();
    View* getView();
    std::vector<Chunk*> getChunks();

private:
    Player* player;
    View* view;
    World *world;

};

#endif
