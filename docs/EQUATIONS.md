## 7. docs/EQUATIONS.md

Créez `docs/EQUATIONS.md` avec ce contenu :

```markdown
# 📐 ÉQUATIONS MATHÉMATIQUES - ChaosSim

## Attracteur de Lorenz

### Équations Différentielles
```
dx/dt = σ(y - x)
dy/dt = x(ρ - z) - y  
dz/dt = xy - βz
```

### Paramètres Classiques
- σ (sigma) = 10.0
- ρ (rho) = 28.0
- β (beta) = 8/3 ≈ 2.6667

### Interprétation Physique
- x : Intensité de la convection
- y : Différence de température horizontale
- z : Différence de température verticale

---

## Attracteur de Rössler

### Équations Différentielles
```
dx/dt = -y - z
dy/dt = x + ay
dz/dt = b + z(x - c)
```

### Paramètres Classiques
- a = 0.2
- b = 0.2
- c = 5.7

---

## Attracteur de Chen

### Équations Différentielles
```
dx/dt = a(y - x)
dy/dt = (c - a)x - xz + cy
dz/dt = xy - bz
```

### Paramètres Classiques
- a = 35.0
- b = 3.0
- c = 28.0

---

## Méthode d'Intégration : Runge-Kutta 4

### Formule
```
k₁ = f(tₙ, yₙ)
k₂ = f(tₙ + h/2, yₙ + h·k₁/2)
k₃ = f(tₙ + h/2, yₙ + h·k₂/2)
k₄ = f(tₙ + h, yₙ + h·k₃)

yₙ₊₁ = yₙ + (h/6)(k₁ + 2k₂ + 2k₃ + k₄)
```

### Pas de Temps
- h = 0.01 (recommandé pour stabilité)

---

## Projection 3D → 2D

### Coordonnées Sphériques
```
x = r·sin(φ)·cos(θ)
y = r·cos(φ)
z = r·sin(φ)·sin(θ)
```

### Matrice de Vue (LookAt)
```
view = lookAt(cameraPos, target, up)
```

### Matrice de Projection
```
proj = perspective(fov, aspect, near, far)
```

### Pipeline Complet
```
WorldPos → ViewMatrix → ProjMatrix → ÷w → NDC → ScreenCoords
```
