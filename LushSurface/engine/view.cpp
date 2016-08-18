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
    return cameraPos;
}

glm::vec3 View::getCameraFront() {
    return cameraFront;
}

glm::mat4 View::getViewMatrix() {
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

void View::updateCameraPos(GLuint deltaTime) {
    if(focus != NULL) {
        cameraPos = *focus;

        if(currentProjection == strategic) {
            if(cameraFront != cameraTurn) {
                updateTick += deltaTime;
                if(updateTick > VIEWUPDATETICK) {
                    cameraFront.x = (cameraFront.x + cameraTurn.x) * 0.5f;
                    cameraFront.z = (cameraFront.z + cameraTurn.z) * 0.5f;
                    updateTick -= VIEWUPDATETICK;
                }
            }

            cameraPos.y += 10.0f;
            cameraPos.x -= cameraFront.x * 3;
            cameraPos.z -= cameraFront.z * 3;
        } else {
            cameraPos.x += cameraFront.x * 0.2;
            cameraPos.z += cameraFront.z * 0.2;
        }
    } else return;
}

void View::updateCameraFront() {
    glm::vec3 front;
    front.x = float(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    front.y = float(sin(glm::radians(pitch)));
    front.z = float(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    setCameraFront(glm::normalize(front));
}

void View::mouseInput(GLint relX, GLint relY) {
    if(currentProjection == strategic) return;

    yaw += relX / 10.0f;
    pitch += -relY / 10.0f;

    if(pitch > 89.0f) pitch = 89.0f;
    else if(pitch < -89.0f) pitch = -89.0f;

    updateCameraFront();
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
    double amount = std::max(viewportWidth, viewportHeight) / ORTHORATIO;
    float width = viewportWidth / amount;
    float height = viewportHeight / amount;
    projection = glm::ortho(-width, width, -height, height, -10.0f, 100.0f);

    cameraFront = cameraTurn = glm::vec3(3.0f, -3.0f, 3.0f);
}

void View::turnLeft() {
    if(!(currentProjection == strategic)) return;

    if(cameraFront.x > 0 && cameraFront.z > 0) {
        cameraTurn.x = -3.0f;
        cameraTurn.z = 3.0f;
    } else if(cameraFront.x < 0 && cameraFront.z > 0) {
        cameraTurn.x = -3.0f;
        cameraTurn.z = -3.0f;
    } else if(cameraFront.x < 0 && cameraFront.z < 0) {
        cameraTurn.x = 3.0f;
        cameraTurn.z = -3.0f;
    } else {
        cameraTurn.x = 3.0f;
        cameraTurn.z = 3.0f;
    }
}

void View::turnRight() {
    if(!(currentProjection == strategic)) return;

    if(cameraFront.x > 0 && cameraFront.z > 0) {
        cameraTurn.x = 3.0f;
        cameraTurn.z = -3.0f;
    } else if(cameraFront.x > 0 && cameraFront.z < 0) {
        cameraTurn.x = -3.0f;
        cameraTurn.z = -3.0f;
    } else if(cameraFront.x < 0 && cameraFront.z < 0) {
        cameraTurn.x = -3.0f;
        cameraTurn.z = 3.0f;
    } else {
        cameraTurn.x = 3.0f;
        cameraTurn.z = 3.0f;
    }
}

void View::setBirdseye() {
    projection = glm::perspective(45.0f, (GLfloat)viewportWidth / viewportHeight, 0.1f, 1000.0f);
}

glm::vec3 View::getFocusPoint() {
    return glm::vec3(focus->x, focus->y, focus->z);
}
