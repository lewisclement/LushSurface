#include "pch.hpp"

#include "engine/game.hpp"
#include "engine/renderengine.hpp"
#include "engine/view.hpp"
#include "engine/entities/player.hpp"

#include "engine/world.hpp"

static RenderEngine* renderer;
static Game* game;
static GLulong frameCount = 0;
static GLuint frameRate = 60;
Settings settings;

int main() {
    renderer = new RenderEngine();
    bool done = false;

    if(!renderer->initialize()) return 1;

    game = new Game();

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
                        game->keyInput(event.key);
                    }
                    break;
                }
                case SDL_KEYUP: {
                    game->keyInput(event.key);
                    break;
                }
                case SDL_MOUSEMOTION: {
                    int x, y;
                    SDL_GetRelativeMouseState(&x, &y);

                    game->mouseInput(x, y);
                    break;
                }
            }
        }

        if(currentTime - lastTime < 1000/frameRate) {
            continue;
        }

        GLuint deltaTime = currentTime - lastTime;
        game->tick(deltaTime);
        renderer->setPlayer(game->getPlayerPos());

        auto views = game->getViews();
        for(int i = 0; i < views.size(); i++) renderer->addView(views[i]);

        std::vector<Chunk*> chunks = game->getChunks();
        for(int i = 0; i < chunks.size(); i++) {
            renderer->addChunk(chunks[i]);
        }

        renderer->render(deltaTime, currentTime);
        frameCount++;
        lastTime = currentTime;
    }

    delete renderer;
    return 0;
}
