#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <SDL.h>
#include "Attractor.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void initialize();
    void clear();
    void render(const Attractor& attractor);

private:
    SDL_Color pointColor;
};

#endif // RENDERER_H
