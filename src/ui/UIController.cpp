/**
 * UIController.cpp - Implémentation du contrôleur d'interface (SEMAINE 4)
 * 
 * Contient toutes les fenêtres ImGui et leur logique.
 * 
 * NOUVEAUTÉS SEMAINE 4 :
 * - Bouton Mode Présentation
 * - Bouton Capture d'écran
 * - Affichage progression présentation
 */

#include "UIController.h"
#include "../attractors/AttractorSystem.h"
#include "../attractors/LorenzAttractor.h"
#include "../attractors/RosslerAttractor.h"
#include "../attractors/ChenAttractor.h"
#include "../graphics/Camera3D.h"
#include "../graphics/ParticleSystem.h"
#include "../core/PresentationMode.h"  // NOUVEAU

#include <imgui.h>

/**
 * UIController - Constructeur
 */
UIController::UIController()
    : mState()
    , mRequestedAttractorIndex(-1)
    , mCaptureScreenshotRequested(false)  // NOUVEAU
    , mPresentationToggleRequested(false)  // NOUVEAU
{
    // Initialiser l'état par défaut
}

/**
 * Render - Dessine toute l'interface
 */
void UIController::Render(AttractorSystem* attractor, 
                          Camera3D* camera,
                          ParticleSystem* particles,
                          float deltaTime,
                          int currentAttractorIndex,
                          PresentationMode* presentation)  // NOUVEAU paramètre
{
    // Dessiner chaque fenêtre si elle est active
    
    if (mState.showInfo)
    {
        RenderInfoWindow(deltaTime);
    }
    
    if (mState.showControls)
    {
        RenderControlsWindow(currentAttractorIndex, presentation);  // NOUVEAU paramètre
    }
    
    if (mState.showParameters)
    {
        RenderParametersWindow(attractor);
    }
    
    if (mState.showStatistics)
    {
        RenderStatisticsWindow(attractor, camera, particles);
    }
}

/**
 * RenderInfoWindow - Fenêtre d'informations générales
 */
void UIController::RenderInfoWindow(float deltaTime)
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("ChaosSim - Informations", &mState.showInfo, 
                 ImGuiWindowFlags_AlwaysAutoResize);
    
    // Logo/Titre
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "ChaosSim v3.0");
    ImGui::Text("Visualiseur d'Attracteurs Etranges");
    
    ImGui::Separator();
    
    // FPS et performance
    float fps = (deltaTime > 0.0f) ? (1.0f / deltaTime) : 0.0f;
    ImGui::Text("Performance:");
    ImGui::Text("  FPS: %.1f", fps);
    ImGui::Text("  Frame Time: %.3f ms", deltaTime * 1000.0f);
    
    // Indicateur de performance visuel
    ImVec4 perfColor;
    if (fps >= 55.0f)
        perfColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);  // Vert : excellent
    else if (fps >= 30.0f)
        perfColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);  // Jaune : moyen
    else
        perfColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  // Rouge : mauvais
    
    ImGui::SameLine();
    ImGui::TextColored(perfColor, fps >= 55.0f ? "[OK]" : fps >= 30.0f ? "[WARN]" : "[BAD]");
    
    ImGui::Separator();
    
    // Aide rapide
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Raccourcis clavier:");
    ImGui::Text("  Espace: Pause/Play");
    ImGui::Text("  P: Mode Presentation");  // NOUVEAU
    ImGui::Text("  F12: Screenshot");        // NOUVEAU
    ImGui::Text("  R: Reset camera");
    ImGui::Text("  1/2/3: Changer attracteur");
    ImGui::Text("  Echap: Quitter");
    
    ImGui::End();
}

/**
 * RenderControlsWindow - Fenêtre des contrôles (SEMAINE 4 AMÉLIORÉE)
 */
