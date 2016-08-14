#include "view.hpp"

View::~View() {
    focus = NULL;
}

void View::setFocusPoint(glm::vec3 *point) {
    focus = point;
}

void View::setProjection(Projection p) {
    if(p == strategic) {
        setOrtho();
        currentProjection = strategic;
    } else if(p == birdseye) {
        setBirdseye();
        currentProjection = birdseye;
    }
}

void View::setYaw(GLfloat Yaw) {
    yaw = Yaw;
}

void View::setPitch(GLfloat Pitch) {
    pitch = Pitch;
}

void View::setCameraFront(glm::vec3 CameraFront) {
    cameraFront.x = CameraFront.x;
    cameraFront.y = CameraFront.y;
    cameraFront.z = CameraFront.z;
}

void View::setViewport(uint16_t ViewportWidth, uint16_t ViewportHeight, uint16_t ViewportX, uint16_t ViewportY) {
    viewportWidth = ViewportWidth;
    viewportHeight = ViewportHeight;
    viewportX = ViewportX;
    viewportY = ViewportY;

    if(currentProjection == strategic) {
        setOrtho();
    } else if(currentProjection == birdseye) {
        setBirdseye();
    }
}

glm::vec3 View::getCameraPos() {
    updateCameraPos();
    return cameraPos;
}

glm::vec3 View::getCameraFront() {
    return cameraFront;
}

glm::mat4 View::getViewMatrix() {
    updateCameraPos();
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 View::getProjectionMatrix() {
    return projection;
}

GLfloat View::getYaw() {
    return yaw;
}

GLfloat View::getPitch() {
    return pitch;
}

View::Projection View::getProjectionType() {
    return currentProjection;
}

void View::moveCamera(short direction, GLfloat distance) {
    if(direction == 0) {
        cameraPos -= distance * glm::vec3(1.0f, 0.0f, 1.0f);
        //cameraPos -= cameraSpeed * cameraFront;
    }
    if(direction == 1) {
        cameraPos += distance * glm::vec3(1.0f, 0.0f, 1.0f);
        //cameraPos += cameraSpeed * cameraFront;
    }
    if(direction == 2) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * distance;
    }
    if(direction == 3) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * distance;
    }
}

void View::updateCameraPos() {
    if(focus != NULL) {
        cameraPos = *focus;
        cameraPos.y += 10.0f;
        cameraPos.x -= 10.0f;
        cameraPos.z -= 10.0f;
    } else return;
}

void View::mouseInput(GLint relX, GLint relY) {
    yaw += relX / 10.0f;
    pitch += -relY / 10.0f;

    if(pitch > 89.0f) pitch = 89.0f;
    else if(pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = float(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    front.y = float(sin(glm::radians(pitch)));
    front.z = float(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    setCameraFront(glm::normalize(front));
}

uint16_t View::getViewPortX() {
    return viewportX;
}

uint16_t View::getViewPortY() {
    return viewportY;
}

uint16_t View::getViewPortWidth() {
    return viewportWidth;
}

uint16_t View::getViewPortHeight() {
    return viewportHeight;
}

void View::setOrtho() {
    double amount = std::max(viewportWidth, viewportHeight) / orthoRatio;
    float width = viewportWidth / amount;
    float height = viewportHeight / amount;
    projection = glm::ortho(-width, width, -height, height, 0.1f, 100.0f);
}

void View::setBirdseye() {
    projection = glm::perspective(45.0f, (GLfloat)viewportWidth / viewportHeight, 0.1f, 1000.0f);
}
