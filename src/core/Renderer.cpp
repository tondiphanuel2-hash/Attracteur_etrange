/**
 * Renderer.cpp - Implémentation du système de rendu
 * 
 * Contient toutes les fonctions de dessin utilisant SDL3.
 */

#include "Renderer.h"
#include <cmath>
#include <algorithm>

// === Définition des couleurs statiques ===
const Color Color::WHITE   = Color(255, 255, 255, 255);
const Color Color::BLACK   = Color(0, 0, 0, 255);
const Color Color::RED     = Color(255, 0, 0, 255);
const Color Color::GREEN   = Color(0, 255, 0, 255);
const Color Color::BLUE    = Color(0, 0, 255, 255);
const Color Color::YELLOW  = Color(255, 255, 0, 255);
const Color Color::CYAN    = Color(0, 255, 255, 255);
const Color Color::MAGENTA = Color(255, 0, 255, 255);

/**
 * Renderer - Constructeur
 * @sdlRenderer: Pointeur vers le renderer SDL existant
 */
Renderer::Renderer(SDL_Renderer* sdlRenderer)
    : mSDLRenderer(sdlRenderer)
{
    // Vérifier que le renderer est valide
    if (!mSDLRenderer)
    {
        throw std::runtime_error("Renderer SDL null passé au constructeur Renderer");
    }
}

/**
 * ~Renderer - Destructeur
 * 
 * Rien à faire ici car nous ne possédons pas le SDL_Renderer
 */
Renderer::~Renderer()
{
    // Le SDL_Renderer sera détruit par Game
}

/**
 * Clear - Efface l'écran avec une couleur
 * @color: Couleur de fond
 */
void Renderer::Clear(const Color& color)
{
    SetDrawColor(color);
    SDL_RenderClear(mSDLRenderer);
}

/**
 * SetDrawColor - Définit la couleur courante de dessin SDL
 * @color: Couleur à appliquer
 */
void Renderer::SetDrawColor(const Color& color)
{
    SDL_SetRenderDrawColor(mSDLRenderer, color.r, color.g, color.b, color.a);
}

/**
 * DrawPoint - Dessine un point (ou petit carré) à l'écran
 * @position: Position (x, y)
 * @color: Couleur du point
 * @size: Taille du point (1 = 1 pixel, 2 = carré 2x2, etc.)
 */
void Renderer::DrawPoint(const glm::vec2& position, const Color& color, int size)
{
    SetDrawColor(color);
    
    if (size <= 1)
    {
        // Point d'1 pixel : utiliser SDL_RenderPoint pour l'efficacité
        SDL_RenderPoint(mSDLRenderer, position.x, position.y);
    }
    else
    {
        // Point plus gros : dessiner un rectangle rempli
        // Centrer le rectangle sur la position
        SDL_FRect rect;
        rect.x = position.x - size / 2.0f;
        rect.y = position.y - size / 2.0f;
        rect.w = static_cast<float>(size);
        rect.h = static_cast<float>(size);
        
        SDL_RenderFillRect(mSDLRenderer, &rect);
    }
}

/**
 * DrawLine - Dessine une ligne entre deux points
 * @start: Point de départ
 * @end: Point d'arrivée
 * @color: Couleur de la ligne
 */
void Renderer::DrawLine(const glm::vec2& start, const glm::vec2& end, const Color& color)
{
    SetDrawColor(color);
    SDL_RenderLine(mSDLRenderer, start.x, start.y, end.x, end.y);
}

/**
 * DrawLines - Dessine plusieurs lignes connectées (trail de particules)
 * @points: Liste de points à connecter
 * @color: Couleur des lignes
 * 
 * Cette fonction est optimisée pour dessiner de nombreux points.
 * Elle utilise SDL_RenderLines qui est plus rapide que d'appeler
 * SDL_RenderLine en boucle.
 */