void RenderControlsWindow(int currentAttractorIndex, PresentationMode* presentation)
{
    ImGui::SetNextWindowPos(ImVec2(10, 200), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Controles de Simulation", &mState.showControls,
                 ImGuiWindowFlags_AlwaysAutoResize);
    
    // === Section Attracteurs === (code existant inchangé)
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Attracteur:");
    
    if (ImGui::Button("Lorenz", ImVec2(90, 30)))
    {
        mRequestedAttractorIndex = 0;
    }
    if (currentAttractorIndex == 0)
    {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "<- Actif");
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Rossler", ImVec2(90, 30)))
    {
        mRequestedAttractorIndex = 1;
    }
    if (currentAttractorIndex == 1)
    {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "<- Actif");
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Chen", ImVec2(90, 30)))
    {
        mRequestedAttractorIndex = 2;
    }
    if (currentAttractorIndex == 2)
    {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "<- Actif");
    }
    
    ImGui::Separator();
    
    // === NOUVEAU : Section Mode Présentation ===
    if (presentation)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Mode Presentation:");
        
        if (presentation->IsActive())
        {
            // Présentation en cours
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "EN COURS");
            ImGui::Text("Phase: %s", presentation->GetPhaseName().c_str());
            
            // Barre de progression
            float progress = presentation->GetProgress();
            ImGui::ProgressBar(progress, ImVec2(-1, 0), "");
            
            // Bouton Stop
            if (ImGui::Button("⏹ Arreter Presentation", ImVec2(-1, 30)))
            {
                mPresentationToggleRequested = true;
            }
        }
        else
        {
            // Présentation inactive
            if (ImGui::Button("🎬 Demarrer Presentation (P)", ImVec2(-1, 30)))
            {
                mPresentationToggleRequested = true;
            }
            
            ImGui::TextWrapped("Demo automatique de 60 sec avec tous les attracteurs");
        }
        
        ImGui::Separator();
    }
    
    // === Section Simulation === (code existant inchangé)
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Simulation:");
    
    if (mState.isPaused)
    {
        if (ImGui::Button("▶ Play", ImVec2(140, 30)))
        {
            mState.isPaused = false;
        }
    }
    else
    {
        if (ImGui::Button("⏸ Pause", ImVec2(140, 30)))
        {
            mState.isPaused = true;
        }
    }
    
    ImGui::SameLine();
    if (ImGui::Button("↻ Reset", ImVec2(140, 30)))
    {
        // Le reset sera géré par Game.cpp
    }
    
    ImGui::Text("Vitesse:");
    ImGui::SliderFloat("##speed", &mState.simulationSpeed, 0.1f, 5.0f, "x%.1f");
    
    ImGui::Separator();
    
    // === NOUVEAU : Section Capture ===
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Capture:");
    
    if (ImGui::Button("📸 Screenshot (F12)", ImVec2(-1, 30)))
    {
        mCaptureScreenshotRequested = true;
    }
    
    ImGui::Separator();
    
    // === Section Visualisation === (code existant inchangé)
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Visualisation:");
    
    ImGui::Text("Particules:");
    if (ImGui::SliderInt("##particles", &mState.particleCount, 50, 1000))
    {
        // Le changement sera appliqué dans Game.cpp
    }
    
    ImGui::Text("Longueur trails:");
    if (ImGui::SliderInt("##trail", &mState.maxTrailLength, 100, 2000))
    {
        // Le changement sera appliqué dans Game.cpp
    }
    
    ImGui::Text("Mode couleur:");
    const char* colorModes[] = { "Velocite", "Position", "Temps", "Fixe" };
    int currentMode = static_cast<int>(mState.colorMode);
    if (ImGui::Combo("##colormode", &currentMode, colorModes, 4))
    {
        mState.colorMode = static_cast<ColorMode>(currentMode);
    }
    
    ImGui::End();
}
{
    ImGui::SetNextWindowPos(ImVec2(10, 200), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Controles de Simulation", &mState.showControls,
                 ImGuiWindowFlags_AlwaysAutoResize);
    
    // === Section Attracteurs ===
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Attracteur:");
    
    // Boutons pour chaque attracteur
    if (ImGui::Button("Lorenz", ImVec2(90, 30)))
    {
        mRequestedAttractorIndex = 0;
    }
    if (currentAttractorIndex == 0)
    {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "<- Actif");
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Rossler", ImVec2(90, 30)))
    {
        mRequestedAttractorIndex = 1;
    }
    if (currentAttractorIndex == 1)
    {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "<- Actif");
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Chen", ImVec2(90, 30)))
    {
        mRequestedAttractorIndex = 2;
    }
    if (currentAttractorIndex == 2)
    {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "<- Actif");
    }
    
    ImGui::Separator();
    
    // === Section Simulation ===
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Simulation:");
    
    // Bouton Pause/Play
    if (mState.isPaused)
    {
        if (ImGui::Button("▶ Play", ImVec2(140, 30)))
        {
            mState.isPaused = false;
        }
    }
    else
    {
        if (ImGui::Button("⏸ Pause", ImVec2(140, 30)))
        {
            mState.isPaused = true;
        }
    }
    
    ImGui::SameLine();
    if (ImGui::Button("↻ Reset", ImVec2(140, 30)))
    {
        // Le reset sera géré par Game.cpp
    }
    
    // Slider vitesse de simulation
    ImGui::Text("Vitesse:");
    ImGui::SliderFloat("##speed", &mState.simulationSpeed, 0.1f, 5.0f, "x%.1f");
    
    ImGui::Separator();
    
    // === Section Visualisation ===
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Visualisation:");
    
    // Slider nombre de particules
    ImGui::Text("Particules:");
    if (ImGui::SliderInt("##particles", &mState.particleCount, 50, 1000))
    {
        // Le changement sera appliqué dans Game.cpp
    }
    
    // Slider longueur trails
    ImGui::Text("Longueur trails:");
    if (ImGui::SliderInt("##trail", &mState.maxTrailLength, 100, 2000))
    {
        // Le changement sera appliqué dans Game.cpp
    }
    
    // Mode de coloration
    ImGui::Text("Mode couleur:");
    const char* colorModes[] = { "Velocite", "Position", "Temps", "Fixe" };
    int currentMode = static_cast<int>(mState.colorMode);
    if (ImGui::Combo("##colormode", &currentMode, colorModes, 4))
    {
        mState.colorMode = static_cast<ColorMode>(currentMode);
    }
    
    ImGui::End();
}

