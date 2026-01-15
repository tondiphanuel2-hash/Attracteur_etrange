/**
 * Math.h - Fonctions mathématiques utilitaires
 * 
 * Constantes et fonctions supplémentaires pour ChaosSim
 */

#ifndef MATH_H
#define MATH_H

#include <cmath>

namespace MathUtils
{
    // === Constantes mathématiques ===
    const float PI = 3.14159265358979323846f;
    const float TWO_PI = 2.0f * PI;
    const float HALF_PI = PI / 2.0f;
    const float E = 2.71828182845904523536f;
    const float GOLDEN_RATIO = 1.61803398874989484820f;
    
    // === Conversion degrés/radians ===
    inline float DegreesToRadians(float degrees)
    {
        return degrees * (PI / 180.0f);
    }
    
    inline float RadiansToDegrees(float radians)
    {
        return radians * (180.0f / PI);
    }
    
    // === Fonctions d'interpolation ===
    inline float Lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }
    
    inline float SmoothStep(float t)
    {
        return t * t * (3.0f - 2.0f * t);
    }
    
    inline float SmootherStep(float t)
    {
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }
    
    // === Clamping ===
    inline float Clamp(float value, float min, float max)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
    
    // === Mapping de plages ===
    inline float Map(float value, float inMin, float inMax, float outMin, float outMax)
    {
        return outMin + (value - inMin) * (outMax - outMin) / (inMax - inMin);
    }
}

#endif // MATH_H

/*
 * Colors.h - Fonctions utilitaires pour la gestion des couleurs
 * 
 * Fournit des fonctions pour :
 * - Convertir entre espaces de couleur (HSV ↔ RGB)
 * - Générer des couleurs basées sur la vélocité ou la position
 * - Créer des dégradés de couleurs
 */