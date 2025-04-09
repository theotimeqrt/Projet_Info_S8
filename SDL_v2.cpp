// SDL.cpp modifié - raquettes en cercles avec manche bois
#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>
#include "classes.hpp"
#include "simulation.hpp"

// Dimensions de la fenêtre
const int WIDTH = 1280;
const int HEIGHT = 720;

// Conversion des centimètres en pixels
const double CM_TO_PIXELS = 3.503649635;

// Position de la ligne de base en pixels
const int Z_ZERO_PIXEL = HEIGHT * 3 / 4;

// Angle de la caméra (0 pour vue latérale, 1 pour vue de dessus)
int camera_angle = 0;

// Objets externes utilisés dans la simulation
extern balle balle1;
extern table table1;
extern filet filet1;
extern raquette raquette1;
extern raquette raquette2;

// Fonction pour tracer un pixel à une position donnée
void tracerPixel(SDL_Renderer* renderer, int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}

// Fonction pour tracer une ligne horizontale
void tracerLigneHorizontale(SDL_Renderer* renderer, int x1, int x2, int y) {
    SDL_RenderDrawLine(renderer, x1, y, x2, y);
}

// Fonction pour tracer un cercle en utilisant l'algorithme de tracé de cercle
void tracerCercle(SDL_Renderer* renderer, int rayon, int x_centre, int y_centre) {
    int x = 0, y = rayon, m = 5 - 4 * rayon;
    while (x <= y) {
        // Tracé des 8 octants du cercle
        tracerPixel(renderer, x + x_centre, y + y_centre);
        tracerPixel(renderer, y + x_centre, x + y_centre);
        tracerPixel(renderer, -x + x_centre, y + y_centre);
        tracerPixel(renderer, -y + x_centre, x + y_centre);
        tracerPixel(renderer, x + x_centre, -y + y_centre);
        tracerPixel(renderer, y + x_centre, -x + y_centre);
        tracerPixel(renderer, -x + x_centre, -y + y_centre);
        tracerPixel(renderer, -y + x_centre, -x + y_centre);
        if (m > 0) { y--; m -= 8 * y; }
        x++; m += 8 * x + 4;
    }
}

// Fonction pour remplir un cercle
void remplirCercle(SDL_Renderer* renderer, int rayon, int x_centre, int y_centre) {
    int x = 0, y = rayon, m = 5 - 4 * rayon;
    while (x <= y) {
        // Remplissage des lignes horizontales pour chaque segment du cercle
        tracerLigneHorizontale(renderer, x_centre - x, x_centre + x, y_centre + y);
        tracerLigneHorizontale(renderer, x_centre - x, x_centre + x, y_centre - y);
        tracerLigneHorizontale(renderer, x_centre - y, x_centre + y, y_centre + x);
        tracerLigneHorizontale(renderer, x_centre - y, x_centre + y, y_centre - x);
        if (m > 0) { y--; m -= 8 * y; }
        x++; m += 8 * x + 4;
    }
}

// Fonction pour dessiner une raquette
void dessinerRaquette(SDL_Renderer* renderer, const raquette& r) {
    // Conversion des coordonnées de la raquette en pixels
    int x = r.centre.x * 100 * CM_TO_PIXELS + WIDTH / 2;
    int y = (camera_angle == 0) ? Z_ZERO_PIXEL - r.centre.z * 100 * CM_TO_PIXELS
                                : r.centre.y * 100 * CM_TO_PIXELS + HEIGHT / 2;

    int rayon = 20; // Rayon de la tête de la raquette

    // Dessin du manche en marron foncé
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    SDL_Rect manche = {x - 3, y + rayon, 6, 15};
    SDL_RenderFillRect(renderer, &manche);

    // Dessin de la tête de la raquette (rouge ou noir selon la position)
    if (r.centre.x < 0)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge
    else
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);   // Noir

    remplirCercle(renderer, rayon, x, y);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    tracerCercle(renderer, rayon, x, y);
}

