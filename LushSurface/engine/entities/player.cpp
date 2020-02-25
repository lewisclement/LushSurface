#include "player.hpp"

static bool up = false, down = false, right = false, left = false, lower = false, higher = false, jump = false, collision = false;

Player::~Player() {
    delete location;
}

void Player::activate() {
    rigidBody->activate();
}

void Player::processInput(GLuint deltaTime, glm::vec3 cameraFront) {
    GLfloat speed = 0.05f * deltaTime;
    GLfloat maxSpeed = 10.0f;

    btVector3 velocity = rigidBody->getLinearVelocity();
    glm::vec3 newVelocity(velocity.getX(), 0, velocity.getZ());

    double degrees = glm::radians(45.0);

    if(down && left) {
        if(cameraFront.z > 0 && cameraFront.x > 0)
            newVelocity.z -= speed;
        else if(cameraFront.z > 0 && cameraFront.x < 0)
            newVelocity.x += speed;
        else if(cameraFront.z < 0 && cameraFront.x < 0)
            newVelocity.z += speed;
        else if(cameraFront.z < 0 && cameraFront.x > 0)
            newVelocity.x -= speed;
    }
    else if(down && right) {
        if(cameraFront.z > 0 && cameraFront.x > 0)
            newVelocity.x -= speed;
        else if(cameraFront.z > 0 && cameraFront.x < 0)
            newVelocity.z -= speed;
        else if(cameraFront.z < 0 && cameraFront.x < 0)
            newVelocity.x += speed;
        else if(cameraFront.z < 0 && cameraFront.x > 0)
            newVelocity.z += speed;
    }
    else if(up && left) {
        if(cameraFront.z > 0 && cameraFront.x > 0)
            newVelocity.x += speed;
        else if(cameraFront.z > 0 && cameraFront.x < 0)
            newVelocity.z += speed;
        else if(cameraFront.z < 0 && cameraFront.x < 0)
            newVelocity.x -= speed;
        else if(cameraFront.z < 0 && cameraFront.x > 0)
            newVelocity.z -= speed;
    }
    else if(up && right) {
        if(cameraFront.z > 0 && cameraFront.x > 0)
            newVelocity.z += speed;
        else if(cameraFront.z > 0 && cameraFront.x < 0)
            newVelocity.x -= speed;
        else if(cameraFront.z < 0 && cameraFront.x < 0)
            newVelocity.z -= speed;
        else if(cameraFront.z < 0 && cameraFront.x > 0)
            newVelocity.x += speed;
    }
    else if(up) {
        if(cameraFront.z > 0)
            newVelocity.z += speed * std::sin(degrees);
        else
            newVelocity.z -= speed * std::sin(degrees);

        if(cameraFront.x > 0)
            newVelocity.x += speed * std::cos(degrees);
        else
            newVelocity.x -= speed * std::cos(degrees);
    }
    else if(down) {
        if(cameraFront.z > 0)
            newVelocity.z -= speed * std::sin(degrees);
        else
            newVelocity.z += speed * std::sin(degrees);

        if(cameraFront.x > 0)
            newVelocity.x -= speed * std::cos(degrees);
        else
            newVelocity.x += speed * std::cos(degrees);
    }
    else if(right) {
        if((cameraFront.z < 0 && cameraFront.x > 0) || (cameraFront.z > 0 && cameraFront.x < 0)) {
            if(cameraFront.z > 0) {
                newVelocity.z -= speed * std::sin(degrees);
                newVelocity.x -= speed * std::cos(degrees);
            } else {
                newVelocity.z += speed * std::sin(degrees);
                newVelocity.x += speed * std::cos(degrees);
            }
        }

        if((cameraFront.z > 0 && cameraFront.x > 0) || (cameraFront.z < 0 && cameraFront.x < 0)) {
            if(cameraFront.z > 0) {
                newVelocity.z += speed * std::sin(degrees);
                newVelocity.x -= speed * std::cos(degrees);
            } else {
                newVelocity.z -= speed * std::sin(degrees);
                newVelocity.x += speed * std::cos(degrees);
            }
        }
    }
    else if(left) {
        if((cameraFront.z < 0 && cameraFront.x > 0) || (cameraFront.z > 0 && cameraFront.x < 0)) {
            if(cameraFront.z > 0) {
                newVelocity.z += speed * std::sin(degrees);
                newVelocity.x += speed * std::cos(degrees);
            } else {
                newVelocity.z -= speed * std::sin(degrees);
                newVelocity.x -= speed * std::cos(degrees);
            }
        }

        if((cameraFront.z > 0 && cameraFront.x > 0) || (cameraFront.z < 0 && cameraFront.x < 0)) {
            if(cameraFront.z > 0) {
                newVelocity.z -= speed * std::sin(degrees);
                newVelocity.x += speed * std::cos(degrees);
            } else {
                newVelocity.z += speed * std::sin(degrees);
                newVelocity.x -= speed * std::cos(degrees);
            }
        }
    }

    double length = glm::length(newVelocity);
    if(length > maxSpeed) newVelocity *= 1 - (length - maxSpeed)/length;

    newVelocity.y = velocity.getY();

    if(lower) {
        newVelocity.y -= speed;
    }
    if(higher) {
        newVelocity.y += 1.0f;
    }
    if(jump && collision) {
        if(newVelocity.y < 0.05f) newVelocity.y += 10.0f;
    }

    rigidBody->setLinearVelocity(btVector3(newVelocity.x, newVelocity.y, newVelocity.z));

    collision = false;
}

void Player::setCollision(bool collided) {
    collision = collided;
}

void Player::setMovement(short direction, bool movement) {
    if(direction == 0) down = movement;
    else if(direction == 1) up = movement;
    else if(direction == 2) right = movement;
    else if(direction == 3) left = movement;
    else if(direction == 4) lower = movement;
    else if(direction == 5) higher = movement;
    else if(direction == 6) jump = movement;

    rigidBody->activate();
}
