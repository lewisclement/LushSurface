#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#include "../pch.hpp"
#include "shader.hpp"
#include "world.hpp"
#include "view.hpp"
#include "../globals.hpp"

#pragma clang diagnostic ignored "-Wpadded"

class RenderEngine{
public:
    bool initialize(GLuint width = MINRESX, GLuint height = MINRESY);

    void render(GLuint deltaTime = 0, GLuint ticks = 0);
    void keyInput(SDL_KeyboardEvent key);
    void processInput(GLuint deltaTime);


    void addChunk(Chunk* chunk);
    void addView(View* view);

    void removeView(View* view);

    void setPlayer(glm::vec3 pos);

    RenderEngine();
    ~RenderEngine();

    SDL_Keysym lastKey;

    //Temporary


private:
    bool initializeGL();
    bool initializeScene();

    void drawLampShader(View* view);
    void drawLightingShader(View* view);

    SDL_Window* screen;
    SDL_GLContext glcontext;

    Shader *lightingShader, *lampShader;

    glm::vec3 lightPositions[4];

    GLuint windowWidth = MINRESX;
    GLuint windowHeight = MINRESY;
    GLuint* textures;

    GLuint VAO, lightVAO; //VAO's
    GLuint VBO; //VBO's

    glm::mat4 trans;
    glm::mat4 model;

    std::vector<Chunk*> chunks;
    std::vector<View*> views;
};

#endif // RENDERENGINE_H
