#ifndef VIEW_H
#define VIEW_H

#include "../pch.hpp"
#include "../globals.hpp"

class View {
public:
    enum Projection : int {strategic = 0, birdseye};

    View(uint16_t ViewportWidth, uint16_t ViewportHeight, uint16_t ViewportX = 0, uint16_t ViewportY = 0) : viewportWidth(ViewportWidth), viewportHeight(ViewportHeight), viewportX(ViewportX), viewportY(ViewportY)
    {
        focus = NULL;
        setOrtho();
        currentProjection = strategic;
    }
    ~View();

    void setFocusPoint(glm::vec3 *point);
    void setProjection(Projection p);
    void setYaw(GLfloat Yaw);
    void setPitch(GLfloat Pitch);
    void setCameraFront(glm::vec3 CameraFront);
    void setViewport(uint16_t ViewportWidth, uint16_t ViewportHeight, uint16_t ViewportX = 0, uint16_t ViewportY = 0);

    void turnLeft();
    void turnRight();

    glm::vec3 getFocusPoint();

    glm::vec3 getCameraPos();
    glm::vec3 getCameraFront();

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    GLfloat getYaw();
    GLfloat getPitch();

    uint16_t getViewPortX();
    uint16_t getViewPortY();
    uint16_t getViewPortWidth();
    uint16_t getViewPortHeight();

    Projection getProjectionType();

    void updateCameraPos(GLuint deltaTime);

    void moveCamera(short direction, GLfloat distance);
    void mouseInput(GLint relX, GLint relY);

private:
    glm::vec3 *focus;

    glm::vec3 cameraPos   = glm::vec3(0.0f, 30.0f,  0.0f);
    glm::vec3 cameraFront = glm::vec3(3.0f, -3.0f, 3.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    glm::vec3 cameraTurn  = glm::vec3(0.0f, 0.0f, 0.0f);
    GLuint updateTick = 0;

    glm::mat4 view;
    glm::mat4 projection;

    GLfloat yaw = -90.0f, pitch = 0.0f;

    Projection currentProjection;

    uint16_t viewportX, viewportY, viewportWidth, viewportHeight;

    void updateCameraFront();

    void setOrtho();
    void setBirdseye();
};

#endif
