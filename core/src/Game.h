#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "Renderer.h"
#include "UI.h"
#include "Attractor.h"

class Game {
public:
    Game();
    ~Game();

    bool initialize();
    void run();

private:
    void handleEvents();
    void update();
    void render();

    SDL_Window* window;
    SDL_GLContext glContext;
    bool isRunning;
    Renderer renderer;
    UI ui;
    Attractor attractor;
};

#endif // GAME_H