/**
 * RenderParametersWindow - Fenêtre des paramètres d'attracteur
 */
void UIController::RenderParametersWindow(AttractorSystem* attractor)
{
    if (!attractor)
    {
        return;
    }
    
    ImGui::SetNextWindowPos(ImVec2(10, 450), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Parametres Attracteur", &mState.showParameters,
                 ImGuiWindowFlags_AlwaysAutoResize);
    
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), 
                       "Attracteur: %s", attractor->GetName().c_str());
    
    ImGui::Separator();
    
    // Afficher des sliders différents selon l'attracteur
    std::string attractorName = attractor->GetName();
    
    if (attractorName == "Lorenz")
    {
        LorenzAttractor* lorenz = dynamic_cast<LorenzAttractor*>(attractor);
        if (lorenz)
        {
            float sigma = lorenz->GetSigma();
            float rho = lorenz->GetRho();
            float beta = lorenz->GetBeta();
            
            ImGui::Text("Sigma (σ) - Convection:");
            if (ImGui::SliderFloat("##sigma", &sigma, 5.0f, 15.0f, "%.2f"))
            {
                lorenz->SetSigma(sigma);
            }
            
            ImGui::Text("Rho (ρ) - Temperature:");
            if (ImGui::SliderFloat("##rho", &rho, 20.0f, 35.0f, "%.2f"))
            {
                lorenz->SetRho(rho);
            }
            
            ImGui::Text("Beta (β) - Dimension:");
            if (ImGui::SliderFloat("##beta", &beta, 1.0f, 4.0f, "%.2f"))
            {
                lorenz->SetBeta(beta);
            }
            
            if (ImGui::Button("Valeurs classiques"))
            {
                lorenz->SetParameters(10.0f, 28.0f, 8.0f/3.0f);
            }
        }
    }
    else if (attractorName == "Rössler")
    {
        RosslerAttractor* rossler = dynamic_cast<RosslerAttractor*>(attractor);
        if (rossler)
        {
            float a = rossler->GetA();
            float b = rossler->GetB();
            float c = rossler->GetC();
            
            ImGui::Text("a - Couplage:");
            if (ImGui::SliderFloat("##a", &a, 0.1f, 0.4f, "%.2f"))
            {
                rossler->SetA(a);
            }
            
            ImGui::Text("b - Decalage:");
            if (ImGui::SliderFloat("##b", &b, 0.1f, 0.4f, "%.2f"))
            {
                rossler->SetB(b);
            }
            
            ImGui::Text("c - Non-linearite:");
            if (ImGui::SliderFloat("##c", &c, 4.0f, 7.0f, "%.2f"))
            {
                rossler->SetC(c);
            }
            
            if (ImGui::Button("Valeurs classiques"))
            {
                rossler->SetParameters(0.2f, 0.2f, 5.7f);
            }
        }
    }
    else if (attractorName == "Chen")
    {
        ChenAttractor* chen = dynamic_cast<ChenAttractor*>(attractor);
        if (chen)
        {
            float a = chen->GetA();
            float b = chen->GetB();
            float c = chen->GetC();
            
            ImGui::Text("a - Couplage:");
            if (ImGui::SliderFloat("##a", &a, 30.0f, 40.0f, "%.2f"))
            {
                chen->SetA(a);
            }
            
            ImGui::Text("b - Amortissement:");
            if (ImGui::SliderFloat("##b", &b, 2.0f, 4.0f, "%.2f"))
            {
                chen->SetB(b);
            }
            
            ImGui::Text("c - Bifurcation:");
            if (ImGui::SliderFloat("##c", &c, 25.0f, 35.0f, "%.2f"))
            {
                chen->SetC(c);
            }
            
            if (ImGui::Button("Valeurs classiques"))
            {
                chen->SetParameters(35.0f, 3.0f, 28.0f);
            }
        }
    }
    
    ImGui::End();
}

