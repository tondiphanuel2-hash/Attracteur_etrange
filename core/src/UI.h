#ifndef UI_H
#define UI_H

#include <imgui.h>
#include <SDL.h>
#include "Attractor.h"

class UI {
public:
    UI();
    ~UI();

    void initialize();
    void handleEvent(const SDL_Event& event);
    void update(const Attractor& attractor);
    void render();

private:
    void renderMenu(const Attractor& attractor);
};

#endif // UI_H
/**
 * UI.h
 * 
 * Contient la déclaration de la classe UI pour gérer l'interface utilisateur
 * avec ImGui.
 */