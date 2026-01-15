/**
 * Camera3D.cpp - Implémentation du système de caméra 3D
 * 
 * Gère les transformations 3D → 2D et les contrôles utilisateur.
 */

#include "Camera3D.h"
#include <algorithm>  // Pour std::clamp
#include <cmath>      // Pour sin, cos, etc.

// Constante mathématique PI
const float PI = 3.14159265358979323846f;

/**
 * Camera3D - Constructeur
 * @screenWidth: Largeur de l'écran
 * @screenHeight: Hauteur de l'écran
 */
Camera3D::Camera3D(int screenWidth, int screenHeight)
    : mConfig()
    , mScreenWidth(screenWidth)
    , mScreenHeight(screenHeight)
    , mAspectRatio(static_cast<float>(screenWidth) / static_cast<float>(screenHeight))
    , mDistance(mConfig.initialDistance)
    , mTheta(0.0f)      // Commence face à l'avant
    , mPhi(PI / 4.0f)   // Commence à 45° d'élévation
    , mNeedUpdate(true)
{
    // Calculer les matrices initiales
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

/**
 * Update - Met à jour la caméra chaque frame
 * @deltaTime: Temps écoulé
 */
void Camera3D::Update(float deltaTime)
{
    // Pour l'instant, pas d'animations automatiques
    // Cette méthode sera utile en Semaine 4 pour le mode présentation
    
    // Recalculer les matrices si nécessaire
    if (mNeedUpdate)
    {
        UpdateViewMatrix();
        mNeedUpdate = false;
    }
    
    // Éviter le warning "unused parameter"
    (void)deltaTime;
}

/**
 * ProjectToScreen - Projette un point 3D vers l'écran 2D
 * @worldPos: Position dans l'espace monde (x, y, z)
 * 
 * Return: Position sur l'écran en pixels (x, y)
 * 
 * Pipeline de transformation :
 * 1. Monde → Vue (appliquer rotation caméra)
 * 2. Vue → Clip space (appliquer projection perspective)
 * 3. Perspective divide (diviser par w pour obtenir NDC)
 * 4. NDC → Écran (mapper [-1,1] vers [0, width/height])
 */
glm::vec2 Camera3D::ProjectToScreen(const glm::vec3& worldPos) const
{
    // Étape 1 : Transformer en coordonnées vue
    // glm::vec4 car les matrices 4x4 nécessitent des vecteurs homogènes
    glm::vec4 viewPos = mViewMatrix * glm::vec4(worldPos, 1.0f);
    
    // Étape 2 : Appliquer la projection perspective
    glm::vec4 clipPos = mProjectionMatrix * viewPos;
    
    // Étape 3 : Perspective divide (division par w)
    // Cela transforme les coordonnées en NDC (Normalized Device Coordinates)
    // NDC : x, y, z sont tous dans [-1, 1] après cette division
    if (clipPos.w != 0.0f)
    {
        clipPos /= clipPos.w;
    }
    
    // Étape 4 : Convertir NDC en coordonnées écran
    // NDC x: -1 (gauche) → +1 (droite)
    // Écran x: 0 (gauche) → width (droite)
    float screenX = (clipPos.x + 1.0f) * 0.5f * mScreenWidth;
    
    // NDC y: -1 (bas) → +1 (haut)
    // Écran y: height (haut) → 0 (bas) (inversion car SDL a Y vers le bas)
    float screenY = (1.0f - clipPos.y) * 0.5f * mScreenHeight;
    
    return glm::vec2(screenX, screenY);
}

/**
 * Rotate - Fait pivoter la caméra
 * @deltaX: Déplacement horizontal de la souris
 * @deltaY: Déplacement vertical de la souris
 * 
 * Utilise un système de coordonnées sphériques :
 * - Theta (θ) : rotation autour de l'axe Y (gauche/droite)
 * - Phi (φ) : rotation vers le haut/bas
 */
void Camera3D::Rotate(float deltaX, float deltaY)
{
    // Rotation horizontale (azimut)
    // Déplacement souris droite → theta augmente → caméra va vers la droite
    mTheta += deltaX * mConfig.rotationSpeed;
    
    // Normaliser theta dans [0, 2π]
    while (mTheta > 2.0f * PI) mTheta -= 2.0f * PI;
    while (mTheta < 0.0f) mTheta += 2.0f * PI;
    
    // Rotation verticale (élévation)
    // Déplacement souris bas → phi augmente → caméra descend
    mPhi += deltaY * mConfig.rotationSpeed;
    
    // Limiter phi pour éviter le gimbal lock
    // On empêche d'aller exactement au pôle Nord ou Sud
    const float epsilon = 0.01f;
    mPhi = std::clamp(mPhi, epsilon, PI - epsilon);
    
    // Marquer que les matrices doivent être recalculées
    mNeedUpdate = true;
}

/**
 * Zoom - Change la distance de la caméra
 * @delta: +1 pour zoomer, -1 pour dézoomer
 */
void Camera3D::Zoom(float delta)
{
    // Modifier la distance
    mDistance -= delta * mConfig.zoomSpeed;
    
    // Respecter les limites min/max
    mDistance = std::clamp(mDistance, mConfig.minDistance, mConfig.maxDistance);
    
    // Marquer que la matrice de vue doit être recalculée
    mNeedUpdate = true;
}

/**
 * Reset - Réinitialise la caméra à sa position par défaut
 */
void Camera3D::Reset()
{
    mDistance = mConfig.initialDistance;
    mTheta = 0.0f;
    mPhi = PI / 4.0f;  // 45 degrés
    mNeedUpdate = true;
}

/**
 * SetScreenSize - Change les dimensions de l'écran
 * @width: Nouvelle largeur
 * @height: Nouvelle hauteur
 */
void Camera3D::SetScreenSize(int width, int height)
{
    mScreenWidth = width;
    mScreenHeight = height;
    mAspectRatio = static_cast<float>(width) / static_cast<float>(height);
    
    // Recalculer la matrice de projection avec le nouveau ratio
    UpdateProjectionMatrix();
}

/**
 * GetPosition - Calcule la position cartésienne de la caméra
 * 
 * Conversion coordonnées sphériques → cartésiennes :
 * x = r * sin(φ) * cos(θ)
 * y = r * cos(φ)
 * z = r * sin(φ) * sin(θ)
 * 
 * Return: Position (x, y, z) de la caméra
 */
glm::vec3 Camera3D::GetPosition() const
{
    float x = mDistance * std::sin(mPhi) * std::cos(mTheta);
    float y = mDistance * std::cos(mPhi);
    float z = mDistance * std::sin(mPhi) * std::sin(mTheta);
    
    return glm::vec3(x, y, z);
}

/**
 * SetDistance - Définit directement la distance
 * @distance: Nouvelle distance
 */
void Camera3D::SetDistance(float distance)
{
    mDistance = std::clamp(distance, mConfig.minDistance, mConfig.maxDistance);
    mNeedUpdate = true;
}

/**
 * UpdateViewMatrix - Recalcule la matrice de vue
 * 
 * Crée une matrice "lookAt" qui :
 * - Place la caméra à sa position calculée en sphériques
 * - Fait regarder la caméra vers l'origine (0, 0, 0)
 * - Définit le vecteur "up" (vers le haut)
 */
void Camera3D::UpdateViewMatrix()
{
    // Calculer la position de la caméra en cartésien
    glm::vec3 cameraPos = GetPosition();
    
    // Point que la caméra regarde (toujours l'origine pour nous)
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    
    // Vecteur "up" (vers le haut du monde, axe Y positif)
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    // Créer la matrice lookAt
    // Cette fonction GLM crée une matrice de vue qui transforme
    // les coordonnées monde en coordonnées caméra
    mViewMatrix = glm::lookAt(cameraPos, target, up);
}

/**
 * UpdateProjectionMatrix - Recalcule la matrice de projection
 * 
 * Crée une matrice de projection perspective qui simule
 * la façon dont nos yeux voient le monde 3D.
 * 
 * Les objets éloignés apparaissent plus petits (perspective).
 */
void Camera3D::UpdateProjectionMatrix()
{
    // Convertir FOV de degrés en radians
    float fovRadians = glm::radians(mConfig.fieldOfView);
    
    // Créer la matrice de projection perspective
    // Paramètres :
    // - FOV vertical en radians
    // - Ratio d'aspect (largeur/hauteur)
    // - Near plane (objets plus proches ne sont pas dessinés)
    // - Far plane (objets plus loin ne sont pas dessinés)
    mProjectionMatrix = glm::perspective(
        fovRadians,
        mAspectRatio,
        mConfig.nearPlane,
        mConfig.farPlane
    );
}