void Renderer::DrawLines(const std::vector<glm::vec2>& points, const Color& color)
{
    // Si moins de 2 points, impossible de dessiner une ligne
    if (points.size() < 2)
    {
        return;
    }
    
    SetDrawColor(color);
    
    // Convertir les glm::vec2 en SDL_FPoint pour SDL_RenderLines
    std::vector<SDL_FPoint> sdlPoints;
    sdlPoints.reserve(points.size());
    
    for (const auto& point : points)
    {
        sdlPoints.push_back(SDL_FPoint{point.x, point.y});
    }
    
    // Dessiner toutes les lignes d'un coup (beaucoup plus rapide)
    SDL_RenderLines(mSDLRenderer, sdlPoints.data(), static_cast<int>(sdlPoints.size()));
}

/**
 * DrawCircle - Dessine un cercle
 * @center: Centre du cercle
 * @radius: Rayon en pixels
 * @color: Couleur
 * @filled: true pour remplir, false pour contour seulement
 */
void Renderer::DrawCircle(const glm::vec2& center, float radius, 
                         const Color& color, bool filled)
{
    SetDrawColor(color);
    
    int centerX = static_cast<int>(center.x);
    int centerY = static_cast<int>(center.y);
    int r = static_cast<int>(radius);
    
    if (filled)
    {
        // Remplir le cercle en dessinant des lignes horizontales
        for (int y = -r; y <= r; y++)
        {
            // Calculer la largeur de la ligne à cette hauteur
            // Utiliser le théorème de Pythagore : x² + y² = r²
            int width = static_cast<int>(std::sqrt(r * r - y * y));
            
            SDL_RenderLine(mSDLRenderer, 
                          centerX - width, centerY + y,
                          centerX + width, centerY + y);
        }
    }
    else
    {
        // Dessiner seulement le contour avec l'algorithme de Bresenham
        DrawCircleBresenham(centerX, centerY, r);
    }
}

/**
 * DrawCircleBresenham - Algorithme de Bresenham pour dessiner un cercle
 * @centerX: Coordonnée X du centre
 * @centerY: Coordonnée Y du centre
 * @radius: Rayon du cercle
 * 
 * Cet algorithme dessine un cercle en calculant seulement 1/8 des points
 * et en utilisant la symétrie pour dessiner les 7 autres octants.
 */
void Renderer::DrawCircleBresenham(int centerX, int centerY, int radius)
{
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;  // Paramètre de décision
    
    // Fonction lambda pour dessiner les 8 points symétriques
    auto drawSymmetricPoints = [&](int x, int y)
    {
        SDL_RenderPoint(mSDLRenderer, centerX + x, centerY + y);
        SDL_RenderPoint(mSDLRenderer, centerX - x, centerY + y);
        SDL_RenderPoint(mSDLRenderer, centerX + x, centerY - y);
        SDL_RenderPoint(mSDLRenderer, centerX - x, centerY - y);
        SDL_RenderPoint(mSDLRenderer, centerX + y, centerY + x);
        SDL_RenderPoint(mSDLRenderer, centerX - y, centerY + x);
        SDL_RenderPoint(mSDLRenderer, centerX + y, centerY - x);
        SDL_RenderPoint(mSDLRenderer, centerX - y, centerY - x);
    };
    
    // Dessiner le cercle
    while (y >= x)
    {
        drawSymmetricPoints(x, y);
        x++;
        
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
    }
}

/**
 * DrawRectangle - Dessine un rectangle
 * @position: Coin supérieur gauche
 * @width: Largeur
 * @height: Hauteur
 * @color: Couleur
 * @filled: true pour remplir, false pour contour seulement
 */
void Renderer::DrawRectangle(const glm::vec2& position, float width, float height,
                            const Color& color, bool filled)
{
    SetDrawColor(color);
    
    SDL_FRect rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.w = width;
    rect.h = height;
    
    if (filled)
    {
        SDL_RenderFillRect(mSDLRenderer, &rect);
    }
    else
    {
        SDL_RenderRect(mSDLRenderer, &rect);
    }
}

/**
 * GetWindowSize - Retourne la taille actuelle de la fenêtre
 * @outWidth: Référence pour stocker la largeur
 * @outHeight: Référence pour stocker la hauteur
 */
void Renderer::GetWindowSize(int& outWidth, int& outHeight) const
{
    // Récupérer la taille de sortie du renderer
    // (qui correspond à la taille de la fenêtre)
    SDL_GetRenderOutputSize(mSDLRenderer, &outWidth, &outHeight);
}