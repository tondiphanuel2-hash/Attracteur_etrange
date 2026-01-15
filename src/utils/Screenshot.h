/**
 * Screenshot.h - Utilitaire de capture d'écran
 * 
 * Permet de sauvegarder le rendu actuel dans un fichier PNG.
 * Utile pour :
 * - Documenter le projet
 * - Créer des présentations PowerPoint
 * - Portfolio personnel
 * 
 * Auteur : [VOTRE NOM]
 * Date : Décembre 2025
 */

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <SDL3/SDL.h>
#include <string>

/**
 * namespace Screenshot - Fonctions de capture d'écran
 */
namespace Screenshot
{
    /**
     * Capture - Capture le rendu actuel et sauvegarde en PNG
     * @renderer: Renderer SDL à capturer
     * @filename: Nom du fichier (optionnel, auto-généré si vide)
     * 
     * Return: true si succès, false si échec
     * 
     * Le nom de fichier auto-généré suit le format :
     * chaossim_YYYY-MM-DD_HH-MM-SS.png
     * 
     * Exemple : chaossim_2025-12-16_14-30-45.png
     */
    bool Capture(SDL_Renderer* renderer, const std::string& filename = "");
    
    /**
     * GenerateFilename - Génère un nom de fichier avec timestamp
     * @prefix: Préfixe du nom (par défaut "chaossim")
     * @extension: Extension (par défaut ".png")
     * 
     * Return: Nom de fichier complet
     */
    std::string GenerateFilename(const std::string& prefix = "chaossim",
                                 const std::string& extension = ".png");
    
    /**
     * SaveSurfaceToPNG - Sauvegarde une surface SDL en PNG
     * @surface: Surface à sauvegarder
     * @filename: Nom du fichier
     * 
     * Return: true si succès
     * 
     * Note : Cette fonction est simplifiée et utilise SDL_SaveBMP
     * puis conversion BMP→PNG si possible. Pour un vrai PNG,
     * il faudrait intégrer une bibliothèque comme stb_image_write.
     */
    bool SaveSurfaceToPNG(SDL_Surface* surface, const std::string& filename);
}

#endif // SCREENSHOT_H