/**
 * LorenzAttractor.h - Implémentation de l'attracteur de Lorenz
 * 
 * L'attracteur de Lorenz est un système d'équations différentielles
 * découvert par Edward Lorenz en 1963 lors de ses recherches sur
 * la modélisation météorologique.
 * 
 * Équations :
 *   dx/dt = σ(y - x)
 *   dy/dt = x(ρ - z) - y
 *   dz/dt = xy - βz
 * 
 * Où :
 *   σ (sigma) = taux de convection (viscosité)
 *   ρ (rho)   = différence de température
 *   β (beta)  = dimension physique du système
 * 
 * Valeurs classiques :
 *   σ = 10.0
 *   ρ = 28.0
 *   β = 8/3 ≈ 2.6667
 * 
 * Avec ces paramètres, le système exhibe un comportement chaotique
 * et forme la célèbre "double aile de papillon".
 * 
 * Auteur : [VOTRE NOM]
 * Date : Décembre 2025
 * Référence : Lorenz, E. N. (1963). "Deterministic Nonperiodic Flow"
 */

#ifndef LORENZ_ATTRACTOR_H
#define LORENZ_ATTRACTOR_H

#include "AttractorSystem.h"

/**
 * class LorenzAttractor - Attracteur de Lorenz
 * 
 * Hérite de AttractorSystem et implémente les équations
 * spécifiques de Lorenz.
 */
class LorenzAttractor : public AttractorSystem
{
public:
    /**
     * LorenzAttractor - Constructeur avec paramètres par défaut
     * 
     * Initialise l'attracteur avec les valeurs classiques qui
     * produisent le comportement chaotique caractéristique.
     */
    LorenzAttractor();
    
    /**
     * LorenzAttractor - Constructeur avec paramètres personnalisés
     * @sigma: Paramètre σ (taux de convection)
     * @rho: Paramètre ρ (différence de température)
     * @beta: Paramètre β (dimension physique)
     * 
     * Permet de créer un attracteur de Lorenz avec des paramètres
     * différents pour explorer d'autres comportements.
     */
    LorenzAttractor(float sigma, float rho, float beta);
    
    /**
     * ~LorenzAttractor - Destructeur
     */
    ~LorenzAttractor() override = default;
    
    /**
     * GetName - Retourne le nom de l'attracteur
     * 
     * Return: "Lorenz"
     */
    std::string GetName() const override;
    
    // === Getters pour les paramètres ===
    
    /**
     * GetSigma - Retourne le paramètre σ actuel
     * Return: Valeur de sigma
     */
    float GetSigma() const { return mSigma; }
    
    /**
     * GetRho - Retourne le paramètre ρ actuel
     * Return: Valeur de rho
     */
    float GetRho() const { return mRho; }
    
    /**
     * GetBeta - Retourne le paramètre β actuel
     * Return: Valeur de beta
     */
    float GetBeta() const { return mBeta; }
    
    // === Setters pour les paramètres ===
    
    /**
     * SetSigma - Modifie le paramètre σ
     * @sigma: Nouvelle valeur (recommandé : 5-15)
     * 
     * Modifie le taux de convection du système.
     * Valeurs typiques : 10.0
     */
    void SetSigma(float sigma) { mSigma = sigma; }
    
    /**
     * SetRho - Modifie le paramètre ρ
     * @rho: Nouvelle valeur (recommandé : 20-35)
     * 
     * Modifie la différence de température.
     * Valeurs typiques : 28.0
     * 
     * Note : Pour ρ < 1, le système converge vers un point fixe (pas chaotique)
     *        Pour ρ ≈ 24.74, bifurcation vers le chaos
     *        Pour ρ > 30, comportement devient plus erratique
     */
    void SetRho(float rho) { mRho = rho; }
    
    /**
     * SetBeta - Modifie le paramètre β
     * @beta: Nouvelle valeur (recommandé : 1-4)
     * 
     * Modifie la dimension physique du système.
     * Valeur classique : 8/3 ≈ 2.6667
     */
    void SetBeta(float beta) { mBeta = beta; }
    
    /**
     * SetParameters - Modifie tous les paramètres à la fois
     * @sigma: Nouveau σ
     * @rho: Nouveau ρ
     * @beta: Nouveau β
     * 
     * Utile pour l'interface ImGui (un seul appel au lieu de 3)
     */
    void SetParameters(float sigma, float rho, float beta);

protected:
    /**
     * ComputeDerivative - Calcule la dérivée selon les équations de Lorenz
     * @state: État actuel (x, y, z)
     * @outDerivative: Sortie contenant (dx/dt, dy/dt, dz/dt)
     * 
     * Implémente les équations :
     *   dx/dt = σ(y - x)
     *   dy/dt = x(ρ - z) - y
     *   dz/dt = xy - βz
     */
    void ComputeDerivative(const glm::vec3& state, 
                          glm::vec3& outDerivative) const override;

private:
    // === Paramètres de l'attracteur de Lorenz ===
    float mSigma;  // σ : Paramètre de convection (typiquement 10.0)
    float mRho;    // ρ : Paramètre de température (typiquement 28.0)
    float mBeta;   // β : Paramètre de dimension (typiquement 8/3)
};

#endif // LORENZ_ATTRACTOR_H