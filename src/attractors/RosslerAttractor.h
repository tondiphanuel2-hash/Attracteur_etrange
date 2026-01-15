/**
 * RosslerAttractor.h - Implémentation de l'attracteur de Rössler
 * 
 * L'attracteur de Rössler est un système d'équations différentielles
 * découvert par Otto Rössler en 1976. C'est l'un des exemples les plus
 * simples de systèmes chaotiques continus.
 * 
 * Équations :
 *   dx/dt = -y - z
 *   dy/dt = x + ay
 *   dz/dt = b + z(x - c)
 * 
 * Où :
 *   a = paramètre de couplage linéaire
 *   b = paramètre de décalage
 *   c = paramètre de non-linéarité
 * 
 * Valeurs classiques pour comportement chaotique :
 *   a = 0.2
 *   b = 0.2
 *   c = 5.7
 * 
 * Contrairement à Lorenz qui forme un "papillon", l'attracteur de Rössler
 * ressemble à un "ruban tordu" ou une "bande de Möbius".
 * 
 * Auteur : [VOTRE NOM]
 * Date : Décembre 2025
 * Référence : Rössler, O. E. (1976). "An equation for continuous chaos"
 */

#pragma once

  #include "../AttractorSystem.h"
#include "AttractorSysteme.h"

/**
 * class RosslerAttractor - Attracteur de Rössler
 * 
 * Hérite de AttractorSystem et implémente les équations
 * spécifiques de Rössler.
 */
class RosslerAttractor : public AttractorSystem
{
public:
    /**
     * RosslerAttractor - Constructeur avec paramètres par défaut
     * 
     * Initialise avec les valeurs classiques qui produisent
     * le comportement chaotique caractéristique en "ruban".
     */
    RosslerAttractor(float a = 0.2f, float b = 0.2f, float c = 5.7f);
    
    /**
     * ~RosslerAttractor - Destructeur
     */
    ~RosslerAttractor() override = default;
    
    /**
     * GetName - Retourne le nom de l'attracteur
     * 
     * Return: "Rössler"
     */
    std::string GetName() const override;
    
    // === Getters pour les paramètres ===
    
    /**
     * GetA - Retourne le paramètre a actuel
     * Return: Valeur de a
     */
    float GetA() const { return mA; }
    
    /**
     * GetB - Retourne le paramètre b actuel
     * Return: Valeur de b
     */
    float GetB() const { return mB; }
    
    /**
     * GetC - Retourne le paramètre c actuel
     * Return: Valeur de c
     */
    float GetC() const { return mC; }
    
    // === Setters pour les paramètres ===
    
    /**
     * SetA - Modifie le paramètre a
     * @a: Nouvelle valeur (recommandé : 0.1-0.3)
     * 
     * Contrôle le couplage linéaire dans l'équation dy/dt.
     * Valeur classique : 0.2
     */
    void SetA(float a) { mA = a; }
    
    /**
     * SetB - Modifie le paramètre b
     * @b: Nouvelle valeur (recommandé : 0.1-0.3)
     * 
     * Contrôle le terme constant dans l'équation dz/dt.
     * Valeur classique : 0.2
     */
    void SetB(float b) { mB = b; }
    
    /**
     * SetC - Modifie le paramètre c
     * @c: Nouvelle valeur (recommandé : 4.0-7.0)
     * 
     * Contrôle la non-linéarité dans l'équation dz/dt.
     * Valeur classique : 5.7
     * 
     * Note : Pour c < 4, comportement périodique (pas chaotique)
     *        Pour c ≈ 4.2, bifurcation vers le chaos
     *        Pour c > 6, comportement plus complexe
     */
    void SetC(float c) { mC = c; }
    
    /**
     * SetParameters - Modifie tous les paramètres à la fois
     * @a: Nouveau a
     * @b: Nouveau b
     * @c: Nouveau c
     * 
     * Utile pour l'interface ImGui.
     */
    void SetParameters(float a, float b, float c);

protected:
    /**
     * ComputeDerivative - Calcule la dérivée selon les équations de Rössler
     * @state: État actuel (x, y, z)
     * @outDerivative: Sortie contenant (dx/dt, dy/dt, dz/dt)
     * 
     * Implémente les équations :
     *   dx/dt = -y - z
     *   dy/dt = x + ay
     *   dz/dt = b + z(x - c)
     */
    void ComputeDerivative(const glm::vec3& state, 
                          glm::vec3& outDerivative) override;

private:
    // === Paramètres de l'attracteur de Rössler ===
    float mA;  // Paramètre de couplage (typiquement 0.2)
    float mB;  // Paramètre de décalage (typiquement 0.2)
    float mC;  // Paramètre de non-linéarité (typiquement 5.7)
};