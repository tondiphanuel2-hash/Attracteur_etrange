/**
 * Renderer.h - Système de rendu encapsulant SDL
 * 
 * Cette classe encapsule toutes les opérations de dessin SDL
 * pour simplifier le code et faciliter les modifications futures.
 * 
 * Elle fournit des méthodes de haut niveau pour dessiner :
 * - Points (particules)
 * - Lignes (trails entre particules)
 * - Formes géométriques (cercles, rectangles)
 * 
 * Auteur : [VOTRE NOM]
 * Date : Décembre 2025
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>
#include <glm/glm.hpp>  // Pour les vecteurs 2D et 3D
#include <vector>

/**
 * struct Color - Représente une couleur RGBA
 * 
 * Utilise des valeurs entre 0 et 255 pour chaque composante.
 * 
 * @r: Rouge (0-255)
 * @g: Vert (0-255)
 * @b: Bleu (0-255)
 * @a: Alpha/Transparence (0=transparent, 255=opaque)
 */
struct Color
{
    Uint8 r;  // Red
    Uint8 g;  // Green
    Uint8 b;  // Blue
    Uint8 a;  // Alpha
    
    // Constructeur par défaut : blanc opaque
    Color() : r(255), g(255), b(255), a(255) {}
    
    // Constructeur avec paramètres
    Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}
    
    // Couleurs prédéfinies (static pour être partagées)
    static const Color WHITE;
    static const Color BLACK;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color YELLOW;
    static const Color CYAN;
    static const Color MAGENTA;
};

/**
 * class Renderer - Encapsule les opérations de rendu SDL
 * 
 * Cette classe ne possède PAS le SDL_Renderer (non-owning)
 * Elle reçoit un pointeur vers un renderer existant et l'utilise
 * pour dessiner.
 */
class Renderer
{
public:
    /**
     * Renderer - Constructeur
     * @sdlRenderer: Pointeur vers un SDL_Renderer existant
     * 
     * Stocke la référence au renderer SDL pour l'utiliser
     * dans toutes les méthodes de dessin.
     */
    explicit Renderer(SDL_Renderer* sdlRenderer);
    
    /**
     * ~Renderer - Destructeur
     * 
     * Ne détruit PAS le SDL_Renderer (il appartient à Game)
     * Libère seulement les ressources propres au Renderer
     */
    ~Renderer();
    
    /**
     * Clear - Efface l'écran avec une couleur
     * @color: Couleur de fond
     * 
     * Remplit tout l'écran avec la couleur spécifiée.
     */
    void Clear(const Color& color);
    
    /**
     * DrawPoint - Dessine un point à l'écran
     * @position: Position (x, y) du point
     * @color: Couleur du point
     * @size: Taille du point en pixels (1 = 1 pixel)
     * 
     * Pour size > 1, dessine un carré centré sur la position.
     */
    void DrawPoint(const glm::vec2& position, const Color& color, int size = 1);
    
    /**
     * DrawLine - Dessine une ligne entre deux points
     * @start: Point de départ
     * @end: Point d'arrivée
     * @color: Couleur de la ligne
     * 
     * Trace une ligne droite avec anti-aliasing si disponible.
     */
    void DrawLine(const glm::vec2& start, const glm::vec2& end, const Color& color);
    
    /**
     * DrawLines - Dessine plusieurs lignes connectées
     * @points: Liste de points à connecter
     * @color: Couleur des lignes
     * 
     * Connecte tous les points dans l'ordre pour former un trail.
     * Optimisé pour dessiner de nombreux points rapidement.
     */
    void DrawLines(const std::vector<glm::vec2>& points, const Color& color);
    
    /**
     * DrawCircle - Dessine un cercle
     * @center: Centre du cercle
     * @radius: Rayon en pixels
     * @color: Couleur du contour
     * @filled: true pour remplir, false pour contour seulement
     * 
     * Utilise l'algorithme de Bresenham pour un rendu rapide.
     */
    void DrawCircle(const glm::vec2& center, float radius, 
                    const Color& color, bool filled = false);
    
    /**
     * DrawRectangle - Dessine un rectangle
     * @position: Coin supérieur gauche
     * @width: Largeur
     * @height: Hauteur
     * @color: Couleur
     * @filled: true pour remplir, false pour contour seulement
     */
    void DrawRectangle(const glm::vec2& position, float width, float height,
                      const Color& color, bool filled = false);
    
    /**
     * GetWindowSize - Retourne la taille actuelle de la fenêtre
     * @outWidth: Référence pour stocker la largeur
     * @outHeight: Référence pour stocker la hauteur
     */
    void GetWindowSize(int& outWidth, int& outHeight) const;
    
    /**
     * SetDrawColor - Définit la couleur courante de dessin
     * @color: Couleur à utiliser
     * 
     * Toutes les opérations de dessin SDL suivantes utiliseront cette couleur.
     */
    void SetDrawColor(const Color& color);

private:
    SDL_Renderer* mSDLRenderer;  // Pointeur non-owning vers le renderer SDL
    
    /**
     * DrawCircleBresenham - Algorithme de Bresenham pour cercles
     * @centerX: Coordonnée X du centre
     * @centerY: Coordonnée Y du centre
     * @radius: Rayon du cercle
     * 
     * Méthode privée utilisée par DrawCircle.
     * Dessine les 8 octants du cercle simultanément pour l'efficacité.
     */
    void DrawCircleBresenham(int centerX, int centerY, int radius);
};

#endif // RENDERER_H