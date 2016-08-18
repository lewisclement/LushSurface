#include "game.hpp"

struct ContactSensorCallback : public btCollisionWorld::ContactResultCallback {
    ContactSensorCallback(Player& tgtBody) : btCollisionWorld::ContactResultCallback(), body(tgtBody) {}

    Player& body;

    virtual btScalar addSingleResult(btManifoldPoint& cp,const btCollisionObjectWrapper* obj1,int id1,int index1,const btCollisionObjectWrapper* obj2,int id2,int index2) {
        body.setCollision(true);

        return 0;
    }
};

Game::Game() {
    views.push_back(new View(settings.WINDOWWIDTH, settings.WINDOWHEIGHT));

    player = new Player(0);
    views[0]->setFocusPoint(player->getLocation());
    glm::vec3 startLocation;
    startLocation.x = player->getLocation()->x;
    startLocation.y = player->getLocation()->y + 25;
    startLocation.z = player->getLocation()->z;
    player->setLocation(startLocation);

    glm::vec3 cameraPos = views[0]->getCameraPos();
    world = new World(startLocation.x, startLocation.z);

    world->addEntity((Entity*)player);
}

Game::~Game() {
    for(int i = 0; i < views.size(); i++) delete views[i];
    views.clear();
    delete player;
    delete world;
}

bool Game::tick(GLuint deltaTime) {
    world->processPhysics(deltaTime);
    player->processInput(deltaTime, views[0]->getCameraFront());

    for(int i = 0; i < views.size(); i++)
        views[i]->updateCameraPos(deltaTime);

    glm::vec3* playerPos = player->getLocation();
    world->loadTerrain(playerPos->x, playerPos->z);

    ContactSensorCallback callback(*player);
    world->dynamicsWorld->contactTest(player->rigidBody, callback);

    return true;
}

void Game::keyInput(SDL_KeyboardEvent key) {
    SDL_Keycode k = key.keysym.sym;
    if(k == SDLK_p) {
        if(key.type == SDL_KEYDOWN) {
            View::Projection current = views[0]->getProjectionType();
            if(current == View::birdseye)
                views[0]->setProjection(View::strategic);
            else if (current == View::strategic)
                views[0]->setProjection(View::birdseye);
        }
    }
    else if(k == SDLK_o) {
        if(key.type == SDL_KEYDOWN) {
            if(views.size() == 1) {
                views[0]->setViewport(settings.WINDOWWIDTH/2, settings.WINDOWHEIGHT);
                views.push_back(new View(settings.WINDOWWIDTH/2, settings.WINDOWHEIGHT, settings.WINDOWWIDTH/2, 0));
                views[1]->setFocusPoint(player->getLocation());
                views[1]->setProjection(View::strategic);
                views[1]->mouseInput(800, 200);
            }
            else if (views.size() > 1) {
                delete views[1];
                views.pop_back();
                views[0]->setViewport(settings.WINDOWWIDTH, settings.WINDOWHEIGHT);
            }
        }
    }

    else if(k == SDLK_RIGHT) {
        if(key.type == SDL_KEYDOWN) views[0]->turnRight();
    } else if(k == SDLK_LEFT) {
        if(key.type == SDL_KEYDOWN) views[0]->turnLeft();
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
    if(relX != 0 || relY != 0) views[0]->mouseInput(relX, relY);
}

glm::vec3 Game::getPlayerPos() {
    glm::vec3* loc = player->getLocation();
    glm::vec3 returnLoc;
    returnLoc.x = loc->x;
    returnLoc.y = loc->y;
    returnLoc.z = loc->z;

    return returnLoc;
}

std::vector<View*> Game::getViews() {
    return views;
}

std::vector<Chunk*> Game::getChunks() {
    return world->getChunks();
}
