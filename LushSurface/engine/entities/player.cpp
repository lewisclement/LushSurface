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
    GLfloat cameraSpeed = 0.02f * deltaTime;

    if(down) {
        location->z -= cameraSpeed;
        location->x -= cameraSpeed;
    }
    if(up) {
        location->z += cameraSpeed;
        location->x += cameraSpeed;
    }
    if(right) {
        location->z += cameraSpeed;
        location->x -= cameraSpeed;
    }
    if(left) {
        location->z -= cameraSpeed;
        location->x += cameraSpeed;
    }
    if(lower) {
        location->y -= cameraSpeed;
    }
    if(higher) {
        location->y += cameraSpeed;
    }
}
