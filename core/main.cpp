#include <SDL3/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <vector>
#include <cmath>
#include <iostream>

// Structure de point pour les calculs
struct Point { float x, y, z; };

// Classe Attractor (mise à jour avec vos 14 types)
class Attractor {
public:
    int type = 1;
    Point p = {0.1f, 0.0f, 0.0f};
    float dt = 0.01f; // Pas de temps plus fin pour la stabilité

    void update() {
        float dx = 0, dy = 0, dz = 0;
        if (type == 1) { // Lorenz
            dx = (10.0f * (p.y - p.x));
            dy = (p.x * (28.0f - p.z) - p.y);
            dz = (p.x * p.y - (8.0f / 3.0f) * p.z);
        } 
        else if (type == 2) { // Rössler
            dx = (-p.y - p.z);
            dy = (p.x + 0.2f * p.y);
            dz = (0.2f + p.z * (p.x - 5.7f));
        }
        else if (type == 11) { // Chua
            float h = -0.714f * p.x + 0.5f * (-1.143f + 0.714f) * (std::abs(p.x + 1.0f) - std::abs(p.x - 1.0f));
            dx = 15.6f * (p.y - p.x - h);
            dy = p.x - p.y + p.z;
            dz = -28.0f * p.y;
        }
        else if (type == 14) { // Double Scroll
            dx = 0.7f * (p.y - p.x);
            dy = p.x - p.x * p.z + 7.0f * p.y;
            dz = p.x * p.y - 0.7f * p.z;
        }
        // ... Ajoutez ici les autres types (3 à 13) de votre fichier attracteurs.cpp original

        p.x += dx * dt; 
        p.y += dy * dt; 
        p.z += dz * dt;
    }
};

int main(int argc, char* argv[]) {
    // 1. Initialisation SDL3
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) return -1;

    SDL_Window* window = SDL_CreateWindow("ChaosSim 2026 - SDL3 & ImGui", 1280, 720, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    // 2. Initialisation ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    Attractor att;
    std::vector<SDL_FPoint> points; // SDL3 utilise des SDL_FPoint (float)
    float zoom = 15.0f;
    bool running = true;
    ImVec4 color = ImVec4(0.0f, 1.0f, 1.0f, 1.0f); // Cyan

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) running = false;
        }

        // 3. Logique de calcul (5 itérations par frame)
        for (int i = 0; i < 5; i++) {
            att.update();
            points.push_back({ 640.0f + att.p.x * zoom, 360.0f + att.p.y * zoom });
        }

        // 4. Interface ImGui
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Contrôles de l'Attracteur");
        ImGui::Text("Système actuel: %d", att.type);
        if (ImGui::SliderInt("Type", &att.type, 1, 14)) points.clear();
        ImGui::SliderFloat("Zoom", &zoom, 1.0f, 300.0f);
        ImGui::SliderFloat("Vitesse (dt)", &att.dt, 0.001f, 0.05f);
        ImGui::ColorEdit3("Couleur", (float*)&color);
        if (ImGui::Button("Réinitialiser")) points.clear();
        ImGui::End();

        // 5. Rendu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, color.x * 255, color.y * 255, color.z * 255, 255);
        if (!points.empty()) {
            SDL_RenderPoints(renderer, points.data(), points.size());
        }

        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    // Nettoyage
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
