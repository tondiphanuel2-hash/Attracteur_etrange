/**
 * PresentationMode.h - Mode présentation automatique
 * 
 * Ce mode permet de créer une démonstration automatique qui :
 * - Change d'attracteur automatiquement
 * - Fait tourner la caméra progressivement
 * - Zoome/dézoome de manière fluide
 * - Transitions en douceur entre les attracteurs
 * 
 * Parfait pour les présentations de 5 minutes devant le professeur !
 * 
 * Auteur : [VOTRE NOM]
 * Date : Décembre 2025
 */

#ifndef PRESENTATION_MODE_H
#define PRESENTATION_MODE_H

#include <vector>
#include <string>

// Forward declarations
class Camera3D;
class Game;

/**
 * enum PresentationPhase - Phase actuelle de la présentation
 */
enum class PresentationPhase
{
    LORENZ_INTRO,          // Introduction avec Lorenz
    LORENZ_ROTATE,         // Rotation autour de Lorenz
    TRANSITION_TO_ROSSLER, // Transition vers Rössler
    ROSSLER_SHOWCASE,      // Présentation de Rössler
    TRANSITION_TO_CHEN,    // Transition vers Chen
    CHEN_SHOWCASE,         // Présentation de Chen
    FINALE,                // Retour à Lorenz avec zoom out
    COMPLETED              // Présentation terminée
};

/**
 * struct PresentationConfig - Configuration de la présentation
 */
struct PresentationConfig
{
    float phaseDuration = 10.0f;        // Durée de chaque phase (secondes)
    float transitionDuration = 3.0f;    // Durée des transitions
    float rotationSpeed = 0.2f;         // Vitesse de rotation auto
    float zoomSpeed = 5.0f;             // Vitesse du zoom
    bool loopPresentation = false;      // Boucler à la fin
};

/**
 * class PresentationMode - Gère la présentation automatique
 * 
 * Contrôle automatiquement :
 * - Changement d'attracteurs
 * - Rotation de caméra
 * - Zoom de caméra
 * - Transitions fluides
 */
class PresentationMode
{
public:
    /**
     * PresentationMode - Constructeur
     */
    PresentationMode();
    
    /**
     * ~PresentationMode - Destructeur
     */
    ~PresentationMode() = default;
    
    /**
     * Start - Démarre la présentation
     */
    void Start();
    
    /**
     * Stop - Arrête la présentation
     */
    void Stop();
    
    /**
     * Update - Met à jour la présentation
     * @camera: Caméra à contrôler
     * @deltaTime: Temps écoulé
     * 
     * Return: Index de l'attracteur à afficher (-1 si pas de changement)
     */
    int Update(Camera3D* camera, float deltaTime);
    
    /**
     * IsActive - Vérifie si la présentation est active
     * 
     * Return: true si en mode présentation
     */
    bool IsActive() const { return mIsActive; }
    
    /**
     * GetCurrentPhase - Retourne la phase actuelle
     * 
     * Return: Phase en cours
     */
    PresentationPhase GetCurrentPhase() const { return mCurrentPhase; }
    
    /**
     * GetPhaseName - Nom de la phase actuelle
     * 
     * Return: Nom en string
     */
    std::string GetPhaseName() const;
    
    /**
     * GetProgress - Progression dans la phase actuelle
     * 
     * Return: Valeur entre 0 (début) et 1 (fin)
     */
    float GetProgress() const;
    
    /**
     * GetTotalProgress - Progression totale de la présentation
     * 
     * Return: Valeur entre 0 et 1
     */
    float GetTotalProgress() const;
    
    /**
     * GetConfig - Accès à la configuration
     * 
     * Return: Référence modifiable à la config
     */
    PresentationConfig& GetConfig() { return mConfig; }

private:
    PresentationConfig mConfig;       // Configuration
    bool mIsActive;                   // true si présentation en cours
    PresentationPhase mCurrentPhase;  // Phase actuelle
    float mPhaseTimer;                // Temps écoulé dans la phase
    float mTotalTimer;                // Temps total écoulé
    
    float mInitialCameraDistance;     // Distance caméra au départ
    float mTargetCameraDistance;      // Distance cible
    
    /**
     * UpdateLorenzIntro - Phase d'introduction avec Lorenz
     * @camera: Caméra à animer
     * @deltaTime: Temps écoulé
     */
    void UpdateLorenzIntro(Camera3D* camera, float deltaTime);
    
    /**
     * UpdateLorenzRotate - Rotation autour de Lorenz
     * @camera: Caméra à animer
     * @deltaTime: Temps écoulé
     */
    void UpdateLorenzRotate(Camera3D* camera, float deltaTime);
    
    /**
     * UpdateTransitionToRossler - Transition vers Rössler
     * @camera: Caméra à animer
     * @deltaTime: Temps écoulé
     * 
     * Return: Index attracteur à charger (1 = Rössler)
     */
    int UpdateTransitionToRossler(Camera3D* camera, float deltaTime);
    
    /**
     * UpdateRosslerShowcase - Présentation de Rössler
     * @camera: Caméra à animer
     * @deltaTime: Temps écoulé
     */
    void UpdateRosslerShowcase(Camera3D* camera, float deltaTime);
    
    /**
     * UpdateTransitionToChen - Transition vers Chen
     * @camera: Caméra à animer
     * @deltaTime: Temps écoulé
     * 
     * Return: Index attracteur à charger (2 = Chen)
     */
    int UpdateTransitionToChen(Camera3D* camera, float deltaTime);
    
    /**
     * UpdateChenShowcase - Présentation de Chen
     * @camera: Caméra à animer
     * @deltaTime: Temps écoulé
     */
    void UpdateChenShowcase(Camera3D* camera, float deltaTime);
    
    /**
     * UpdateFinale - Phase finale
     * @camera: Caméra à animer
     * @deltaTime: Temps écoulé
     * 
     * Return: Index attracteur à charger (0 = Lorenz)
     */
    int UpdateFinale(Camera3D* camera, float deltaTime);
    
    /**
     * AdvanceToNextPhase - Passe à la phase suivante
     */
    void AdvanceToNextPhase();
};

#endif // PRESENTATION_MODE_H