/**
 * Game.h - Classe principale qui gère la boucle de jeu
 * 
 * Cette classe est responsable de :
 * - Créer et gérer la fenêtre SDL
 * - Gérer le renderer SDL
 * - Initialiser ImGui
 * - Gérer la boucle principale (événements, update, render)
 * - Coordonner tous les sous-systèmes (attracteurs, particules, caméra)
 * 
 * Auteur : [VOTRE NOM]
 * Date : Décembre 2025
 */

#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <memory>  // Pour std::unique_ptr

// Forward declarations (déclarations anticipées)
// Cela permet d'éviter d'inclure tous les headers ici
// et réduit les dépendances de compilation
class Renderer;
class AttractorSystem;  // Classe de base (polymorphisme)
class Camera3D;
class ParticleSystem;

/**
 * struct GameConfig - Configuration du jeu
 * 
 * Regroupe tous les paramètres de configuration
 * dans une structure pour faciliter les modifications
 */
struct GameConfig
{
    const char* windowTitle = "ChaosSim - Attracteurs Étranges";
    int windowWidth = 1280;
    int windowHeight = 720;
    int targetFPS = 60;
};

/**
 * class Game - Classe principale du jeu
 * 
 * Gère l'intégralité du cycle de vie de l'application :
 * - Initialisation des ressources
 * - Boucle principale (événements -> mise à jour -> rendu)
 * - Nettoyage des ressources
 */
class Game
{
public:
    /**
     * Game - Constructeur
     * 
     * Initialise tous les sous-systèmes :
     * - Crée la fenêtre SDL
     * - Crée le renderer SDL
     * - Initialise ImGui
     * - Crée l'attracteur de Lorenz
     * - Crée le système de particules
     * 
     * Peut lancer std::runtime_error en cas d'échec
     */
    Game();
    
    /**
     * ~Game - Destructeur
     * 
     * Nettoie toutes les ressources allouées :
     * - Détruit ImGui
     * - Détruit le renderer
     * - Détruit la fenêtre
     * - Libère la mémoire des sous-systèmes
     */
    ~Game();
    
    /**
     * Run - Lance la boucle principale du jeu
     * 
     * Cette méthode ne retourne que lorsque l'utilisateur
     * ferme la fenêtre ou appuie sur Échap.
     * 
     * Boucle : tant que mIsRunning est true
     *   1. HandleEvents() - Traite les événements SDL et ImGui
     *   2. Update() - Met à jour la logique (attracteur, particules)
     *   3. Render() - Dessine tout à l'écran
     */
    void Run();
    
    /**
     * HandleEvents - Gère les événements SDL
     * 
     * Traite tous les événements en attente :
     * - Fermeture de fenêtre (SDL_EVENT_QUIT)
     * - Touches clavier (Échap pour quitter)
     * - Événements souris (pour rotation caméra future)
     * - Événements ImGui (clics dans l'interface)
     */
    void HandleEvents();
    
    /**
     * Update - Met à jour la logique du jeu
     * @deltaTime: Temps écoulé depuis la dernière frame (en secondes)
     * 
     * Appelle les méthodes Update() de tous les sous-systèmes :
     * - Attracteur (calcul nouvelles positions)
     * - Particules (mise à jour trails)
     * - Caméra (animations futures)
     */
    void Update(float deltaTime);
    
    /**
     * Render - Dessine tout à l'écran
     * 
     * Séquence de rendu :
     * 1. Efface l'écran (fond noir)
     * 2. Dessine les particules de l'attracteur
     * 3. Prépare la nouvelle frame ImGui
     * 4. Dessine l'interface utilisateur
     * 5. Présente le résultat à l'écran (SDL_RenderPresent)
     */
    void Render();

private:
    // === Ressources SDL ===
    SDL_Window* mWindow;       // Pointeur vers la fenêtre SDL
    Renderer* mSDLRenderer; // Pointeur vers le renderer SDL
    
    // === État du jeu ===
    bool mIsRunning;           // true tant que le jeu tourne
    
    // === Configuration ===
    GameConfig mConfig;        // Paramètres de configuration
    
    // === Timing (gestion du temps) ===
    Uint64 mLastFrameTime;     // Timestamp de la dernière frame
    float mDeltaTime;          // Temps entre deux frames (pour Update)
    
    // === Sous-systèmes (utilisation de smart pointers) ===
    // std::unique_ptr gère automatiquement la mémoire
    // Pas besoin de delete manuel dans le destructeur
    std::unique_ptr<Renderer> mRenderer;              // Système de rendu
    std::unique_ptr<Camera3D> mCamera;                // Caméra 3D
    std::unique_ptr<AttractorSystem> mAttractor;      // Attracteur actuel (polymorphisme)
    std::unique_ptr<ParticleSystem> mParticles;       // Système de particules
    
    // === Gestion des attracteurs ===
    int mCurrentAttractorIndex;                       // Index de l'attracteur actuel (0=Lorenz, 1=Rössler, 2=Chen)
    
    // === Gestion de la souris pour la caméra ===
    bool mIsMouseDragging;                            // true si on fait glisser la souris
    int mLastMouseX;                                  // Dernière position X de la souris
    int mLastMouseY;                                  // Dernière position Y de la souris
    
    /**
     * InitializeSDL - Initialise la fenêtre et le renderer SDL
     * 
     * Crée la fenêtre centrée à l'écran avec les dimensions configurées.
     * Crée le renderer avec accélération matérielle (GPU) si disponible.
     * 
     * Peut lancer std::runtime_error en cas d'échec.
     */
    void InitializeSDL();
    
    /**
     * InitializeImGui - Initialise Dear ImGui
     * 
     * Configure ImGui pour fonctionner avec SDL3.
     * Définit le style visuel de l'interface.
     * 
     * Peut lancer std::runtime_error en cas d'échec.
     */
    void InitializeImGui();
    
    /**
     * CalculateDeltaTime - Calcule le temps écoulé depuis la dernière frame
     * 
     * Utilise SDL_GetPerformanceCounter() pour un timing précis.
     * Met à jour mDeltaTime et mLastFrameTime.
     */
    void CalculateDeltaTime();
    
    /**
     * SwitchAttractor - Change l'attracteur actuel
     * @index: Index du nouvel attracteur (0=Lorenz, 1=Rössler, 2=Chen)
     * 
     * Crée un nouvel attracteur et réinitialise le système de particules.
     */
    void SwitchAttractor(int index);
    
    /**
     * HandleMouseInput - Gère les événements souris pour la caméra
     * @event: Événement SDL à traiter
     * 
     * Gère le drag pour rotation et la molette pour le zoom.
     */
    void HandleMouseInput(const SDL_Event& event);
};

#endif // GAME_H