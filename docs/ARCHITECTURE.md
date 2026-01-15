#  ARCHITECTURE DE CHAOSSIM

**Version :** 1.0  
**Date :** Décembre 2025  
**Auteur :** [TONDI Essono]

---

## 📋 Table des Matières

1. [Vue d'Ensemble](#vue-densemble)
2. [Architecture Logicielle](#architecture-logicielle)
3. [Hiérarchie des Classes](#hiérarchie-des-classes)
4. [Flux de Données](#flux-de-données)
5. [Design Patterns](#design-patterns)
6. [Modules Détaillés](#modules-détaillés)
7. [Pipeline de Rendu](#pipeline-de-rendu)
8. [Gestion Mémoire](#gestion-mémoire)
9. [Performance](#performance)
10. [Extensibilité](#extensibilité)

---

## 1. Vue d'Ensemble

ChaosSim est une application de **visualisation interactive** de systèmes dynamiques chaotiques, construite selon une architecture **modulaire** et **orientée objet**.

### Principes Architecturaux

- ✅ **Séparation des responsabilités** (SRP)
- ✅ **Ouvert/Fermé** (OCP)
- ✅ **Inversion de dépendances** (DIP)
- ✅ **Composition over Inheritance**
- ✅ **MVC Pattern** (Model-View-Controller)

### Technologies Utilisées

| Composant | Technologie | Version | Rôle |
|-----------|-------------|---------|------|
| Langage | C++ | 17 | Logique principale |
| Rendu | SDL3 | 3.x | Graphismes et fenêtrage |
| Interface | ImGui | 1.89+ | GUI interactive |
| Mathématiques | GLM | 0.9.9+ | Calculs vectoriels 3D |
| Build | Python | 3.8+ | Script de compilation |

---

## 2. Architecture Logicielle

### 2.1 Architecture en Couches

```
┌───────────────────────────────────────────────────┐
│              COUCHE PRÉSENTATION                  │
│  (Interface Utilisateur - ImGui + UIController)   │
└───────────────────┬───────────────────────────────┘
                    │
┌───────────────────▼───────────────────────────────┐
│              COUCHE APPLICATION                   │
│        (Game Loop - Orchestration)                │
└───────────────────┬───────────────────────────────┘
                    │
┌───────────────────▼───────────────────────────────┐
│              COUCHE MÉTIER                        │
│  (Attracteurs, Particules, Caméra, Présentation)  │
└───────────────────┬───────────────────────────────┘
                    │
┌───────────────────▼───────────────────────────────┐
│              COUCHE UTILITAIRES                   │
│     (Colors, Screenshot, Math)                    │
└───────────────────┬───────────────────────────────┘
                    │
┌───────────────────▼───────────────────────────────┐
│              COUCHE INFRASTRUCTURE                │
│         (SDL3, ImGui, GLM)                        │
└───────────────────────────────────────────────────┘
```

### 2.2 Composants Principaux

```cpp
main.cpp
  │
  └─► Game (Orchestrateur Central)
       ├─► Renderer (Affichage SDL)
       ├─► Camera3D (Projection 3D)
       ├─► AttractorSystem* (Polymorphisme)
       │    ├─► LorenzAttractor
       │    ├─► RosslerAttractor
       │    └─► ChenAttractor
       ├─► ParticleSystem (Visualisation)
       ├─► UIController (Interface)
       └─► PresentationMode (Démo auto)
```

---

## 3. Hiérarchie des Classes

### 3.1 Diagramme UML Complet

```
┌────────────────────────────────────────┐
│              Game                      │
├────────────────────────────────────────┤
│ - mWindow: SDL_Window*                 │
│ - mSDLRenderer: SDL_Renderer*          │
│ - mRenderer: unique_ptr<Renderer>      │
│ - mCamera: unique_ptr<Camera3D>        │
│ - mAttractor: unique_ptr<Attractor...> │
│ - mParticles: unique_ptr<Particle...>  │
│ - mUIController: unique_ptr<UICont...> │
│ - mPresentationMode: unique_ptr<...>   │
│ - mIsRunning: bool                     │
│ - mDeltaTime: float                    │
│ - mElapsedTime: float                  │
├────────────────────────────────────────┤
│ + Run(): void                          │
│ + HandleEvents(): void                 │
│ + Update(float): void                  │
│ + Render(): void                       │
│ - SwitchAttractor(int): void           │
│ - InitializeSDL(): void                │
│ - InitializeImGui(): void              │
│ - CalculateDeltaTime(): void           │
└────────────────────────────────────────┘
```

### 3.2 Hiérarchie d'Héritage

```
AttractorSystem (Abstract)
├── LorenzAttractor
├── RosslerAttractor
└── ChenAttractor

(Toutes les autres classes sont indépendantes)
```

### 3.3 Relations de Composition

```
Game HAS-A:
├── Renderer (1)
├── Camera3D (1)
├── AttractorSystem* (1, polymorphique)
├── ParticleSystem (1)
│   └── vector<Particle> (n)
├── UIController (1)
└── PresentationMode (1)
```

---

## 4. Flux de Données

### 4.1 Boucle Principale

```
┌─────────────┐
│   Start     │
└──────┬──────┘
       │
       ▼
┌─────────────────────────┐
│  CalculateDeltaTime()   │
└──────┬──────────────────┘
       │
       ▼
┌─────────────────────────┐
│   HandleEvents()        │
│  - Clavier              │
│  - Souris               │
│  - Fenêtre              │
└──────┬──────────────────┘
       │
       ▼
┌─────────────────────────┐
│   Update(deltaTime)     │
│  - Camera3D             │
│  - AttractorSystem      │
│  - ParticleSystem       │
│  - PresentationMode     │
└──────┬──────────────────┘
       │
       ▼
┌─────────────────────────┐
│   Render()              │
│  - Clear screen         │
│  - Draw particles       │
│  - Draw UI              │
│  - Present frame        │
└──────┬──────────────────┘
       │
       ▼
    ┌──────┐
    │Loop? │──Yes──┐
    └──┬───┘       │
       │           │
       No          │
       │           │
       ▼           │
    ┌──────┐      │
    │ End  │◄─────┘
    └──────┘
```

### 4.2 Flux de Mise à Jour

```
AttractorSystem::Update()
    │
    ├─► ComputeDerivative() [Virtual, implémentée par sous-classes]
    │   └─► Calcule (dx/dt, dy/dt, dz/dt)
    │
    ├─► IntegrateRK4()
    │   └─► mCurrentState += ...
    │
    └─► Return nouvelle position

ParticleSystem::Update()
    │
    ├─► Récupère position de l'attracteur
    │
    ├─► Pour chaque Particle:
    │   ├─► particle.position = attractorPos
    │   ├─► particle.velocity = calculée
    │   └─► particle.trail.push_back(position)
    │
    └─► UpdateColor() selon mode
```

### 4.3 Flux de Rendu

```
Particle (3D)
    │
    ├─► Camera3D::ProjectToScreen()
    │   ├─► View Matrix (rotation)
    │   ├─► Projection Matrix (perspective)
    │   └─► NDC → Screen coordinates
    │
    ├─► ColorUtils::GetColor()
    │   └─► Selon mode (velocity/position/time)
    │
    └─► Renderer::DrawLines() / DrawPoint()
        └─► SDL_RenderLines() / SDL_RenderPoint()
```

---

## 5. Design Patterns

### 5.1 Strategy Pattern

**Contexte :** Attracteurs interchangeables

```cpp
// Interface
class AttractorSystem {
    virtual void ComputeDerivative() = 0;
};

// Stratégies concrètes
class LorenzAttractor : public AttractorSystem { ... };
class RosslerAttractor : public AttractorSystem { ... };
class ChenAttractor : public AttractorSystem { ... };

// Utilisation
unique_ptr<AttractorSystem> attractor;
attractor = make_unique<LorenzAttractor>();  // Changement facile
```

**Avantages :**
- ✅ Ajout de nouveaux attracteurs sans modifier le code existant
- ✅ Changement d'attracteur à la volée
- ✅ Respect du principe Open/Closed

### 5.2 MVC Pattern

```
Model:      AttractorSystem, ParticleSystem
            └─► Logique métier, calculs

View:       Renderer, Camera3D
            └─► Affichage, projection

Controller: Game, UIController
            └─► Coordination, événements
```

### 5.3 Facade Pattern

```cpp
// Renderer simplifie l'API complexe de SDL3
class Renderer {
    void DrawPoint(vec2, Color, size);  // Simple
    // Au lieu de:
    // SDL_SetRenderDrawColor(...)
    // SDL_RenderFillRect(...)
    // etc.
};
```

### 5.4 Observer Pattern (Implicite)

```cpp
// UIController observe les changements
if (button_clicked) {
    mRequestedAttractorIndex = 2;  // Notifie Game
}

// Game réagit
if (ui->mRequestedAttractorIndex != -1) {
    SwitchAttractor(ui->mRequestedAttractorIndex);
}
```

### 5.5 Template Method Pattern

```cpp
// AttractorSystem définit le squelette
void Update(float dt) {
    ComputeDerivative();  // Appel polymorphique
    IntegrateRK4();
}

// Sous-classes implémentent les détails
void LorenzAttractor::ComputeDerivative() {
    // Équations spécifiques de Lorenz
}
```

---

## 6. Modules Détaillés

### 6.1 Core (Noyau)

**Game.cpp/h**
- Rôle : Orchestration centrale
- Responsabilités :
  - Initialisation SDL/ImGui
  - Boucle principale
  - Gestion événements
  - Coordination sous-systèmes
- Dépendances : Tous les autres modules

**Renderer.cpp/h**
- Rôle : Abstraction SDL
- Responsabilités :
  - Dessiner points/lignes/formes
  - Gérer couleurs
  - Encapsuler SDL_Renderer
- Dépendances : SDL3

**PresentationMode.cpp/h**
- Rôle : Démonstration automatique
- Responsabilités :
  - Animer la caméra
  - Changer d'attracteurs
  - Gérer les phases
- Dépendances : Camera3D, Game

### 6.2 Attractors (Attracteurs)

**AttractorSystem.cpp/h**
- Rôle : Classe abstraite de base
- Responsabilités :
  - Définir l'interface commune
  - Intégration numérique (RK4)
  - Gestion de l'état
- Dépendances : GLM

**LorenzAttractor.cpp/h**
- Équations : σ(y-x), x(ρ-z)-y, xy-βz
- Paramètres : σ=10, ρ=28, β=8/3
- Forme : Papillon double aile

**RosslerAttractor.cpp/h**
- Équations : -y-z, x+ay, b+z(x-c)
- Paramètres : a=0.2, b=0.2, c=5.7
- Forme : Ruban tordu

**ChenAttractor.cpp/h**
- Équations : a(y-x), (c-a)x-xz+cy, xy-bz
- Paramètres : a=35, b=3, c=28
- Forme : Double hélice

### 6.3 Graphics (Graphiques)

**Camera3D.cpp/h**
- Rôle : Caméra virtuelle
- Responsabilités :
  - Coordonnées sphériques (θ, φ, r)
  - Matrices de transformation
  - Projection 3D → 2D
- Algorithmes :
  - lookAt pour view matrix
  - perspective pour projection matrix

**ParticleSystem.cpp/h**
- Rôle : Gestion des particules
- Responsabilités :
  - Stocker trails
  - Calculer couleurs
  - Rendre avec projection
- Optimisations :
  - Pré-allocation mémoire
  - Limitation longueur trails

### 6.4 UI (Interface)

**UIController.cpp/h**
- Rôle : Interface ImGui
- Responsabilités :
  - Fenêtres d'information
  - Contrôles interactifs
  - Sliders paramètres
  - Boutons actions
- État : UIState (pause, colorMode, etc.)

### 6.5 Utils (Utilitaires)

**Colors.cpp/h**
- Fonctions :
  - HsvToRgb()
  - GetColorFromVelocity()
  - GetColorFromPosition()
  - GetColorFromTime()

**Screenshot.cpp/h**
- Fonctions :
  - Capture()
  - SaveSurfaceToPNG()
  - GenerateFilename()

**Math.cpp/h** (Optionnel)
- Fonctions mathématiques supplémentaires
- Constantes (PI, etc.)

---

## 7. Pipeline de Rendu

### 7.1 Séquence Complète

```
1. CLEAR
   SDL_SetRenderDrawColor(0, 0, 0, 255)
   SDL_RenderClear()

2. PROJECTION 3D
   Pour chaque Particle:
       vec3 pos3D = particle.position
       vec2 pos2D = camera->ProjectToScreen(pos3D)

3. COLORATION
   vec3 color = ColorUtils::GetColor(...)
   Color sdlColor = ConvertToSDL(color)

4. DESSIN
   Renderer::DrawLines(trail, color)
   Renderer::DrawPoint(position, color)

5. INTERFACE
   ImGui::NewFrame()
   UIController::Render(...)
   ImGui::Render()

6. PRÉSENTATION
   SDL_RenderPresent()
```

### 7.2 Transformation 3D → 2D

```
Point Monde (x, y, z)
    │
    ▼
View Matrix (rotation caméra)
    │
    ▼
Point Vue (x', y', z')
    │
    ▼
Projection Matrix (perspective)
    │
    ▼
Clip Space (x'', y'', z'', w)
    │
    ▼
Perspective Divide (÷ w)
    │
    ▼
NDC (-1 à +1)
    │
    ▼
Viewport Transform
    │
    ▼
Screen Coordinates (pixels)
```

---

## 8. Gestion Mémoire

### 8.1 Smart Pointers

```cpp
// Possession unique (ownership)
unique_ptr<Renderer> mRenderer;
unique_ptr<Camera3D> mCamera;

// Avantages :
// - Libération automatique
// - Pas de delete manuel
// - Exception-safe
```

### 8.2 Allocation Particules

```cpp
// Pré-allocation pour éviter réallocations
vector<Particle> mParticles;
mParticles.reserve(1000);  // Réserve mémoire

// Trails avec limite
if (trail.size() > MAX_LENGTH) {
    trail.erase(trail.begin(), trail.begin() + 10);
}
```

### 8.3 Éviter les Copies

```cpp
// Passage par référence const
void Render(const Particle& p);

// Move semantics pour transfert
attractor = make_unique<LorenzAttractor>();
```

---

## 9. Performance

### 9.1 Objectifs

- **FPS cible** : 60 FPS constant
- **Particules** : 500-1000 @ 60 FPS
- **Latence input** : < 16ms
- **Mémoire** : < 100 MB

### 9.2 Optimisations Implémentées

**Calculs :**
- RK4 au lieu d'Euler (précision vs vitesse)
- Limitation longueur trails
- Pas de recalculs inutiles

**Rendu :**
- Batch drawing (DrawLines au lieu de multiples DrawLine)
- Culling spatial (futur)
- Dirty flag pour matrices caméra

**Mémoire :**
- Reserve() pour éviter réallocations
- Réutilisation buffers
- Smart pointers pour RAII

### 9.3 Profiling

Zones à surveiller :
1. **ParticleSystem::Render()** (> 50% du temps)
2. **AttractorSystem::IntegrateRK4()** (> 20% du temps)
3. **Camera3D::ProjectToScreen()** (> 15% du temps)

---

## 10. Extensibilité

### 10.1 Ajouter un Nouvel Attracteur

```cpp
// 1. Créer header
class MyAttractor : public AttractorSystem {
    void ComputeDerivative(...) override;
};

// 2. Implémenter
void MyAttractor::ComputeDerivative(...) {
    // Vos équations ici
}

// 3. Intégrer dans Game
case 3:
    mAttractor = make_unique<MyAttractor>();
    break;
```

### 10.2 Ajouter un Mode de Coloration

```cpp
// 1. Ajouter dans Colors.h
vec3 GetColorFromCustom(...);

// 2. Ajouter enum
enum ColorMode {
    ...,
    COLOR_MODE_CUSTOM
};

// 3. Case dans UpdateColor()
```

### 10.3 Points d'Extension Faciles

 Nouveaux attracteurs (héritage)  
 Nouveaux modes couleur (fonction)  
  Nouvelles phases présentation  
  Exports (vidéo, données)  
  Shaders personnalisés (remplacer Renderer)

---

##  Métriques du Projet

| Métrique | Valeur |
|----------|--------|
| Lignes de code | ~3500 |
| Fichiers source | ~25 |
| Classes | 12 |
| Fonctions | ~80 |
| Complexité moyenne | 8-12 |
| Dépendances | 3 (SDL3, ImGui, GLM) |

---

**Cette architecture assure :**
  Modularité  
  Extensibilité  
  Maintenabilité  
  Performance  
  Testabilité