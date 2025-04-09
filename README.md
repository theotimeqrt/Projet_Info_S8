## Simulateur de Physique de Ping Pong 3D

### Description

Ce projet est un simulateur de physique en 3D pour le ping pong, mettant en œuvre les principes de la programmation orientée objet (POO). 
L'objectif est de simuler les effets de balle (lift, slice, smash, etc.) en mettant l'accent sur la précision physique et la réalisation graphique.

### Fonctionnalités

- **Rendu graphique 2D réaliste avec SDL**, modélisant les interactions 3D (vue arbitre et vue aérienne).
- **Modélisation d'une table de ping pong, de deux raquettes et d'une balle** avec collisions et rebonds réalistes.
- **Simulation complète** de trajectoires, avec calcul de l'effet Magnus, frottement de l'air, et gravité.
- **Jeu interactif avec IA** intégrée : les coups sont automatiquement renvoyés en fonction des règles physiques.
- **Large palette de coups** différents grâce au spin (service court, lift, lobe, smash...)
- **Affichage dynamique des échanges**, avec comptage des échanges réussis.
- **Visualisation scientifique** : génération de courbes de position, vitesse et accélération (x, y, z).

### Partie graphique (SDL)

- **Menu de démarrage interactif** avec fond personnalisé, titre stylisé, et boutons "JOUER", "COMMENT JOUER ?", "QUITTER"
- **Affichage de la table et du filet** réalistes
- **Dessins stylisés des raquettes** (couleur rouge/noire, manche en bois)
- **Simulation visuelle en temps réel** avec animation fluide de la balle et des raquettes
- **Commutation de caméra** en temps réel entre vue de l'arbitre (x,z) et vue du dessus (x,y) via `Espace`

## Jouer

### Lancement
```bash
make
```

## Technologies Utilisées

- **Langages** : C++ et Python (pour l'analyse graphique)
- **Bibliothèques** : SDL2, SDL_ttf, SDL_image
- **Environnement de développement** : Visual Studio Code
- **Versionnement** : GitHub

## Structure des fichiers

### ▶️ Modélisation physique
- `physique.cpp` : formules d'intégration d'Euler pour calculer la trajectoire de la balle
- `forces.cpp` : calcul des forces (gravité, frottement, Magnus...) et gestion des collisions
- `autopilote.cpp` : intelligence des coups (force, effet, cible, joueur à jouer, aléatoire)

### 📊 Simulation de trajectoires
- `main.cpp` : fonction principale de la simulation de trajectoire
- `courbes.py` : génération de courbes (positions, vitesses, accélérations) dans `./courbes`

### 🕹️ Jeu interactif
- `SDL.cpp` : affichage graphique du jeu, menu et boucle principale SDL
- `simulation.cpp` : logique du jeu, appels aux fonctions physiques, gestion des échanges

## Bibliographie

- https://wiki.libsdl.org/SDL2/FrontPage
- https://zestedesavoir.com/tutoriels/1014/utiliser-la-sdl-en-langage-c/
- https://devopssec.fr/article/cours-introduction-sdl-2
- https://loka.developpez.com/tutoriel/sdl/sons/
