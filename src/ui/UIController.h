/**
 * UIController.h - Contrôleur d'interface utilisateur ImGui
 * 
 * Cette classe encapsule toute l'interface utilisateur ImGui du projet.
 * Elle gère :
 * - Les fenêtres ImGui (infos, contrôles, paramètres)
 * - Les sliders pour modifier les paramètres des attracteurs
 * - Les boutons et checkboxes
 * - Les statistiques en temps réel
 * 
 * Séparer l'UI dans sa propre classe améliore l'organisation du code
 * et facilite les modifications futures de l'interface.
 * 
 * Auteur : [VOTRE NOM]
 * Date : Décembre 2025
 */

#ifndef UI_CONTROLLER_H
#define UI_CONTROLLER_H

#include <memory>
#include <string>

// Forward declarations
class Game;
class AttractorSystem;
class Camera3D;
class ParticleSystem;
class PresentationMode;  // NOUVEAU Semaine 4

/**
 * enum ColorMode - Modes de coloration des particules
 */
enum ColorMode
{
    COLOR_MODE_VELOCITY,    // Couleur selon la vélocité
    COLOR_MODE_POSITION,    // Couleur selon la position
    COLOR_MODE_TIME,        // Couleur selon le temps
    COLOR_MODE_FIXED        // Couleur fixe (blanc)
};

/**
 * struct UIState - État de l'interface utilisateur
 * 
 * Stocke toutes les variables de l'UI pour faciliter
 * la sauvegarde/restauration de l'état.
 */
struct UIState
{
    bool isPaused = false;               // Simulation en pause
    bool showInfo = true;                // Afficher fenêtre infos
    bool showControls = true;            // Afficher fenêtre contrôles
    bool showParameters = true;          // Afficher fenêtre paramètres
    bool showStatistics = true;          // Afficher fenêtre stats
    
    ColorMode colorMode = COLOR_MODE_VELOCITY;  // Mode de coloration actuel
    
    int particleCount = 200;             // Nombre de particules
    int maxTrailLength = 1000;           // Longueur max des trails
    float simulationSpeed = 1.0f;        // Vitesse de simulation (multiplicateur)
};

/**
 * class UIController - Gère l'interface utilisateur ImGui
 * 
 * Responsabilités :
 * - Dessiner toutes les fenêtres ImGui
 * - Gérer les interactions utilisateur (sliders, boutons)
 * - Mettre à jour les paramètres des attracteurs en temps réel
 * - Afficher les statistiques de performance
 */
class UIController
{
public:
    /**
     * UIController - Constructeur
     */
    UIController();
    
    /**
     * ~UIController - Destructeur
     */
    ~UIController() = default;
    
    /**
     * Render - Dessine toute l'interface ImGui
     * @attractor: Pointeur vers l'attracteur actuel
     * @camera: Pointeur vers la caméra
     * @particles: Pointeur vers le système de particules
     * @deltaTime: Temps de frame pour calcul FPS
     * @currentAttractorIndex: Index de l'attracteur actuel (0=Lorenz, 1=Rössler, 2=Chen)
     * @presentation: Pointeur vers le mode présentation (NOUVEAU Semaine 4)
     * 
     * Cette méthode dessine toutes les fenêtres ImGui actives.
     */
    void Render(AttractorSystem* attractor, 
                Camera3D* camera,
                ParticleSystem* particles,
                float deltaTime,
                int currentAttractorIndex,
                PresentationMode* presentation = nullptr);  // NOUVEAU paramètre
    
    /**
     * GetState - Retourne l'état actuel de l'UI
     * 
     * Return: Référence constante vers l'état
     */
    const UIState& GetState() const { return mState; }
    
    /**
     * GetState - Version non-const pour modification
     * 
     * Return: Référence modifiable vers l'état
     */
    UIState& GetState() { return mState; }
    
    /**
     * IsPaused - Vérifie si la simulation est en pause
     * 
     * Return: true si en pause
     */
    bool IsPaused() const { return mState.isPaused; }
    
    /**
     * TogglePause - Inverse l'état pause/play
     */
    void TogglePause() { mState.isPaused = !mState.isPaused; }
    
    /**
     * GetColorMode - Retourne le mode de coloration actuel
     * 
     * Return: Mode de coloration
     */
    ColorMode GetColorMode() const { return mState.colorMode; }
    
    /**
     * GetSimulationSpeed - Retourne le multiplicateur de vitesse
     * 
     * Return: Vitesse (1.0 = normal, 2.0 = double, 0.5 = moitié)
     */
    float GetSimulationSpeed() const { return mState.simulationSpeed; }

private:
    UIState mState;  // État de l'interface
    
    /**
     * RenderInfoWindow - Fenêtre d'informations générales
     * @deltaTime: Pour calcul FPS
     */
    void RenderInfoWindow(float deltaTime);
    
    /**
     * RenderControlsWindow - Fenêtre des contrôles de simulation
     * @currentAttractorIndex: Index attracteur actuel
     * @presentation: Pointeur vers mode présentation (NOUVEAU)
     * 
     * Contient :
     * - Boutons attracteurs
     * - Play/Pause
     * - Reset
     * - Vitesse simulation
     * - Bouton Mode Présentation (NOUVEAU Semaine 4)
     * - Bouton Capture d'écran (NOUVEAU Semaine 4)
     */
    void RenderControlsWindow(int currentAttractorIndex, PresentationMode* presentation = nullptr);
    
    /**
     * RenderParametersWindow - Fenêtre des paramètres d'attracteur
     * @attractor: Attracteur dont on modifie les paramètres
     * 
     * Affiche des sliders différents selon l'attracteur :
     * - Lorenz : σ, ρ, β
     * - Rössler : a, b, c
     * - Chen : a, b, c
     */
    void RenderParametersWindow(AttractorSystem* attractor);
    
    /**
     * RenderStatisticsWindow - Fenêtre des statistiques
     * @attractor: Pour vélocité
     * @camera: Pour info caméra
     * @particles: Pour stats particules
     */
    void RenderStatisticsWindow(AttractorSystem* attractor,
                                Camera3D* camera,
                                ParticleSystem* particles);
    
    /**
     * RenderVisualizationSettings - Section paramètres visuels
     * @particles: Pour modifier nombre particules et trails
     */
    void RenderVisualizationSettings(ParticleSystem* particles);
    
    /**
     * OnAttractorButtonClicked - Callback quand on clique sur un bouton attracteur
     * @index: Index du nouvel attracteur
     * 
     * Return: true si changement demandé
     */
    bool OnAttractorButtonClicked(int index);
    
    // === Variables pour les callbacks ===
    int mRequestedAttractorIndex = -1;  // -1 = pas de changement demandé
    bool mCaptureScreenshotRequested = false;  // NOUVEAU : true si screenshot demandé
    bool mPresentationToggleRequested = false; // NOUVEAU : true si toggle présentation
};

#endif // UI_CONTROLLER_H