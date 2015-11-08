#include "pch.hpp"

#include "engine/renderengine.hpp"
#include "engine/entities/player.hpp"

static RenderEngine* renderer;
static GLulong frameCount = 0;
static GLuint frameRate = 60;

int main() {
    renderer = new RenderEngine();
    bool done = false;

    if(!renderer->initialize()) return 1;

    Player player(0);////Temporary
    renderer->view->setFocusPoint(player.getLocation());////Temporary
    glm::vec3 startLocation;
    startLocation.x = player.getLocation()->x;
    startLocation.y = player.getLocation()->y + 20;
    startLocation.z = player.getLocation()->z;
    player.setLocation(startLocation);////Temporary

    GLuint lastTime = SDL_GetTicks();
    while(!done) {
        GLuint currentTime = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    done = true;
                    break;
                }
                case SDL_KEYDOWN: {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        done = true;
                        break;
                    } else {
                        renderer->keyInput(event.key);
                        player.keyInput(event.key);
                    }
                    break;
                }
                case SDL_KEYUP: {
                    renderer->keyInput(event.key);
                    player.keyInput(event.key);
                    break;
                }
                case SDL_MOUSEMOTION: {
                    int x, y;
                    SDL_GetRelativeMouseState(&x, &y);

                    if(x != 0 || y != 0) renderer->mouseInput(x, y);
                    break;
                }
            }
        }

        if(currentTime - lastTime < 1000/frameRate) {
            continue;
        }

        player.processInput(currentTime - lastTime);
        renderer->lightPositions[0] = *player.getLocation();

        renderer->render(currentTime - lastTime, currentTime);
        frameCount++;
        lastTime = currentTime;
    }

    delete renderer;
    return 0;
}
