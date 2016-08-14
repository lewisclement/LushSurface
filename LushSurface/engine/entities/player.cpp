#include "player.hpp"

static bool up = false, down = false, right = false, left = false, lower = false, higher = false, jump = false;

Player::~Player() {
    delete location;
}

void Player::activate() {
    rigidBody->activate();
}

void Player::processInput(GLuint deltaTime) {
    GLfloat speed = 0.05f * deltaTime;
    GLfloat maxSpeed = 10.0f;
    double degrees = 45 * (M_PI / 180);

    btVector3 velocity = rigidBody->getLinearVelocity();
    glm::vec3 newVelocity(velocity.getX(), 0, velocity.getZ());

    if(down && left) {
        newVelocity.z -= speed;
    }
    else if(down && right) {
        newVelocity.x -= speed;
    }
    else if(up && left) {
        newVelocity.x += speed;
    }
    else if(up && right) {
        newVelocity.z += speed;
    }
    else if(up) {
        newVelocity.z += speed * std::sin(degrees);
        newVelocity.x += speed * std::cos(degrees);
    }
    else if(down) {
        newVelocity.z -= speed * std::sin(degrees);
        newVelocity.x -= speed * std::cos(degrees);
    }
    else if(right) {
        newVelocity.z += speed * std::sin(degrees);
        newVelocity.x -= speed * std::cos(degrees);
    }
    else if(left) {
        newVelocity.z -= speed * std::sin(degrees);
        newVelocity.x += speed * std::cos(degrees);
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

    rigidBody->setLinearVelocity(btVector3(newVelocity.x, newVelocity.y, newVelocity.z));

    if(jump) {
        //if(newVelocity.y < 0.05f) newVelocity.y -= 10.0f;
        //rigidBody->applyCentralImpulse(btVector3(0,-1,0));
        btVector3 relativeForce = btVector3(0,100,0);
        btMatrix3x3& boxRot = rigidBody->getWorldTransform().getBasis();
        btVector3 correctedForce = boxRot * relativeForce;
        rigidBody->applyCentralForce(correctedForce);
    }
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
