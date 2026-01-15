/**
 * Screenshot.cpp - Implémentation de la capture d'écran
 * 
 * Sauvegarde le rendu actuel en fichier image.
 */

#include "Screenshot.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>

namespace Screenshot
{
    /**
     * GenerateFilename - Génère un nom avec timestamp
     */
    std::string GenerateFilename(const std::string& prefix,
                                 const std::string& extension)
    {
        // Obtenir le timestamp actuel
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);
        
        // Formater : prefix_YYYY-MM-DD_HH-MM-SS.ext
        std::ostringstream oss;
        oss << prefix << "_"
            << std::setfill('0')
            << std::setw(4) << (localTime->tm_year + 1900) << "-"
            << std::setw(2) << (localTime->tm_mon + 1) << "-"
            << std::setw(2) << localTime->tm_mday << "_"
            << std::setw(2) << localTime->tm_hour << "-"
            << std::setw(2) << localTime->tm_min << "-"
            << std::setw(2) << localTime->tm_sec
            << extension;
        
        return oss.str();
    }
    
    /**
     * Capture - Capture le rendu actuel
     */
    bool Capture(SDL_Renderer* renderer, const std::string& filename)
    {
        if (!renderer)
        {
            std::cerr << "❌ Renderer null, impossible de capturer" << std::endl;
            return false;
        }
        
        // Obtenir les dimensions du rendu
        int width, height;
        if (SDL_GetRenderOutputSize(renderer, &width, &height) != 0)
        {
            std::cerr << "❌ Impossible d'obtenir les dimensions : " 
                      << SDL_GetError() << std::endl;
            return false;
        }
        
        // Créer une surface pour stocker les pixels
        SDL_Surface* surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGB24);
        if (!surface)
        {
            std::cerr << "❌ Impossible de créer la surface : " 
                      << SDL_GetError() << std::endl;
            return false;
        }
        
        // Lire les pixels du renderer
        if (SDL_RenderReadPixels(renderer, nullptr, surface->format->format,
                                 surface->pixels, surface->pitch) != 0)
        {
            std::cerr << "❌ Impossible de lire les pixels : " 
                      << SDL_GetError() << std::endl;
            SDL_DestroySurface(surface);
            return false;
        }
        
        // Générer le nom de fichier si non fourni
        std::string outputFile = filename;
        if (outputFile.empty())
        {
            outputFile = GenerateFilename();
        }
        
        // Créer le dossier screenshots s'il n'existe pas
        std::filesystem::create_directories("screenshots");
        
        // Ajouter le chemin du dossier
        std::string fullPath = "screenshots/" + outputFile;
        
        // Sauvegarder en BMP (format natif SDL)
        // Pour PNG, il faudrait une bibliothèque externe
        // On change juste l'extension pour l'utilisateur
        std::string bmpPath = fullPath;
        size_t dotPos = bmpPath.find_last_of('.');
        if (dotPos != std::string::npos)
        {
            bmpPath = bmpPath.substr(0, dotPos) + ".bmp";
        }
        
        if (SDL_SaveBMP(surface, bmpPath.c_str()) != 0)
        {
            std::cerr << "❌ Impossible de sauvegarder : " 
                      << SDL_GetError() << std::endl;
            SDL_DestroySurface(surface);
            return false;
        }
        
        // Libérer la surface
        SDL_DestroySurface(surface);
        
        std::cout << "📸 Screenshot sauvegardé : " << bmpPath << std::endl;
        return true;
    }
    
    /**
     * SaveSurfaceToPNG - Version simplifiée (BMP seulement)
     */
    bool SaveSurfaceToPNG(SDL_Surface* surface, const std::string& filename)
    {
        if (!surface)
        {
            return false;
        }
        
        // Pour l'instant, on sauvegarde en BMP
        // Une vraie implémentation PNG nécessiterait stb_image_write ou libpng
        return SDL_SaveBMP(surface, filename.c_str()) == 0;
    }
}