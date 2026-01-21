#include "UI.h"
#include <imgui.h>

UI::UI() {}

UI::~UI() {}

void UI::initialize() {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Setup style
    ImGui::StyleColorsDark();
}

void UI::handleEvent(const SDL_Event& event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
}

void UI::update(const Attractor& attractor) {
    ImGui::NewFrame();
    renderMenu(attractor);
    ImGui::Render();
}

void UI::renderMenu(const Attractor& attractor) {
    ImGui::Begin("Attracteur Étrange");
    // Parameters UI
    ImGui::SliderFloat("Parameter A", &attractor.parameterA, 0.0f, 10.0f);
    ImGui::SliderFloat("Parameter B", &attractor.parameterB, 0.0f, 10.0f);
    ImGui::SliderFloat("Speed", &attractor.speed, 0.1f, 10.0f);
    ImGui::End();
}

void UI::render() {
    ImGui::Render();
}
/**
 * UIController.cpp
 * 
 * Contient l'implémentation de la classe UI pour gérer l'interface utilisateur
 * avec ImGui.
 */