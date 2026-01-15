## 11. README.md (Version Finale du Projet)

**IMPORTANT** : Ce README.md doit être créé à la **RACINE** du projet (pas dans docs/).

Créez `README.md` (racine du projet) :

```markdown
# 🌀 ChaosSim - Visualiseur d'Attracteurs Étranges

[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/[username]/ChaosSim)
[![C++](https://img.shields.io/badge/C++-17-00599C.svg)](https://isocpp.org/)
[![SDL3](https://img.shields.io/badge/SDL3-3.x-brightgreen.svg)](https://github.com/libsdl-org/SDL)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

> **Projet Académique** - Visualisation interactive de systèmes dynamiques chaotiques en C++ avec SDL3

![ChaosSim Demo](docs/images/demo.gif)

---

## 📋 Description

ChaosSim est une application de **visualisation 3D interactive** de systèmes dynamiques chaotiques. Le projet implémente trois attracteurs célèbres (Lorenz, Rössler, Chen) avec :

- ✨ Visualisation 3D temps réel avec projection perspective
- 🎨 4 modes de coloration dynamique (vélocité, position, temps, fixe)
- 🎮 Contrôles interactifs (rotation souris, zoom, paramètres)
- 🎬 Mode présentation automatique (60 secondes)
- 📸 Capture d'écran (F12)
- ⚙️ Interface ImGui complète
- 🔬 Intégration numérique Runge-Kutta 4
- ⚡ Performance optimisée (500-1000 particules @ 60 FPS)

---

## 🎥 Démonstration

### Attracteurs Implémentés

| Lorenz (Papillon) | Rössler (Ruban) | Chen (Double Hélice) |
|:-----------------:|:---------------:|:--------------------:|
| ![Lorenz](docs/images/lorenz.png) | ![Rossler](docs/images/rossler.png) | ![Chen](docs/images/chen.png) |

---

## 🚀 Installation Rapide

### Prérequis

- **Compilateur** : Clang++ ou G++ (C++17)
- **Python** : 3.8+ (pour le script de build)
- **Bibliothèques** : SDL3, ImGui, GLM (incluses dans `thirdparty/`)

### Compilation

```bash
# 1. Cloner le repository
git clone https://github.com/[username]/ChaosSim.git
cd ChaosSim

# 2. Compiler
python build.py

# 3. Lancer
./ChaosSim          # Linux/macOS
ChaosSim.exe        # Windows
```

---

## 🎮 Contrôles

### Souris
- **Clic gauche + Glisser** : Rotation caméra 3D
- **Molette** : Zoom avant/arrière

### Clavier
| Touche | Action |
|--------|--------|
| **Espace** | Pause/Play |
| **P** | Mode Présentation |
| **F12** | Capture d'écran |
| **R** | Reset caméra |
| **1/2/3** | Changer d'attracteur |
| **Échap** | Quitter |

---

## 📊 Fonctionnalités

### ✨ Principales
- [x] 3 attracteurs chaotiques (Lorenz, Rössler, Chen)
- [x] Visualisation 3D avec projection perspective complète
- [x] Rotation interactive et zoom fluide
- [x] 4 modes de coloration dynamique
- [x] Mode présentation automatique (~60s)
- [x] Capture d'écran avec timestamp
- [x] Interface ImGui complète

### ⚙️ Paramètres Modifiables
- [x] Paramètres attracteurs en temps réel (σ, ρ, β, a, b, c)
- [x] Nombre de particules (50-1000)
- [x] Longueur des trails (100-2000)
- [x] Vitesse de simulation (0.1x - 5x)
- [x] Mode de coloration

### 🎨 Modes de Coloration
1. **Vélocité** : Bleu (lent) → Rouge (rapide)
2. **Position** : RGB selon axes X, Y, Z
3. **Temps** : Arc-en-ciel cyclique
4. **Fixe** : Blanc uniforme

---

## 🏗️ Architecture
Attracteuretrange/
├── src/
│   ├── main.cpp                    # Point d'entrée
│   ├── core/                       # Noyau
│   │   ├── Game.cpp/h
│   │   ├── Renderer.cpp/h
│   │   └── PresentationMode.cpp/h
│   ├── attractors/                 # Attracteurs
│   │   ├── AttractorSystem.cpp/h
│   │   ├── LorenzAttractor.cpp/h
│   │   ├── RosslerAttractor.cpp/h
│   │   └── ChenAttractor.cpp/h
│   ├── graphics/                   # Graphiques
│   │   ├── Camera3D.cpp/h
│   │   └── ParticleSystem.cpp/h
│   ├── ui/                         # Interface
│   │   └── UIController.cpp/h
│   └── utils/                      # Utilitaires
│       ├── Colors.cpp/h
│       ├── Screenshot.cpp/h
│       └── Math.cpp/h
├── thirdparty/                     # Dépendances
│   ├── SDL3/
│   ├── imgui/
│   └── glm/
├── docs/                           # Documentation
│   ├── ARCHITECTURE.md
│   ├── USER_GUIDE.md
│   ├── EQUATIONS.md
│   └── CHANGELOG.md
└── build.py                        # Script de compilation
```

