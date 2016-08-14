#ifndef VIEW_H
#define VIEW_H

#include "../pch.hpp"

class View {
public:
    enum Projection : int {strategic = 0, birdseye};

    View();
    ~View();

    void setFocusPoint(glm::vec3 *point);
    void setProjection(Projection p);
    void setYaw(GLfloat Yaw);
    void setPitch(GLfloat Pitch);
    void setCameraFront(glm::vec3 CameraFront);

    glm::vec3 getFocusPoint();

    glm::vec3 getCameraPos();
    glm::vec3 getCameraFront();

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    GLfloat getYaw();
    GLfloat getPitch();

    Projection getProjectionType();

    void moveCamera(short direction, GLfloat distance);
    void mouseInput(GLint relX, GLint relY);

private:
    glm::vec3 *focus;

    glm::vec3 cameraPos   = glm::vec3(0.0f, 30.0f,  0.0f);
    glm::vec3 cameraFront = glm::vec3(3.0f, -3.0f, 3.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

    glm::mat4 view;
    glm::mat4 projection;

    GLfloat yaw = -90.0f, pitch = 0.0f;

    Projection currentProjection;


    void updateCameraPos();
};

#endif
