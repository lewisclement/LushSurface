#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "../pch.hpp"
#include "shader.hpp"
#include "world.hpp"
#include "view.hpp"

#pragma clang diagnostic ignored "-Wpadded"

#define MIN_WIDTH 1280
#define MIN_HEIGHT 720

class RenderEngine{
public:
    bool initialize(GLuint width = MIN_WIDTH, GLuint height = MIN_HEIGHT);

    void render(GLuint deltaTime = 0, GLuint ticks = 0);
    void mouseInput(GLint relX, GLint relY);
    void keyInput(SDL_KeyboardEvent key);
    void processInput(GLuint deltaTime);

    RenderEngine();
    ~RenderEngine();

    SDL_Keysym lastKey;

private:
    bool initializeGL();
    bool initializeScene();

    SDL_Window* screen;
    SDL_GLContext glcontext;

    Shader *lightingShader, *lampShader;
    World *world;

    GLfloat yaw = -90.0f, pitch = 0.0f;

    GLuint windowWidth = MIN_WIDTH;
    GLuint windowHeight = MIN_HEIGHT;
    GLuint* textures;

    GLuint VAO, lightVAO; //VAO's
    GLuint VBO; //VBO's

    glm::mat4 trans;
    glm::mat4 model;
    glm::mat4 projection;

    View *view;
};

#endif // RENDERENGINE_H
