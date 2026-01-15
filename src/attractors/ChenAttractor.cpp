/**
 * ChenAttractor.cpp - Implémentation de l'attracteur de Chen
 * 
 * Contient les équations différentielles de Chen.
 */

#include "ChenAttractor.h"

// === Constantes pour les valeurs classiques ===
const float DEFAULT_A = 35.0f;
const float DEFAULT_B = 3.0f;
const float DEFAULT_C = 28.0f;

/**
 * ChenAttractor - Constructeur par défaut
 */
ChenAttractor::ChenAttractor()
    : AttractorSystem()
    , mA(DEFAULT_A)
    , mB(DEFAULT_B)
    , mC(DEFAULT_C)
{
    // Chen fonctionne bien avec une position initiale proche de celle de Lorenz
    SetInitialState(glm::vec3(-0.1f, 0.5f, -0.6f));
    Reset();
}

/**
 * ChenAttractor - Constructeur avec paramètres personnalisés
 * @a: Paramètre a
 * @b: Paramètre b
 * @c: Paramètre c
 */
ChenAttractor::ChenAttractor(float a, float b, float c)
    : AttractorSystem()
    , mA(a)
    , mB(b)
    , mC(c)
{
    SetInitialState(glm::vec3(-0.1f, 0.5f, -0.6f));
    Reset();
}

/**
 * GetName - Retourne le nom de l'attracteur
 * 
 * Return: "Chen"
 */
std::string ChenAttractor::GetName() const
{
    return "Chen";
}

/**
 * SetParameters - Modifie tous les paramètres simultanément
 * @a: Nouveau a
 * @b: Nouveau b
 * @c: Nouveau c
 */
void ChenAttractor::SetParameters(float a, float b, float c)
{
    mA = a;
    mB = b;
    mC = c;
}

/**
 * ComputeDerivative - Calcule la dérivée selon les équations de Chen
 * @state: État actuel (x, y, z) du système
 * @outDerivative: Sortie contenant (dx/dt, dy/dt, dz/dt)
 * 
 * Équations de Chen :
 * 
 *   dx/dt = a(y - x)
 *   dy/dt = (c - a)x - xz + cy
 *   dz/dt = xy - bz
 * 
 * Relation avec Lorenz :
 * 
 * Chen et Lorenz font partie de la même "famille unifiée" d'attracteurs.
 * En fait, il existe un continuum d'attracteurs entre Lorenz et Chen,
 * paramétré par une variable α.
 * 
 * Pour α = 0 : on obtient Lorenz
 * Pour α = 1 : on obtient Chen
 * Pour 0 < α < 1 : on obtient l'attracteur de Lü (intermédiaire)
 * 
 * Comparaison avec Lorenz :
 * - La première équation est identique : a(y - x)
 * - La deuxième équation diffère par le terme (c - a)x au lieu de x(ρ - z)
 * - La troisième équation est identique : xy - bz
 * 
 * Géométrie de l'attracteur :
 * Chen forme une structure en double hélice, avec deux lobes
 * entrelacés qui ressemblent à de l'ADN. La trajectoire tourne
 * autour d'un lobe, puis saute vers l'autre de manière chaotique.
 * 
 * Contrairement à Lorenz (papillon) et Rössler (ruban), Chen a
 * une topologie unique qui le rend visuellement distinct.
 */
void ChenAttractor::ComputeDerivative(const glm::vec3& state, 
                                     glm::vec3& outDerivative) const
{
    // Extraire les composantes du vecteur d'état
    float x = state.x;
    float y = state.y;
    float z = state.z;
    
    // Calculer chaque composante de la dérivée
    
    // dx/dt = a(y - x)
    // Identique à Lorenz (avec a au lieu de σ)
    // Cette équation crée un couplage linéaire entre x et y
    outDerivative.x = mA * (y - x);
    
    // dy/dt = (c - a)x - xz + cy
    // Différente de Lorenz !
    // Le terme (c - a)x remplace x(ρ - z) de Lorenz
    // Cette modification change la topologie de l'attracteur
    // Le terme -xz est la non-linéarité (produit de deux variables)
    // Le terme cy ajoute un feedback positif sur y
    outDerivative.y = (mC - mA) * x - x * z + mC * y;
    
    // dz/dt = xy - bz
    // Identique à Lorenz (avec b au lieu de β)
    // Le terme xy est non-linéaire (produit)
    // Le terme -bz est un amortissement
    outDerivative.z = x * y - mB * z;
    
    // Note : Chen démontre qu'on peut obtenir des attracteurs
    // avec des topologies très différentes en modifiant légèrement
    // les équations de Lorenz. C'est un exemple de la richesse
    // des systèmes dynamiques chaotiques.
    // 
    // Propriétés remarquables :
    // - Dimension fractale : environ 2.1
    // - Exposant de Lyapunov positif (signature du chaos)
    // - Topologie différente de Lorenz et Rössler
    // - Membre de la famille unifiée d'attracteurs
}