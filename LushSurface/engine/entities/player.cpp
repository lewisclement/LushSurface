#include "player.hpp"

static bool up = false, down = false, right = false, left = false, lower = false, higher = false;

Player::~Player() {
    delete location;
}

void Player::keyInput(SDL_KeyboardEvent key) {
    SDL_Keycode k = key.keysym.sym;
    if(k == SDLK_DOWN || k == SDLK_s) {
        if(key.type == SDL_KEYDOWN) {down = true;} else {down = false;}
    } else if(k == SDLK_UP || k == SDLK_w) {
        if(key.type == SDL_KEYDOWN) {up = true;} else {up = false;}
    } else if(k == SDLK_RIGHT || k == SDLK_d) {
        if(key.type == SDL_KEYDOWN) {right = true;} else {right = false;}
    } else if(k == SDLK_LEFT || k == SDLK_a) {
        if(key.type == SDL_KEYDOWN) {left = true;} else {left = false;}
    } else if(k == SDLK_z) {
        if(key.type == SDL_KEYDOWN) {lower = true;} else {lower = false;}
    } else if(k == SDLK_x) {
        if(key.type == SDL_KEYDOWN) {higher = true;} else {higher = false;}
    }
}

void Player::processInput(GLuint deltaTime) {
    GLfloat speed = 0.002f * deltaTime;

    btVector3 velocity = rigidBody->getLinearVelocity();
    glm::vec3 newLocation(velocity.getX(), velocity.getY(), velocity.getZ());

    if(down) {
        newLocation.z -= speed;
        newLocation.x -= speed;
    }
    if(up) {
        newLocation.z += speed;
        newLocation.x += speed;
    }
    if(right) {
        newLocation.z += speed;
        newLocation.x -= speed;
    }
    if(left) {
        newLocation.z -= speed;
        newLocation.x += speed;
    }


    if(lower) {
        newLocation.y -= speed;
    }
    if(higher) {
        newLocation.y += speed * 30;
    }

    rigidBody->setLinearVelocity(btVector3(newLocation.x, newLocation.y, newLocation.z));
}
