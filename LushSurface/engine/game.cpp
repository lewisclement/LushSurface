#include "game.hpp"

Game::Game() {
    view = new View();
    glm::vec3 cameraPos = view->getCameraPos();
    world = new World(cameraPos.x - chunkSize / 2, cameraPos.z + chunkSize / 2);

    player = new Player(0);
    view->setFocusPoint(player->getLocation());
    glm::vec3 startLocation;
    startLocation.x = player->getLocation()->x;
    startLocation.y = player->getLocation()->y + 25;
    startLocation.z = player->getLocation()->z;
    player->setLocation(startLocation);

    world->addEntity((Entity*)player);
}

Game::~Game() {
    delete view;
    delete player;
    delete world;
}

bool Game::tick(GLuint deltaTime) {
    world->processPhysics(deltaTime);
    player->processInput(deltaTime);

    glm::vec3 cameraPos = view->getCameraPos();
    world->loadTerrain(cameraPos.x, cameraPos.z + chunkSize / 2);

    return true;
}

void Game::keyInput(SDL_KeyboardEvent key) {
    SDL_Keycode k = key.keysym.sym;
    if(k == SDLK_p) {
        if(key.type == SDL_KEYDOWN) {
            View::Projection current = view->getProjectionType();
            if(current == View::birdseye)
                view->setProjection(View::strategic);
            else if (current == View::strategic)
                view->setProjection(View::birdseye);
        }
    }

    else if(k == SDLK_DOWN || k == SDLK_s) {
        if(key.type == SDL_KEYDOWN) {player->setMovement(0, true);} else {player->setMovement(0, false);}
    } else if(k == SDLK_UP || k == SDLK_w) {
        if(key.type == SDL_KEYDOWN) {player->setMovement(1, true);} else {player->setMovement(1, false);}
    } else if(k == SDLK_RIGHT || k == SDLK_d) {
        if(key.type == SDL_KEYDOWN) {player->setMovement(2, true);} else {player->setMovement(2, false);}
    } else if(k == SDLK_LEFT || k == SDLK_a) {
        if(key.type == SDL_KEYDOWN) {player->setMovement(3, true);} else {player->setMovement(3, false);}
    } else if(k == SDLK_z) {
        if(key.type == SDL_KEYDOWN) {player->setMovement(4, true);} else {player->setMovement(4, false);}
    } else if(k == SDLK_x) {
        if(key.type == SDL_KEYDOWN) {player->setMovement(5, true);} else {player->setMovement(5, false);}
    } else if (k == SDLK_SPACE) {
        if(key.type == SDL_KEYDOWN) {player->setMovement(6, true);} else {player->setMovement(6, false);}
    }

}

void Game::mouseInput(GLint relX, GLint relY) {
    if(relX != 0 || relY != 0) view->mouseInput(relX, relY);
}

glm::vec3 Game::getPlayerPos() {
    glm::vec3* loc = player->getLocation();
    glm::vec3 returnLoc;
    returnLoc.x = loc->x;
    returnLoc.y = loc->y;
    returnLoc.z = loc->z;

    return returnLoc;
}

View* Game::getView() {
    return view;
}

std::vector<Chunk*> Game::getChunks() {
    return world->getChunks();
}
