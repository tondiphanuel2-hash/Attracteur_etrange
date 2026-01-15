/**
 * Colors.cpp - Implémentation du système de coloration
 * 
 * Fonctions pour calculer des couleurs dynamiques.
 */

#include "Colors.h"
#include <cmath>

namespace ColorUtils
{
    /**
     * HsvToRgb - Convertit HSV en RGB
     * 
     * Algorithme standard de conversion HSV → RGB.
     */
    glm::vec3 HsvToRgb(float h, float s, float v)
    {
        // Normaliser h dans [0, 360)
        while (h < 0.0f) h += 360.0f;
        while (h >= 360.0f) h -= 360.0f;
        
        float c = v * s;  // Chroma
        float x = c * (1.0f - std::abs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
        float m = v - c;
        
        float r, g, b;
        
        if (h < 60.0f)
        {
            r = c; g = x; b = 0.0f;
        }
        else if (h < 120.0f)
        {
            r = x; g = c; b = 0.0f;
        }
        else if (h < 180.0f)
        {
            r = 0.0f; g = c; b = x;
        }
        else if (h < 240.0f)
        {
            r = 0.0f; g = x; b = c;
        }
        else if (h < 300.0f)
        {
            r = x; g = 0.0f; b = c;
        }
        else
        {
            r = c; g = 0.0f; b = x;
        }
        
        return glm::vec3(r + m, g + m, b + m);
    }
    
    /**
     * GetColorFromVelocity - Couleur selon vitesse (dégradé thermique)
     */
    glm::vec3 GetColorFromVelocity(const glm::vec3& velocity, 
                                   float minSpeed, 
                                   float maxSpeed)
    {
        // Calculer la magnitude de la vitesse
        float speed = glm::length(velocity);
        
        // Normaliser entre 0 et 1
        float t = (speed - minSpeed) / (maxSpeed - minSpeed);
        t = std::clamp(t, 0.0f, 1.0f);
        
        // Créer un dégradé thermique :
        // Bleu (240°) → Cyan (180°) → Vert (120°) → Jaune (60°) → Rouge (0°)
        float hue = 240.0f * (1.0f - t);  // 240° à 0°
        
        return HsvToRgb(hue, 1.0f, 1.0f);
    }
    
    /**
     * GetColorFromPosition - Couleur selon position dans l'espace
     */
    glm::vec3 GetColorFromPosition(const glm::vec3& position, float bounds)
    {
        // Normaliser chaque composante entre 0 et 1
        float r = (position.x + bounds) / (2.0f * bounds);
        float g = (position.y + bounds) / (2.0f * bounds);
        float b = (position.z + bounds) / (2.0f * bounds);
        
        // Clamper pour éviter les dépassements
        r = std::clamp(r, 0.0f, 1.0f);
        g = std::clamp(g, 0.0f, 1.0f);
        b = std::clamp(b, 0.0f, 1.0f);
        
        return glm::vec3(r, g, b);
    }
    
    /**
     * GetColorFromTime - Couleur qui cycle avec le temps
     */
    glm::vec3 GetColorFromTime(float time, float period)
    {
        // Calculer la position dans le cycle (0 à 1)
        float t = std::fmod(time, period) / period;
        
        // Convertir en teinte (0 à 360°)
        float hue = t * 360.0f;
        
        return HsvToRgb(hue, 1.0f, 1.0f);
    }
    
    /**
     * Lerp - Interpolation linéaire entre deux couleurs
     */
    glm::vec3 Lerp(const glm::vec3& colorA, const glm::vec3& colorB, float t)
    {
        t = std::clamp(t, 0.0f, 1.0f);
        return colorA * (1.0f - t) + colorB * t;
    }
    
    /**
     * GetRainbowColor - Arc-en-ciel de 0 à 1
     */
    glm::vec3 GetRainbowColor(float t)
    {
        t = std::clamp(t, 0.0f, 1.0f);
        
        // Convertir t en teinte (0 à 360°)
        float hue = t * 360.0f;
        
        return HsvToRgb(hue, 1.0f, 1.0f);
    }
}