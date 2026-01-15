/**
 * AttractorSystem.cpp - Implémentation de la classe abstraite
 * 
 * Contient l'implémentation des méthodes communes à tous les attracteurs.
 */

#if __has_include("AttractorSystem.h")
#include "AttractorSystem.h"
#include <string>Message
#else
// Minimal glm::vec3 fallback when GLM headers are not available.
namespace glm {
struct vec3 {
    float x, y, z;
    vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    vec3& operator+=(const vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }
};
inline vec3 operator+(const vec3& a, const vec3& b) { return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline vec3 operator*(const vec3& v, float s) { return vec3(v.x * s, v.y * s, v.z * s); }
inline vec3 operator*(float s, const vec3& v) { return vec3(v.x * s, v.y * s, v.z * s); }
} // namespace glm

// Fallback minimal declaration when the header file is not found.
// This allows the .cpp to compile while the proper header is added to the include path.
class AttractorSystem {
protected:
    glm::vec3 mCurrentState;
    glm::vec3 mInitialState;
    float mTimeStep;
    float mAccumulatedTime;

public:
    AttractorSystem();
    virtual ~AttractorSystem() = default;

    // Chaque attracteur doit implémenter le calcul de la dérivée
    virtual void ComputeDerivative(const glm::vec3& state, glm::vec3& derivative) = 0;

    // Méthodes implémentées dans ce fichier
    void Update(float deltaTime);
    void Reset();
    void SetInitialState(const glm::vec3& state);
    void IntegrateEuler(float deltaTime);
    void IntegrateRK4(float deltaTime);
};
#endif

/**
 * AttractorSystem - Constructeur
 * 
 * Initialise l'attracteur avec une position de départ légèrement
 * perturbée de (0, 0, 0) pour éviter les singularités mathématiques.
 */
AttractorSystem::AttractorSystem()
    : mCurrentState(0.1f, 0.0f, 0.0f)    // Position initiale légèrement décalée
    , mInitialState(0.1f, 0.0f, 0.0f)    // Même position initiale sauvegardée
    , mTimeStep(0.01f)                    // Pas de temps de 10ms
    , mAccumulatedTime(0.0f)              // Compteur de temps à 0
{
    // La position initiale (0.1, 0, 0) est choisie car :
    // - Elle évite le point (0, 0, 0) qui peut être un point d'équilibre
    // - Elle est assez proche de l'origine pour converger vers l'attracteur
    // - Elle fonctionne bien pour la plupart des attracteurs chaotiques
}

/**
 * Update - Met à jour l'état de l'attracteur
 * @deltaTime: Temps écoulé depuis la dernière frame (en secondes)
 * 
 * Cette méthode peut être appelée plusieurs fois par frame si deltaTime
 * est grand, pour maintenir la stabilité numérique.
 */
void AttractorSystem::Update(float deltaTime)
{
    // Accumuler le temps écoulé
    mAccumulatedTime += deltaTime;
    
    // Tant qu'on a accumulé assez de temps pour faire un pas d'intégration
    while (mAccumulatedTime >= mTimeStep)
    {
        // SEMAINE 3 : Utiliser RK4 au lieu d'Euler pour meilleure stabilité
        IntegrateRK4(mTimeStep);
        
        // Décrémenter le temps accumulé
        mAccumulatedTime -= mTimeStep;
    }
    
    // Note : Le temps restant dans mAccumulatedTime sera utilisé
    // à la prochaine frame. Cela évite la perte de précision.
}

/**
 * Reset - Réinitialise l'attracteur à sa position initiale
 */
void AttractorSystem::Reset()
{
    // Restaurer la position initiale
    mCurrentState = mInitialState;
    
    // Réinitialiser le compteur de temps
    mAccumulatedTime = 0.0f;
}

/**
 * SetInitialState - Définit une nouvelle position initiale
 * @state: Nouvelle position de départ
 */
void AttractorSystem::SetInitialState(const glm::vec3& state)
{
    mInitialState = state;
}

/**
 * IntegrateEuler - Intégration par méthode d'Euler
 * @deltaTime: Pas de temps d'intégration
 * 
 * La méthode d'Euler est la plus simple des méthodes d'intégration :
 * 
 * Position future = Position actuelle + Vitesse * Temps
 * 
 * En termes mathématiques :
 * y(t + Δt) ≈ y(t) + Δt * f(t, y(t))
 * 
 * où f(t, y) est la dérivée (calculée par ComputeDerivative)
 */
void AttractorSystem::IntegrateEuler(float deltaTime)
{
    // Vecteur pour stocker la dérivée (dx/dt, dy/dt, dz/dt)
    glm::vec3 derivative;
    
    // Calculer la dérivée au point actuel
    // Cette méthode est implémentée par chaque attracteur spécifique
    ComputeDerivative(mCurrentState, derivative);
    
    // Mettre à jour la position selon Euler :
    // nouvelle_position = position_actuelle + dérivée * pas_de_temps
    mCurrentState += derivative * deltaTime;
    
    // Note : Cette méthode est simple mais :
    // - Accumule des erreurs sur le long terme
    // - Peut être instable si deltaTime est trop grand
    // 
    // C'est pourquoi nous passerons à RK4 en Semaine 3
}

/**
 * IntegrateRK4 - Intégration par Runge-Kutta d'ordre 4 (SEMAINE 3)
 * @deltaTime: Pas de temps d'intégration
 * 
 * RK4 est beaucoup plus précise qu'Euler :
 * - Calcule 4 dérivées intermédiaires (k1, k2, k3, k4)
 * - Fait une moyenne pondérée de ces dérivées
 * - Réduit l'erreur d'intégration d'un facteur ~100
 * 
 * La méthode RK4 complète est maintenant implémentée
 */
void AttractorSystem::IntegrateRK4(float deltaTime)
{
    glm::vec3 k1, k2, k3, k4;
    glm::vec3 temp;
    
    // k1 = f(t, y)
    // Dérivée au point actuel
    ComputeDerivative(mCurrentState, k1);
    
    // k2 = f(t + dt/2, y + dt*k1/2)
    // Dérivée au milieu du pas de temps, avec k1
    temp = mCurrentState + k1 * (deltaTime * 0.5f);
    ComputeDerivative(temp, k2);
    
    // k3 = f(t + dt/2, y + dt*k2/2)
    // Dérivée au milieu du pas de temps, avec k2
    temp = mCurrentState + k2 * (deltaTime * 0.5f);
    ComputeDerivative(temp, k3);
    
    // k4 = f(t + dt, y + dt*k3)
    // Dérivée à la fin du pas de temps, avec k3
    temp = mCurrentState + k3 * deltaTime;
    ComputeDerivative(temp, k4);
    
    // y(t + dt) = y(t) + (dt/6)(k1 + 2k2 + 2k3 + k4)
    // Moyenne pondérée des 4 dérivées
    // Les poids sont : 1/6, 2/6, 2/6, 1/6
    // Cela donne plus d'importance aux dérivées au milieu
    mCurrentState += (k1 + 2.0f * k2 + 2.0f * k3 + k4) * (deltaTime / 6.0f);
    
    // Note : RK4 est un excellent compromis entre précision et performance.
    // C'est la méthode standard pour les systèmes dynamiques chaotiques.
}