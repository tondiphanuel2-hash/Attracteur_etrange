/**
 * ParticleSystem.cpp - Implémentation du système de particules (SEMAINE 2)
 * 
 * Gère la création, mise à jour et rendu des particules.
 * 
 * NOUVEAUTÉS SEMAINE 2 :
 * - Projection 3D complète avec Camera3D
 * - Rendu des trails visible
 * - Support de multiple attracteurs
 */

#include "ParticleSystem.h"
#include "../core/Renderer.h"
#include "../attractors/AttractorSystem.h"
#include "../graphics/Camera3D.h"  // NOUVEAU
#include <random>

/**
 * ParticleSystem - Constructeur
 * @numParticles: Nombre de particules à créer
 */
ParticleSystem::ParticleSystem(size_t numParticles)
    : mMaxTrailLength(1000)  // Par défaut, chaque trail peut contenir 1000 points
{
    // Réserver de l'espace pour toutes les particules (optimisation)
    mParticles.reserve(numParticles);
    
    // Créer les particules
    // Note : InitializeParticles sera appelé plus tard avec l'attracteur
    // Pour l'instant, créer des particules vides
    for (size_t i = 0; i < numParticles; ++i)
    {
        mParticles.emplace_back(Particle(glm::vec3(0.0f)));
    }
}

/**
 * Update - Met à jour toutes les particules
 * @attractor: Pointeur vers l'attracteur à suivre
 * @deltaTime: Temps écoulé (non utilisé pour l'instant)
 */
void ParticleSystem::Update(AttractorSystem* attractor, float deltaTime)
{
    // Vérifier que l'attracteur est valide
    if (!attractor)
    {
        return;
    }
    
    // Pour l'instant (Semaine 1), on ne fait qu'un update très simple
    // Chaque particule suit la même position de l'attracteur
    // 
    // Semaine 2 : On ajoutera des positions légèrement décalées
    // pour créer un nuage de particules
    
    glm::vec3 attractorPos = attractor->GetCurrentState();
    
    // Mettre à jour chaque particule
    for (auto& particle : mParticles)
    {
        // Mettre à jour la position de la particule
        particle.position = attractorPos;
        
        // Ajouter cette position au trail
        UpdateTrail(particle);
    }
    
    // Éviter le warning "unused parameter"
    (void)deltaTime;
}

/**
 * UpdateTrail - Met à jour le trail d'une particule
 * @particle: Référence vers la particule
 */
void ParticleSystem::UpdateTrail(Particle& particle)
{
    // Ajouter la position actuelle au trail
    particle.trail.push_back(particle.position);
    
    // Si le trail est trop long, supprimer les points les plus anciens
    if (particle.trail.size() > mMaxTrailLength)
    {
        // Supprimer les 10 premiers points d'un coup (plus efficace)
        particle.trail.erase(
            particle.trail.begin(),
            particle.trail.begin() + 10
        );
    }
}

/**
 * Render - Dessine toutes les particules avec projection 3D
 * @renderer: Pointeur vers le système de rendu
 * @camera: Pointeur vers la caméra 3D
 */
void ParticleSystem::Render(Renderer* renderer, Camera3D* camera)
{
    // Vérifier que le renderer et la caméra sont valides
    if (!renderer || !camera)
    {
        return;
    }
    
    // Dessiner chaque particule
    for (const auto& particle : mParticles)
    {
        // Si le trail a au moins 2 points, dessiner les lignes
        if (particle.trail.size() >= 2)
        {
            // Projeter tous les points 3D vers l'écran 2D
            std::vector<glm::vec2> screenPoints;
            screenPoints.reserve(particle.trail.size());
            
            for (const auto& point3D : particle.trail)
            {
                // Utiliser la caméra pour projeter le point 3D
                glm::vec2 screenPos = camera->ProjectToScreen(point3D);
                screenPoints.push_back(screenPos);
            }
            
            // Dessiner le trail
            // Convertir la couleur glm::vec3 (0-1) en Color (0-255)
            Color trailColor(
                static_cast<Uint8>(particle.color.r * 255),
                static_cast<Uint8>(particle.color.g * 255),
                static_cast<Uint8>(particle.color.b * 255),
                180  // Alpha à 180 pour un peu de transparence
            );
            
            renderer->DrawLines(screenPoints, trailColor);
        }
        
        // Dessiner la particule elle-même (point plus gros)
        if (!particle.trail.empty())
        {
            const glm::vec3& pos3D = particle.trail.back();
            glm::vec2 screenPos = camera->ProjectToScreen(pos3D);
            
            Color particleColor(
                static_cast<Uint8>(particle.color.r * 255),
                static_cast<Uint8>(particle.color.g * 255),
                static_cast<Uint8>(particle.color.b * 255),
                255  // Opaque
            );
            
            renderer->DrawPoint(screenPos, particleColor, 3);
        }
    }
}

/**
 * Clear - Efface toutes les particules et leurs trails
 */
void ParticleSystem::Clear()
{
    mParticles.clear();
}

/**
 * Reset - Réinitialise les particules
 * @attractor: Attracteur pour obtenir la position de départ
 */
void ParticleSystem::Reset(AttractorSystem* attractor)
{
    if (!attractor)
    {
        return;
    }
    
    // Effacer les trails de toutes les particules
    for (auto& particle : mParticles)
    {
        particle.trail.clear();
        particle.position = attractor->GetInitialState();
    }
}

/**
 * SetParticleCount - Change le nombre de particules
 * @count: Nouveau nombre de particules
 */
void ParticleSystem::SetParticleCount(size_t count)
{
    // Effacer les particules existantes
    mParticles.clear();
    
    // Réserver l'espace
    mParticles.reserve(count);
    
    // Créer les nouvelles particules
    for (size_t i = 0; i < count; ++i)
    {
        mParticles.emplace_back(Particle(glm::vec3(0.0f)));
    }
}

/**
 * InitializeParticles - Initialise les particules avec positions variées
 * @attractor: Attracteur pour obtenir la position de base
 * 
 * TODO Semaine 2 : Implémenter avec positions aléatoires
 */
void ParticleSystem::InitializeParticles(AttractorSystem* attractor)
{
    if (!attractor)
    {
        return;
    }
    
    // Générateur de nombres aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-0.5f, 0.5f);
    
    // Position de base de l'attracteur
    glm::vec3 basePos = attractor->GetInitialState();
    
    // Créer chaque particule avec une position légèrement différente
    for (auto& particle : mParticles)
    {
        // Ajouter une petite perturbation aléatoire
        particle.position = basePos + glm::vec3(
            dis(gen),  // Décalage X aléatoire
            dis(gen),  // Décalage Y aléatoire
            dis(gen)   // Décalage Z aléatoire
        );
        
        // Couleur aléatoire (pour l'instant, toutes blanches)
        particle.color = glm::vec3(1.0f, 1.0f, 1.0f);
    }
}