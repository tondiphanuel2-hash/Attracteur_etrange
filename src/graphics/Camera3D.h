/**
 * Camera3D.h - Système de caméra virtuelle pour projection 3D
 * 
 * Cette classe gère :
 * - La projection de points 3D vers l'écran 2D
 * - La rotation de la vue (orbite autour de l'origine)
 * - Le zoom (distance à l'origine)
 * - Les matrices de transformation (rotation, projection)
 * 
 * La caméra utilise un système de coordonnées sphériques pour la rotation :
 * - Theta (θ) : rotation horizontale (azimut)
 * - Phi (φ) : rotation verticale (élévation)
 * - Distance : éloignement de l'origine
 * 
 * Auteur : [VOTRE NOM]
 * Date : Décembre 2025
 */

#ifndef CAMERA3D_H
#define CAMERA3D_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * struct CameraConfig - Configuration de la caméra
 * 
 * Regroupe tous les paramètres configurables de la caméra.
 */
struct CameraConfig
{
    float initialDistance = 50.0f;      // Distance initiale à l'origine
    float minDistance = 10.0f;          // Distance minimale (zoom max)
    float maxDistance = 200.0f;         // Distance maximale (zoom min)
    float zoomSpeed = 2.0f;             // Vitesse du zoom (molette souris)
    float rotationSpeed = 0.005f;       // Vitesse de rotation (souris)
    float fieldOfView = 45.0f;          // Champ de vision (FOV) en degrés
    float nearPlane = 0.1f;             // Plan de clipping proche
    float farPlane = 1000.0f;           // Plan de clipping lointain
};

/**
 * class Camera3D - Caméra virtuelle pour visualisation 3D
 * 
 * Gère la transformation des coordonnées 3D (monde) vers 2D (écran).
 * 
 * Pipeline de transformation :
 * 1. Coordonnées monde (x, y, z) de l'attracteur
 * 2. Transformation vue (rotation caméra)
 * 3. Projection perspective
 * 4. Normalisation (NDC : Normalized Device Coordinates)
 * 5. Coordonnées écran (pixels)
 */
class Camera3D
{
public:
    /**
     * Camera3D - Constructeur
     * @screenWidth: Largeur de l'écran en pixels
     * @screenHeight: Hauteur de l'écran en pixels
     * 
     * Initialise la caméra avec les dimensions de l'écran
     * pour calculer le ratio d'aspect (aspect ratio).
     */
    Camera3D(int screenWidth, int screenHeight);
    
    /**
     * ~Camera3D - Destructeur
     */
    ~Camera3D() = default;
    
    /**
     * Update - Met à jour les matrices de la caméra
     * @deltaTime: Temps écoulé (pour animations futures)
     * 
     * Recalcule les matrices de vue et projection si nécessaire.
     */
    void Update(float deltaTime);
    
    /**
     * ProjectToScreen - Projette un point 3D sur l'écran 2D
     * @worldPos: Position 3D dans l'espace monde
     * 
     * Return: Position 2D en coordonnées écran (pixels)
     * 
     * Cette fonction applique toute la chaîne de transformation :
     * Point monde → Vue caméra → Projection → NDC → Écran
     */
    glm::vec2 ProjectToScreen(const glm::vec3& worldPos) const;
    
    /**
     * Rotate - Fait tourner la caméra (drag souris)
     * @deltaX: Déplacement horizontal de la souris
     * @deltaY: Déplacement vertical de la souris
     * 
     * Modifie les angles theta et phi en coordonnées sphériques.
     */
    void Rotate(float deltaX, float deltaY);
    
    /**
     * Zoom - Change la distance de la caméra (molette souris)
     * @delta: Valeur du zoom (+1 = rapprocher, -1 = éloigner)
     * 
     * Modifie mDistance tout en respectant les limites min/max.
     */
    void Zoom(float delta);
    
    /**
     * Reset - Réinitialise la caméra à sa position par défaut
     * 
     * Remet les angles et la distance à leurs valeurs initiales.
     */
    void Reset();
    
    /**
     * SetScreenSize - Met à jour les dimensions de l'écran
     * @width: Nouvelle largeur
     * @height: Nouvelle hauteur
     * 
     * Appelé quand la fenêtre est redimensionnée.
     * Recalcule la matrice de projection avec le nouveau ratio.
     */
    void SetScreenSize(int width, int height);
    
    /**
     * GetPosition - Retourne la position actuelle de la caméra dans le monde
     * 
     * Calcule la position cartésienne (x, y, z) à partir des
     * coordonnées sphériques (distance, theta, phi).
     * 
     * Return: Position 3D de la caméra
     */
    glm::vec3 GetPosition() const;
    
    /**
     * GetViewMatrix - Retourne la matrice de vue
     * 
     * Return: Matrice 4x4 de transformation de vue
     */
    const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
    
    /**
     * GetProjectionMatrix - Retourne la matrice de projection
     * 
     * Return: Matrice 4x4 de projection perspective
     */
    const glm::mat4& GetProjectionMatrix() const { return mProjectionMatrix; }
    
    /**
     * GetDistance - Retourne la distance actuelle à l'origine
     * 
     * Return: Distance en unités monde
     */
    float GetDistance() const { return mDistance; }
    
    /**
     * SetDistance - Définit directement la distance
     * @distance: Nouvelle distance (sera clampée entre min et max)
     */
    void SetDistance(float distance);

private:
    // === Configuration ===
    CameraConfig mConfig;
    
    // === Dimensions de l'écran ===
    int mScreenWidth;
    int mScreenHeight;
    float mAspectRatio;  // Ratio largeur/hauteur
    
    // === Position de la caméra en coordonnées sphériques ===
    float mDistance;     // Distance à l'origine (r dans sphériques)
    float mTheta;        // Angle horizontal/azimut (θ) en radians
    float mPhi;          // Angle vertical/élévation (φ) en radians
    
    // === Matrices de transformation ===
    glm::mat4 mViewMatrix;        // Matrice de vue (monde → caméra)
    glm::mat4 mProjectionMatrix;  // Matrice de projection (caméra → NDC)
    
    // === État ===
    bool mNeedUpdate;    // true si les matrices doivent être recalculées
    
    /**
     * UpdateViewMatrix - Recalcule la matrice de vue
     * 
     * Crée une matrice lookAt qui transforme les coordonnées monde
     * en coordonnées caméra (la caméra regarde toujours l'origine).
     */
    void UpdateViewMatrix();
    
    /**
     * UpdateProjectionMatrix - Recalcule la matrice de projection
     * 
     * Crée une matrice de projection perspective avec le FOV et
     * le ratio d'aspect actuels.
     */
    void UpdateProjectionMatrix();
};

#endif // CAMERA3D_H