// Fonction pour dessiner la scène
void render(SDL_Renderer* renderer) {
    // Effacement de l'écran avec une couleur bleue
    SDL_SetRenderDrawColor(renderer, 0, 0, 150, 255);
    SDL_RenderClear(renderer);

    if (camera_angle == 0) {
        // Vue latérale : dessin de la table et du filet
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect rect_table = {
            static_cast<int>((WIDTH - (table1.longueur * 100 * CM_TO_PIXELS)) / 2),
            Z_ZERO_PIXEL,
            static_cast<int>(table1.longueur * 100 * CM_TO_PIXELS),
            static_cast<int>(table1.epaisseur * 100 * CM_TO_PIXELS)
        };
        SDL_Rect rect_filet = {
            static_cast<int>((WIDTH - (filet1.epaisseur * 100 * CM_TO_PIXELS)) / 2),
            Z_ZERO_PIXEL - static_cast<int>(filet1.hauteur * 100 * CM_TO_PIXELS),
            static_cast<int>(filet1.epaisseur * 100 * CM_TO_PIXELS),
            static_cast<int>(filet1.hauteur * 100 * CM_TO_PIXELS)
        };
        SDL_RenderFillRect(renderer, &rect_table);
        SDL_RenderFillRect(renderer, &rect_filet);
    } 
    else {
        // Vue de dessus : dessin de la table et du filet
        int table_x = static_cast<int>(WIDTH - (table1.longueur * 100 * CM_TO_PIXELS)) / 2;
        int table_y = static_cast<int>(HEIGHT - (table1.largeur * 100 * CM_TO_PIXELS)) / 2;
        int table_w = static_cast<int>(table1.longueur * 100 * CM_TO_PIXELS);
        int table_h = static_cast<int>(table1.largeur * 100 * CM_TO_PIXELS);
        SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
        SDL_Rect rect_table = {table_x, table_y, table_w, table_h};
        SDL_RenderFillRect(renderer, &rect_table);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, table_x, HEIGHT / 2, table_x + table_w, HEIGHT / 2);

        int filet_x = static_cast<int>((WIDTH - (filet1.epaisseur * 100 * CM_TO_PIXELS)) / 2);
        int filet_y = static_cast<int>((HEIGHT - (table1.largeur * 100 * CM_TO_PIXELS)) / 2);
        int filet_w = static_cast<int>(filet1.epaisseur * 100 * CM_TO_PIXELS);
        int filet_h = static_cast<int>(table1.largeur * 100 * CM_TO_PIXELS);

        SDL_Rect rect_filet = {filet_x, filet_y, filet_w, filet_h};
        SDL_RenderFillRect(renderer, &rect_filet);
    }

    // Dessin des raquettes
    dessinerRaquette(renderer, raquette1);
    dessinerRaquette(renderer, raquette2);

    // Dessin de la balle
    int radius = balle1.rayon * 100 * CM_TO_PIXELS;
    int x = balle1.centre.x * 100 * CM_TO_PIXELS + WIDTH / 2;
    int y = (camera_angle == 0) ? Z_ZERO_PIXEL - balle1.centre.z * 100 * CM_TO_PIXELS
                                : balle1.centre.y * 100 * CM_TO_PIXELS + HEIGHT / 2;

    SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // Couleur orange
    remplirCercle(renderer, radius, x, y);
    SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255); // Couleur orange foncé
    tracerCercle(renderer, radius, x, y);

    // Affichage de la scène
    SDL_RenderPresent(renderer);
}

int main() {
    // Initialisation de SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Simulateur Ping Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Initialisation de la simulation
    initialiser_simulation();
    int compteur = 0;
    bool running = true;
    SDL_Event e;
    auto last_time = std::chrono::high_resolution_clock::now();

    // Boucle principale
    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false; // Quitter
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) camera_angle = !camera_angle; // Changer l'angle de la caméra
        }

        // Mise à jour de la simulation
        auto current_time = std::chrono::high_resolution_clock::now();
        double dt = 0.001; // Pas de temps
        last_time = current_time;

        int fin = mise_a_jour_balle(dt, compteur);
        if (fin == 0) {
            // Fin de la partie
            std::cout << "Nombre d'échanges : " << (compteur / 2) << std::endl;
            SDL_Delay(1500);
            running = false;
            break;
        }

        // Rendu de la scène
        render(renderer);
        SDL_Delay(1.5); // Pause pour limiter la vitesse de la simulation
    }

    // Nettoyage et fermeture
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
