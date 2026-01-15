#  CHANGELOG - ChaosSim

Toutes les modifications notables de ce projet sont documentées dans ce fichier.

Le format est basé sur [Keep a Changelog](https://keepachangelog.com/fr/1.0.0/),
et ce projet adhère au [Semantic Versioning](https://semver.org/lang/fr/).

---

## [1.0.0] - 2025-12-17 (VERSION FINALE)

###  Ajouté (Semaine 4)
- Mode présentation automatique avec 7 phases de démonstration
- Capture d'écran (F12) avec timestamp automatique
- Progression visuelle de la présentation dans l'UI
- Documentation complète du projet (ARCHITECTURE.md, USER_GUIDE.md, EQUATIONS.md)
- Support de la touche P pour toggle présentation
- Fichier Screenshot.cpp/h pour gestion de l'export d'images

###  Modifié
- UIController étendu pour afficher état de la présentation
- Game.cpp intègre PresentationMode dans la boucle Update
- Interface ImGui améliorée avec nouveaux boutons et indicateurs
- README.md finalisé avec toutes les fonctionnalités

###  Corrigé
- Conflits entre pause manuelle et présentation automatique
- Gestion correcte des événements pendant la présentation

---

## [0.3.0] - 2025-12-10 (Semaine 3)

###  Ajouté
- Interface ImGui complète avec 4 fenêtres (Info, Contrôles, Paramètres, Stats)
- Sliders pour modifier paramètres attracteurs en temps réel
- 4 modes de coloration (vélocité, position, temps, fixe)
- Système de coloration dynamique (Colors.cpp/h)
- Intégration numérique Runge-Kutta 4 (RK4)
- Contrôle vitesse de simulation (0.1x à 5x)
- Ajustement dynamique nombre de particules (50-1000)
- Ajustement dynamique longueur trails (100-2000)
- Mode Pause/Play (Espace)
- Statistiques temps réel (FPS, vélocité, etc.)

###  Modifié
- AttractorSystem utilise RK4 au lieu d'Euler
- ParticleSystem calcule maintenant la vélocité
- Game.cpp réorganisé avec UIController
- Amélioration significative de la stabilité numérique

###  Performance
- Optimisation pour 500-1000 particules @ 60 FPS
- Pré-allocation mémoire pour les trails
- Batch rendering pour les lignes

---

## [0.2.0] - 2025-12-03 (Semaine 2)

###  Ajouté
- Classe Camera3D avec projection perspective complète
- Rotation interactive à la souris (drag)
- Zoom avec molette de souris
- Attracteur de Rössler (ruban tordu)
- Attracteur de Chen (double hélice)
- Système de trails visibles et colorés
- Projection 3D → 2D avec matrices de transformation
- Support de 3 attracteurs interchangeables
- Touches 1/2/3 pour changement rapide d'attracteur
- Touche R pour reset caméra

###  Modifié
- ParticleSystem supporte maintenant la projection 3D
- Game.cpp gère le polymorphisme des attracteurs
- Interface ImGui étendue avec sélection d'attracteur
- Amélioration du rendu avec vraie perspective

###  Corrigé
- Problèmes de projection orthographique
- Échelle fixe remplacée par zoom dynamique

---

## [0.1.0] - 2025-11-26 (Semaine 1)

###  Ajouté (Version Initiale)
- Structure de base du projet
- Script de build Python multi-plateforme
- Classe Game avec boucle principale
- Classe Renderer (abstraction SDL3)
- Classe abstraite AttractorSystem
- Attracteur de Lorenz (implémentation de base)
- ParticleSystem basique
- Intégration ImGui minimale
- Intégration numérique Euler simple
- Conventions de nommage (mPascalCase, PascalCase, UPPER_SNAKE_CASE)
- Documentation Betty Doc/Style
- Support Windows/Linux/macOS

###  Configuration
- SDL3 comme moteur de rendu
- ImGui pour l'interface utilisateur
- GLM pour les mathématiques 3D
- Clang++ comme compilateur

---

## Types de Changements

- ` Ajouté` - Nouvelles fonctionnalités
- ` Modifié` - Changements dans fonctionnalités existantes
- ` Déprécié` - Fonctionnalités bientôt supprimées
- ` Corrigé` - Corrections de bugs
- ` Sécurité` - Corrections de vulnérabilités
- ` Performance` - Améliorations de performance

---

## Roadmap Future (v2.0+)

### Planifié
- [ ] Attracteur d'Aizawa (4ème attracteur)
- [ ] Export vidéo (enregistrement frames)
- [ ] Système de sauvegarde/chargement de configurations
- [ ] Shaders OpenGL pour effets avancés
- [ ] Support VR/AR
- [ ] Mode multi-joueurs collaboratif

### En Considération
- [ ] Attracteurs 4D avec projection hypercubique
- [ ] Machine learning pour découverte de nouveaux attracteurs
- [ ] Calcul GPU (CUDA/OpenCL) pour 10000+ particules
- [ ] Support de joystick/gamepad

---

## Notes de Version

### v1.0.0 - Version de Présentation Académique
Cette version est la **version finale** présentée dans le cadre du projet académique.
Elle inclut toutes les fonctionnalités requises :
- ✅ 3 attracteurs chaotiques
- ✅ Visualisation 3D interactive
- ✅ Interface complète
- ✅ Mode présentation
- ✅ Documentation exhaustive
- ✅ Code propre et commenté (Betty compliant)
- ✅ Build multi-plateforme fonctionnel

**Note Espérée :** 16-20/20 🎓

---

[1.0.0]: https://github.com/[tondiphanuel2-hash]/ChaosSim/releases/tag/v1.0.0
[0.3.0]: https://github.com/[tondiphanuel2-hash]/ChaosSim/releases/tag/v0.3.0
[0.2.0]: https://github.com/[tondiphanuel2-hash]/ChaosSim/releases/tag/v0.2.0
[0.1.0]: https://github.com/[tondiphanuel2-hash]/ChaosSim/releases/tag/v0.1.0