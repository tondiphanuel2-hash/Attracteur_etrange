/**
 * LorenzAttractor.cpp - Implémentation de l'attracteur de Lorenz
 * 
 * Contient les équations différentielles de Lorenz.
 */

#include "LorenzAttractor.h"

// === Constantes pour les valeurs classiques ===
const float DEFAULT_SIGMA = 10.0f;
const float DEFAULT_RHO = 28.0f;
const float DEFAULT_BETA = 8.0f / 3.0f;  // 2.6667

/**
 * LorenzAttractor - Constructeur par défaut
 * 
 * Initialise avec les paramètres classiques qui produisent
 * le comportement chaotique caractéristique.
 */
LorenzAttractor::LorenzAttractor()
    : AttractorSystem()          // Appel du constructeur de la classe de base
    , mSigma(DEFAULT_SIGMA)      // σ = 10.0
    , mRho(DEFAULT_RHO)          // ρ = 28.0
    , mBeta(DEFAULT_BETA)        // β = 8/3
{
    // Rien d'autre à initialiser
    // La classe de base a déjà configuré mCurrentState et mInitialState
}

/**
 * LorenzAttractor - Constructeur avec paramètres personnalisés
 * @sigma: Paramètre σ
 * @rho: Paramètre ρ
 * @beta: Paramètre β
 */
LorenzAttractor::LorenzAttractor(float sigma, float rho, float beta)
    : AttractorSystem()
    , mSigma(sigma)
    , mRho(rho)
    , mBeta(beta)
{
    // Constructeur permettant d'explorer différents paramètres
}

/**
 * GetName - Retourne le nom de l'attracteur
 * 
 * Return: "Lorenz"
 */
std::string LorenzAttractor::GetName() const
{
    return "Lorenz";
}

/**
 * SetParameters - Modifie tous les paramètres simultanément
 * @sigma: Nouveau σ
 * @rho: Nouveau ρ
 * @beta: Nouveau β
 */
void LorenzAttractor::SetParameters(float sigma, float rho, float beta)
{
    mSigma = sigma;
    mRho = rho;
    mBeta = beta;
}

/**
 * ComputeDerivative - Calcule la dérivée selon les équations de Lorenz
 * @state: État actuel (x, y, z) du système
 * @outDerivative: Sortie contenant (dx/dt, dy/dt, dz/dt)
 * 
 * Cette méthode implémente les équations différentielles de Lorenz :
 * 
 *   dx/dt = σ(y - x)
 *   dy/dt = x(ρ - z) - y
 *   dz/dt = xy - βz
 * 
 * Explication physique (modèle météorologique simplifié) :
 * 
 * - x représente l'intensité de la convection
 * - y représente la différence de température horizontale
 * - z représente la différence de température verticale
 * 
 * La première équation (dx/dt) dit que le taux de changement de
 * la convection dépend de la différence de température horizontale.
 * 
 * La deuxième équation (dy/dt) combine l'effet de la convection
 * et de la température verticale.
 * 
 * La troisième équation (dz/dt) décrit comment la température
 * verticale est affectée par la convection et elle-même.
 */
void LorenzAttractor::ComputeDerivative(const glm::vec3& state, 
                                       glm::vec3& outDerivative) const
{
    // Extraire les composantes du vecteur d'état
    float x = state.x;
    float y = state.y;
    float z = state.z;
    
    // Calculer chaque composante de la dérivée
    
    // dx/dt = σ(y - x)
    // Le taux de changement de x est proportionnel à la différence entre y et x
    // Si y > x, alors dx/dt > 0 et x augmente
    // Si y < x, alors dx/dt < 0 et x diminue
    outDerivative.x = mSigma * (y - x);
    
    // dy/dt = x(ρ - z) - y
    // Cette équation est non-linéaire à cause du terme x*z
    // C'est cette non-linéarité qui crée le chaos
    outDerivative.y = x * (mRho - z) - y;
    
    // dz/dt = xy - βz
    // Encore une non-linéarité avec le terme xy
    // Le terme -βz agit comme un amortissement
    outDerivative.z = x * y - mBeta * z;
    
    // Note : Ces équations semblent simples, mais leur combinaison
    // produit un comportement extrêmement complexe et chaotique !
    // 
    // Propriétés remarquables :
    // - Déterministe : les mêmes conditions initiales donnent toujours le même résultat
    // - Chaotique : deux trajectoires très proches divergent exponentiellement
    // - Attracteur : toutes les trajectoires convergent vers une structure en "papillon"
    // - Dimension fractale : environ 2.06 (ni 2D ni 3D, mais entre les deux !)
}