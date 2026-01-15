/**
 * AttractorSystem.h - Classe abstraite pour tous les attracteurs
 * 
 * Cette classe définit l'interface commune que tous les attracteurs
 * doivent implémenter. Elle utilise le polymorphisme pour permettre
 * de traiter différents types d'attracteurs de manière uniforme.
 * 
 * Design Pattern utilisé : Template Method + Strategy
 * 
 * Auteur : [VOTRE NOM]
 * Date : Décembre 2025
 */

#ifndef ATTRACTOR_SYSTEM_H
#define ATTRACTOR_SYSTEM_H

#pragma once

#include <string>
#include <vector>

#if defined(__has_include)
  #if __has_include(<glm/glm.hpp>)
    #include <glm/glm.hpp>
  #elif __has_include(<glm/vec3.hpp>)
    #include <glm/vec3.hpp>
  #else
    // Fallback minimal si GLM absent (pour compilation temporaire)
    namespace glm {
      struct vec3 {
        float x = 0.f, y = 0.f, z = 0.f;
        vec3() = default;
        vec3(float a, float b, float c) : x(a), y(b), z(c) {}
        vec3& operator+=(const vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }
      };
      inline vec3 operator+(const vec3& a, const vec3& b) { return vec3(a.x+b.x, a.y+b.y, a.z+b.z); }
      inline vec3 operator*(const vec3& v, float s) { return vec3(v.x*s, v.y*s, v.z*s); }
    }
  #endif
#else
  #include <glm/glm.hpp>
#endif

class AttractorSystem {
public:
    AttractorSystem();
    virtual ~AttractorSystem() = default;

    void Update(float deltaTime);
    void Reset();
    void SetInitialState(const glm::vec3& state);

protected:
    virtual void ComputeDerivative(const glm::vec3& state, glm::vec3& outDerivative) = 0;
    void IntegrateEuler(float deltaTime);
    void IntegrateRK4(float deltaTime);

    glm::vec3 mCurrentState;
    glm::vec3 mInitialState;
    float mTimeStep = 0.01f;
    float mAccumulatedTime = 0.0f;
};
#include <string>

/**
 * Cette architecture permet :
 * - D'ajouter facilement de nouveaux attracteurs
 * - De changer d'attracteur à la volée
 * - De traiter tous les attracteurs de manière uniforme
 */
class AttractorSystem
{
public:
    // Constructeur / destructeur
    AttractorSystem();
    virtual ~AttractorSystem() = default;

    // Mise à jour et réinitialisation
    void Update(float deltaTime);
    void Reset();

    // Accesseurs d'état
    glm::vec3 GetCurrentState() const { return mCurrentState; }
    glm::vec3 GetInitialState() const { return mInitialState; }
    void SetInitialState(const glm::vec3& state);

    // Nom de l'attracteur (ex: "Lorenz", "Rossler")
    virtual std::string GetName() const = 0;

    // Pas de temps
    float GetTimeStep() const { return mTimeStep; }
    void SetTimeStep(float timeStep) { mTimeStep = timeStep; }

protected:
    // Calcule la dérivée f(state) = (dx/dt, dy/dt, dz/dt)
    // Méthode purement virtuelle : chaque attracteur doit l'implémenter.
    virtual void ComputeDerivative(const glm::vec3& state, glm::vec3& outDerivative) const = 0;

    // Schémas d'intégration disponibles
    void IntegrateEuler(float deltaTime);
    void IntegrateRK4(float deltaTime);

protected:
    // État de l'attracteur
    glm::vec3 mCurrentState{};   // Position actuelle (x, y, z)
    glm::vec3 mInitialState{};   // Position de départ (pour Reset)

    // Paramètres d'intégration
    float mTimeStep{0.01f};      // Pas de temps pour l'intégration numérique
    float mAccumulatedTime{0.0f}; // Temps accumulé (utile pour sous-échantillonnage)
};

#endif // ATTRACTOR_SYSTEM_H