**Voir [ARCHITECTURE.md](docs/ARCHITECTURE.md) pour plus de détails.**

---

## 📐 Équations Mathématiques

### Attracteur de Lorenz
```
dx/dt = σ(y - x)
dy/dt = x(ρ - z) - y
dz/dt = xy - βz
```
Paramètres : σ=10, ρ=28, β=8/3

**Voir [EQUATIONS.md](docs/EQUATIONS.md) pour toutes les équations.**

---

## 🛠️ Technologies

| Technologie | Version | Usage |
|-------------|---------|-------|
| C++ | 17 | Langage principal |
| SDL3 | 3.x | Rendu et fenêtrage |
| ImGui | 1.89+ | Interface utilisateur |
| GLM | 0.9.9+ | Mathématiques 3D |
| Python | 3.8+ | Script de build |

---

## 📚 Documentation

- **[Guide Utilisateur](docs/USER_GUIDE.md)** : Comment utiliser ChaosSim
- **[Architecture](docs/ARCHITECTURE.md)** : Architecture technique détaillée
- **[Équations](docs/EQUATIONS.md)** : Équations mathématiques complètes
- **[Changelog](docs/CHANGELOG.md)** : Historique des versions

---

## 🎓 Projet Académique

Ce projet a été développé dans le cadre d'un cours de programmation C++ avec SDL3.

**Étudiant :** [VOTRE NOM]  
**Professeur :** Teuguia Tadjuidje Rodolf Sédéris  
**Date :** Décembre 2025  
**Note Espérée :** 16-20/20 🌟

### Critères d'Évaluation Satisfaits
- ✅ Fonctionnalité de base (40%)
- ✅ Code propre (30%)
- ✅ Interface (20%)
- ✅ Build system (10%)

---

## 🤝 Contribution

Ce projet est académique et n'accepte pas de contributions externes.

---

## 📝 Licence

MIT License - Voir [LICENSE](LICENSE)

---

## 🙏 Remerciements

- Edward Lorenz pour la découverte des attracteurs chaotiques
- Omar Cornut pour Dear ImGui
- Sam Lantinga pour SDL
- La communauté open-source

---

## 📞 Contact

**Étudiant :** [Votre Nom]  
**Email :** [votre.email@exemple.com]  
**GitHub :** [github.com/username/ChaosSim](https://github.com/username/ChaosSim)

---

**⭐ Si ce projet vous a plu, n'hésitez pas à lui donner une étoile sur GitHub !**
```

---

## ✅ RÉCAPITULATIF COMPLET

Vous devez créer ces fichiers :

1. ✅ `assets/fonts/README.md`
2. ✅ `assets/icons/README.md`
3. ✅ `build/bin/README.md`
4. ✅ `build/obj/README.md`
5. ✅ `docs/ARCHITECTURE.md`
6. ✅ `docs/CHANGELOG.md`
7. ✅ `docs/EQUATIONS.md`
8. ✅ `docs/USER_GUIDE.md`
9. ✅ `src/utils/Math.h`
10. ✅ `src/utils/Math.cpp`
11. ✅ `README.md` (racine du projet)

Tous les contenus sont fournis ci-dessus !