/**
 * RenderStatisticsWindow - Fenêtre des statistiques
 */
void UIController::RenderStatisticsWindow(AttractorSystem* attractor,
                                         Camera3D* camera,
                                         ParticleSystem* particles)
{
    ImGui::SetNextWindowPos(ImVec2(320, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_FirstUseEver);
    
    ImGui::Begin("Statistiques", &mState.showStatistics,
                 ImGuiWindowFlags_AlwaysAutoResize);
    
    // === Statistiques Camera ===
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Camera:");
    if (camera)
    {
        glm::vec3 pos = camera->GetPosition();
        ImGui::Text("  Position: (%.1f, %.1f, %.1f)", pos.x, pos.y, pos.z);
        ImGui::Text("  Distance: %.1f", camera->GetDistance());
    }
    
    ImGui::Separator();
    
    // === Statistiques Attracteur ===
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Attracteur:");
    if (attractor)
    {
        glm::vec3 state = attractor->GetCurrentState();
        ImGui::Text("  Position: (%.2f, %.2f, %.2f)", state.x, state.y, state.z);
        ImGui::Text("  Timestep: %.4f", attractor->GetTimeStep());
    }
    
    ImGui::Separator();
    
    // === Statistiques Particules ===
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Particules:");
    if (particles)
    {
        ImGui::Text("  Nombre: %zu", particles->GetParticleCount());
        ImGui::Text("  Trail max: %zu points", particles->GetMaxTrailLength());
        
        // Calcul approximatif du nombre total de points dessinés
        size_t totalPoints = particles->GetParticleCount() * particles->GetMaxTrailLength();
        ImGui::Text("  Points totaux: ~%zu", totalPoints);
    }
    
    ImGui::End();
}

/**
 * OnAttractorButtonClicked - Callback changement attracteur
 */
bool UIController::OnAttractorButtonClicked(int index)
{
    if (mRequestedAttractorIndex == index)
    {
        mRequestedAttractorIndex = -1;
        return true;
    }
    return false;
}