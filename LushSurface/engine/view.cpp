#include "view.hpp"

View::View() {
    focus = NULL;
    projection = glm::ortho(0.0f, 24.0f, 0.0f, 13.5f, 0.1f, 100.0f);
    currentProjection = strategic;
}

View::~View() {
    focus = NULL;
}

void View::setFocusPoint(glm::vec3 *point) {
    focus = point;
}

void View::setProjection(Projection p) {
    if(p == strategic) {
        projection = glm::ortho(0.0f, 24.0f, 0.0f, 13.5f, 0.1f, 100.0f);
        currentProjection = strategic;
    } else if(p == birdseye) {
        projection = glm::perspective(45.0f, (GLfloat)1280 / 720, 0.1f, 1000.0f);
        currentProjection = birdseye;
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
        cameraPos.y += 30.0f;
    } else return;
}
