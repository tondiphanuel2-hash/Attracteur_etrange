## 8. docs/USER_GUIDE.md

Créez `docs/USER_GUIDE.md` :

```markdown
# 📖 GUIDE UTILISATEUR - ChaosSim

## Installation

### Windows
1. Télécharger ChaosSim.zip
2. Extraire dans un dossier
3. Double-cliquer sur ChaosSim.exe

### Linux
```bash
chmod +x ChaosSim
./ChaosSim
```

### macOS
```bash
chmod +x ChaosSim
./ChaosSim
```

---

## Contrôles

### Souris
- **Clic gauche + Glisser** : Rotation caméra 3D
- **Molette** : Zoom avant/arrière

### Clavier
- **Espace** : Pause/Play
- **P** : Mode Présentation
- **F12** : Capture d'écran
- **R** : Reset caméra
- **1** : Attracteur Lorenz
- **2** : Attracteur Rössler
- **3** : Attracteur Chen
- **Échap** : Quitter

---

## Interface Utilisateur

### Fenêtre Informations
- FPS actuel
- Temps de frame
- Raccourcis clavier

### Fenêtre Contrôles
- Boutons changement attracteur
- Pause/Play
- Reset
- Vitesse simulation
- Mode Présentation
- Capture d'écran
- Nombre particules
- Longueur trails
- Mode coloration

### Fenêtre Paramètres
Modifiez les paramètres mathématiques en temps réel :
- **Lorenz** : σ, ρ, β
- **Rössler** : a, b, c
- **Chen** : a, b, c

### Fenêtre Statistiques
- Position caméra
- Distance caméra
- Position attracteur
- Nombre particules
- Points totaux dessinés

---

## Modes de Coloration

### Vélocité
Couleurs selon la vitesse :
- Bleu = Lent
- Vert/Jaune = Moyen
- Rouge = Rapide

### Position
Couleurs selon position dans l'espace :
- Rouge = Axe X
- Vert = Axe Y
- Bleu = Axe Z

### Temps
Arc-en-ciel qui cycle sur 10 secondes

### Fixe
Blanc uniforme

---

## Mode Présentation

Appuyez sur **P** pour démarrer une démo automatique de ~60 secondes :

1. **Phase 1** (10s) : Introduction Lorenz
2. **Phase 2** (10s) : Rotation Lorenz
3. **Phase 3** (3s) : Transition Rössler
4. **Phase 4** (10s) : Présentation Rössler
5. **Phase 5** (3s) : Transition Chen
6. **Phase 6** (10s) : Présentation Chen
7. **Phase 7** (10s) : Finale

Appuyez à nouveau sur **P** pour arrêter.

---

## Capture d'Écran

Appuyez sur **F12** pour capturer l'écran actuel.

Les fichiers sont sauvegardés dans :
```
screenshots/chaossim_YYYY-MM-DD_HH-MM-SS.bmp
```

---

## Dépannage

### Problème : FPS faible
- Réduire nombre de particules
- Réduire longueur trails

### Problème : SDL3.dll introuvable (Windows)
```cmd
copy thirdparty\SDL3\lib\windows\SDL3.dll .
```

### Problème : L'application ne se lance pas
Vérifiez que tous les fichiers sont présents :
- ChaosSim.exe
- SDL3.dll (Windows)
- thirdparty/ (dossier complet)
```

---## Support

Pour toute question ou problème technique, contactez :
- Email : support@chaossim.fr
- GitHub : https://github.com/chaossim/chaossim/issues