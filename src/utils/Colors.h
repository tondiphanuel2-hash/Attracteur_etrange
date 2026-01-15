/**
 * Colors.h - Système de coloration dynamique des particules
 * 
 * Ce fichier contient des fonctions utilitaires pour :
 * - Convertir HSV → RGB (pour dégradés arc-en-ciel)
 * - Calculer des couleurs basées sur la vélocité
 * - Calculer des couleurs basées sur la position
 * - Calculer des couleurs basées sur le temps
 * - Interpoler entre couleurs
 * 
 * Auteur : [Tondi]
 * Date : Décembre 2025
 */

#ifndef COLORS_H
#define COLORS_H
#include <glm/glm.hpp>

#include <algorithm>

/**
 * namespace ColorUtils - Utilitaires de couleur
 */
namespace ColorUtils
{
    /**
     * HsvToRgb - Convertit une couleur HSV en RGB
     * @h: Teinte (Hue) entre 0 et 360 degrés
     * @s: Saturation entre 0 et 1
     * @v: Valeur (Value/Brightness) entre 0 et 1
     * 
     * Return: Couleur RGB avec composantes entre 0 et 1
     * 
     * HSV est plus intuitif pour créer des dégradés :
     * - H = teinte (rouge → jaune → vert → cyan → bleu → magenta)
     * - S = saturation (gris → couleur pure)
     * - V = luminosité (noir → couleur)
     */
    glm::vec3 HsvToRgb(float h, float s, float v);
    
    /**
     * GetColorFromVelocity - Couleur basée sur la vélocité
     * @velocity: Vecteur vitesse 3D
     * @minSpeed: Vitesse minimale (mappée à bleu/froid)
     * @maxSpeed: Vitesse maximale (mappée à rouge/chaud)
     * 
     * Return: Couleur RGB entre 0 et 1
     * 
     * Crée un dégradé thermique :
     * - Vitesse faible = Bleu (froid)
     * - Vitesse moyenne = Vert/Jaune
     * - Vitesse élevée = Rouge (chaud)
     */
    glm::vec3 GetColorFromVelocity(const glm::vec3& velocity, 
                                   float minSpeed = 0.0f, 
                                   float maxSpeed = 50.0f);
    
    /**
     * GetColorFromPosition - Couleur basée sur la position 3D
     * @position: Position dans l'espace
     * @bounds: Limites de l'espace (pour normalisation)
     * 
     * Return: Couleur RGB entre 0 et 1
     * 
     * Mappe chaque axe à une composante de couleur :
     * - X → Rouge
     * - Y → Vert
     * - Z → Bleu
     */
    glm::vec3 GetColorFromPosition(const glm::vec3& position,
                                   float bounds = 30.0f);
    
    /**
     * GetColorFromTime - Couleur qui change avec le temps
     * @time: Temps en secondes
     * @period: Période du cycle (en secondes)
     * 
     * Return: Couleur RGB entre 0 et 1
     * 
     * Crée un cycle arc-en-ciel qui se répète :
     * Rouge → Orange → Jaune → Vert → Cyan → Bleu → Magenta → Rouge
     */
    glm::vec3 GetColorFromTime(float time, float period = 10.0f);
    
    /**
     * Lerp - Interpolation linéaire entre deux couleurs
     * @colorA: Couleur de départ
     * @colorB: Couleur d'arrivée
     * @t: Facteur d'interpolation (0 = A, 1 = B)
     * 
     * Return: Couleur interpolée
     */
    glm::vec3 Lerp(const glm::vec3& colorA, const glm::vec3& colorB, float t);
    
    /**
     * GetRainbowColor - Couleur arc-en-ciel de 0 à 1
     * @t: Position dans l'arc-en-ciel (0 à 1)
     * 
     * Return: Couleur RGB
     * 
     * 0.0 → Rouge
     * 0.16 → Orange
     * 0.33 → Jaune
     * 0.5 → Vert
     * 0.66 → Cyan
     * 0.83 → Bleu
     * 1.0 → Magenta
     */
    glm::vec3 GetRainbowColor(float t);
}

#endif // COLORS_H