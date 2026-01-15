/**
 * ChenAttractor.h - Implémentation de l'attracteur de Chen
 * 
 * L'attracteur de Chen est un système chaotique découvert par
 * Guanrong Chen et Tetsushi Ueta en 1999. C'est un membre de la
 * famille unifiée des attracteurs chaotiques qui inclut Lorenz et Lü.
 * 
 * Équations :
 *   dx/dt = a(y - x)
 *   dy/dt = (c - a)x - xz + cy
 *   dz/dt = xy - bz
 * 
 * Où :
 *   a = paramètre de couplage
 *   b = paramètre d'amortissement
 *   c = paramètre de température/bifurcation
 * 
 * Valeurs classiques pour comportement chaotique :
 *   a = 35.0
 *   b = 3.0
 *   c = 28.0
 * 
 * L'attracteur de Chen forme une structure à "double hélice" ou
 * "double spirale" qui ressemble à de l'ADN entrelacé.
 * 
 * Auteur : [VOTRE NOM]
 * Date : Décembre 2025
 * Référence : Chen, G. & Ueta, T. (1999). "Yet another chaotic attractor"
 */

#ifndef CHEN_ATTRACTOR_H
#define CHEN_ATTRACTOR_H

#include "AttractorSystem.h"

/**
 * class ChenAttractor - Attracteur de Chen
 * 
 * Hérite de AttractorSystem et implémente les équations
 * spécifiques de Chen.
 */
class ChenAttractor : public AttractorSystem
{
public:
    /**
     * ChenAttractor - Constructeur avec paramètres par défaut
     * 
     * Initialise avec les valeurs classiques qui produisent
     * le comportement chaotique caractéristique en double hélice.
     */
    ChenAttractor();
    
    /**
     * ChenAttractor - Constructeur avec paramètres personnalisés
     * @a: Paramètre a (couplage)
     * @b: Paramètre b (amortissement)
     * @c: Paramètre c (bifurcation)
     * 
     * Permet d'explorer différents comportements.
     */
    ChenAttractor(float a, float b, float c);
    
    /**
     * ~ChenAttractor - Destructeur
     */
    ~ChenAttractor() override = default;
    
    /**
     * GetName - Retourne le nom de l'attracteur
     * 
     * Return: "Chen"
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
     * @a: Nouvelle valeur (recommandé : 30-40)
     * 
     * Contrôle le couplage entre x et y.
     * Valeur classique : 35.0
     */
    void SetA(float a) { mA = a; }
    
    /**
     * SetB - Modifie le paramètre b
     * @b: Nouvelle valeur (recommandé : 2-4)
     * 
     * Contrôle l'amortissement de z.
     * Valeur classique : 3.0
     */
    void SetB(float b) { mB = b; }
    
    /**
     * SetC - Modifie le paramètre c
     * @c: Nouvelle valeur (recommandé : 25-35)
     * 
     * Contrôle le comportement global du système.
     * Valeur classique : 28.0
     * 
     * Note : Chen est étroitement lié à Lorenz
     *        Leurs équations sont des variantes l'une de l'autre
     */
    void SetC(float c) { mC = c; }
    
    /**
     * SetParameters - Modifie tous les paramètres à la fois
     * @a: Nouveau a
     * @b: Nouveau b
     * @c: Nouveau c
     */
    void SetParameters(float a, float b, float c);

protected:
    /**
     * ComputeDerivative - Calcule la dérivée selon les équations de Chen
     * @state: État actuel (x, y, z)
     * @outDerivative: Sortie contenant (dx/dt, dy/dt, dz/dt)
     * 
     * Implémente les équations :
     *   dx/dt = a(y - x)
     *   dy/dt = (c - a)x - xz + cy
     *   dz/dt = xy - bz
     */
    void ComputeDerivative(const glm::vec3& state, 
                          glm::vec3& outDerivative) const override;

private:
    // === Paramètres de l'attracteur de Chen ===
    float mA;  // Paramètre de couplage (typiquement 35.0)
    float mB;  // Paramètre d'amortissement (typiquement 3.0)
    float mC;  // Paramètre de bifurcation (typiquement 28.0)
};

#endif // CHEN_ATTRACTOR_H