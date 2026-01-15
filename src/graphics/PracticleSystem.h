/**
 * ParticleSystem.h - Système de gestion des particules (SEMAINE 3)
 * 
 * Ce système gère une collection de particules qui tracent
 * les trajectoires de l'attracteur. Chaque particule conserve
 * un historique de ses positions (trail) pour visualiser le chemin parcouru.
 * 
 * NOUVEAUTÉS SEMAINE 3 :
 * - Coloration dynamique (vélocité, position, temps)
 * - Optimisations pour 500-1000 particules
 * - Calcul de statistiques (vélocité moyenne/max)
 * 
 * Auteur : [VOTRE NOM]
 * Date : Décembre 2025
 */

#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <glm/glm.hpp>
#include <vector>
#include <memory>

// Forward declarations
class Renderer;
class AttractorSystem;
class Camera3D;

// Mode de coloration (doit correspondre à ColorMode dans UIController)
enum class ParticleColorMode
{
    VELOCITY,
    POSITION,
    TIME,
    FIXED
};

/**
 * struct Particle - Représente une particule individuelle
 * 
 * Chaque particule :
 * - A une position actuelle dans l'espace 3D
 * - Conserve un historique de positions (trail)
 * - A une couleur qui peut varier dans le temps
 */
struct Particle
{
    glm::vec3 position;                  // Position actuelle (x, y, z)
    glm::vec3 velocity;                  // Vélocité actuelle (pour coloration)
    std::vector<glm::vec3> trail;        // Historique des positions
    glm::vec3 color;                     // Couleur (RGB entre 0 et 1)
    float timeAlive;                     // Temps depuis création (pour coloration temps)
    
    /**
     * Particle - Constructeur
     * @startPos: Position initiale de la particule
     */
    explicit Particle(const glm::vec3& startPos = glm::vec3(0.0f))
        : position(startPos)
        , velocity(0.0f)
        , color(1.0f, 1.0f, 1.0f)  // Blanc par défaut
        , timeAlive(0.0f)
    {
        // Réserver de l'espace pour le trail (optimisation mémoire)
        trail.reserve(1000);
    }
};

/**
 * class ParticleSystem - Gère une collection de particules
 * 
 * Responsabilités :
 * - Créer et détruire des particules
 * - Mettre à jour leurs positions selon l'attracteur
 * - Gérer les trails (historique)
 * - Rendre les particules à l'écran
 */
class ParticleSystem
{
public:
    /**
     * ParticleSystem - Constructeur
     * @numParticles: Nombre de particules à créer
     * 
     * Crée le nombre spécifié de particules avec des positions
     * initiales légèrement différentes pour créer un effet visuel.
     */
    explicit ParticleSystem(size_t numParticles = 100);
    
    /**
     * ~ParticleSystem - Destructeur
     */
    ~ParticleSystem() = default;
    
    /**
     * Update - Met à jour toutes les particules
     * @attractor: Pointeur vers l'attracteur à suivre
     * @deltaTime: Temps écoulé
     * 
     * Pour chaque particule :
     * 1. Calcule sa nouvelle position selon l'attracteur
     * 2. Ajoute cette position au trail
     * 3. Limite la longueur du trail si nécessaire
     */
    void Update(AttractorSystem* attractor, float deltaTime);
    
    /**
     * Render - Dessine toutes les particules et leurs trails
     * @renderer: Pointeur vers le système de rendu
     * @camera: Pointeur vers la caméra 3D pour projection
     * @colorMode: Mode de coloration (NOUVEAU Semaine 3)
     * @elapsedTime: Temps écoulé total (pour mode TIME)
     * 
     * NOUVEAU Semaine 3 : Coloration dynamique selon le mode choisi
     * 
     * Dessine :
     * - Les trails (lignes entre les points projetés)
     * - Les particules elles-mêmes (points plus gros)
     */
    void Render(Renderer* renderer, Camera3D* camera, 
                ParticleColorMode colorMode, float elapsedTime);
    
    /**
     * Clear - Efface toutes les particules et leurs trails
     * 
     * Utile pour recommencer avec un état propre.
     */
    void Clear();
    
    /**
     * Reset - Réinitialise les particules à leurs positions initiales
     * @attractor: Attracteur pour obtenir la position de départ
     * 
     * Garde les particules mais efface les trails.
     */
    void Reset(AttractorSystem* attractor);
    
    /**
     * SetParticleCount - Change le nombre de particules
     * @count: Nouveau nombre de particules
     * 
     * Recrée toutes les particules avec le nouveau nombre.
     */
    void SetParticleCount(size_t count);
    
    /**
     * GetParticleCount - Retourne le nombre actuel de particules
     * Return: Nombre de particules
     */
    size_t GetParticleCount() const { return mParticles.size(); }
    
    /**
     * SetMaxTrailLength - Définit la longueur maximale des trails
     * @length: Nombre maximum de points dans chaque trail
     * 
     * Plus le trail est long, plus le rendu est lourd mais plus
     * on voit l'historique de la trajectoire.
     */
    void SetMaxTrailLength(size_t length) { mMaxTrailLength = length; }
    
    /**
     * GetMaxTrailLength - Retourne la longueur max des trails
     * Return: Nombre maximum de points par trail
     */
    size_t GetMaxTrailLength() const { return mMaxTrailLength; }
    
    /**
     * GetAverageVelocity - NOUVEAU Semaine 3 : Calcule vélocité moyenne
     * 
     * Return: Magnitude moyenne de la vélocité de toutes les particules
     */
    float GetAverageVelocity() const;
    
    /**
     * GetMaxVelocity - NOUVEAU Semaine 3 : Trouve vélocité maximale
     * 
     * Return: Magnitude maximale de vélocité parmi toutes les particules
     */
    float GetMaxVelocity() const;

private:
    std::vector<Particle> mParticles;     // Collection de toutes les particules
    size_t mMaxTrailLength;               // Longueur maximale de chaque trail
    glm::vec3 mPreviousAttractorPos;      // Position précédente pour calcul vélocité
    
    /**
     * InitializeParticles - Crée les particules avec positions variées
     * @attractor: Attracteur pour obtenir la position de base
     * 
     * Crée chaque particule avec une position initiale légèrement
     * différente pour créer un effet de "nuage" de particules.
     */
    void InitializeParticles(AttractorSystem* attractor);
    
    /**
     * UpdateTrail - Met à jour le trail d'une particule
     * @particle: Référence vers la particule à mettre à jour
     * 
     * Ajoute la position actuelle au trail et supprime les
     * points les plus anciens si le trail est trop long.
     */
    void UpdateTrail(Particle& particle);
    
    /**
     * UpdateColor - NOUVEAU Semaine 3 : Met à jour la couleur d'une particule
     * @particle: Particule à colorier
     * @colorMode: Mode de coloration
     * @elapsedTime: Temps pour mode TIME
     */
    void UpdateColor(Particle& particle, ParticleColorMode colorMode, float elapsedTime);
};

#endif // PARTICLE_SYSTEM_H