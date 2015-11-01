#include "view.hpp"

View::View() {

}


View::~View() {

}

glm::vec3 View::getCameraPos() {
    return cameraPos;
}

glm::vec3 View::getCameraFront() {
    return cameraFront;
}

glm::mat4 View::getViewMatrix() {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
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
