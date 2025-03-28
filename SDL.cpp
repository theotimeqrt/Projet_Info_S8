#include <SDL2/SDL.h>
#include "classes.hpp"

const int WIDTH = 1280;
const int HEIGHT = 720;

//on veut faire que la longueur de notre table prenne 3/4 de la largeur de la fenetre en pixel
//on sait que la longueur de la table est de 274 cm et que la largeur de notre fenetre est de 1280 pixels.
//1 cm --> x pixels
//274 cm --> 960 pixels (3/4 de 1280)
//x = (1*960)/274
const double CM_TO_PIXELS = 3.503649635;

int camera_angle = 0; // 0: arbitre, 1: aérien

// Fonction pour tracer un pixel (point)
void tracerPixel(SDL_Renderer* renderer, int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}

// Fonction pour tracer une ligne horizontale (scanline)
void tracerLigneHorizontale(SDL_Renderer* renderer, int x1, int x2, int y) {
    SDL_RenderDrawLine(renderer, x1, y, x2, y);
}

//Fonction pour dessiner le contour du cercle
//https://fr.wikipedia.org/wiki/Algorithme_de_trac%C3%A9_d%27arc_de_cercle_de_Bresenham
void tracerCercle(SDL_Renderer* renderer, int rayon, int x_centre, int y_centre) {
    int x = 0;
    int y = rayon;
    int m = 5 - 4 * rayon;

    while (x <= y) {
        // Tracer les 8 points symétriques du cercle
        tracerPixel(renderer, x + x_centre, y + y_centre);
        tracerPixel(renderer, y + x_centre, x + y_centre);
        tracerPixel(renderer, -x + x_centre, y + y_centre);
        tracerPixel(renderer, -y + x_centre, x + y_centre);
        tracerPixel(renderer, x + x_centre, -y + y_centre);
        tracerPixel(renderer, y + x_centre, -x + y_centre);
        tracerPixel(renderer, -x + x_centre, -y + y_centre);
        tracerPixel(renderer, -y + x_centre, -x + y_centre);

        if (m > 0) {
            y--;
            m -= 8 * y;
        }
        x++;
        m += 8 * x + 4;
    }
}

// Fonction pour remplir le cercle avec scanlines
void remplirCercle(SDL_Renderer* renderer, int rayon, int x_centre, int y_centre) {
    int x = 0;
    int y = rayon;
    int m = 5 - 4 * rayon;

    while (x <= y) {
        // Dessine les scanlines entre les points symétriques
        tracerLigneHorizontale(renderer, x_centre - x, x_centre + x, y_centre + y);
        tracerLigneHorizontale(renderer, x_centre - x, x_centre + x, y_centre - y);
        tracerLigneHorizontale(renderer, x_centre - y, x_centre + y, y_centre + x);
        tracerLigneHorizontale(renderer, x_centre - y, x_centre + y, y_centre - x);

        if (m > 0) {
            y--;
            m -= 8 * y;
        }
        x++;
        m += 8 * x + 4;
    }
}

void render(SDL_Renderer* renderer, balle balle, const table &tab, const filet &net) {
    //les deux cameras ont un fond bleu    
    SDL_SetRenderDrawColor(renderer, 0, 0, 150, 255); 
    SDL_RenderClear(renderer);

    if(camera_angle == 0){ // Vue arbitre (face)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // blanc
        
        SDL_Rect rect_table = {
            //160,358,960,3};
            static_cast<int>((WIDTH - (tab.longueur * 100 * CM_TO_PIXELS)) / 2),
            static_cast<int>((HEIGHT - (tab.epaisseur * 100 * CM_TO_PIXELS)) / 2),
            static_cast<int>(tab.longueur * 100 * CM_TO_PIXELS),
            static_cast<int>(tab.epaisseur * 100 * CM_TO_PIXELS)
        };
            
        SDL_Rect rect_filet = {
            //638, 305, 3, 53
            static_cast<int>((WIDTH - (net.epaisseur * 100* CM_TO_PIXELS)) / 2),
            static_cast<int>((HEIGHT/2 - (net.hauteur * 100 * CM_TO_PIXELS))),
            static_cast<int>(net.epaisseur * 100 * CM_TO_PIXELS),
            static_cast<int>(net.hauteur * 100 * CM_TO_PIXELS)
        };
        
        SDL_RenderFillRect(renderer, &rect_table);
        SDL_RenderFillRect(renderer, &rect_filet);

    }else if(camera_angle == 1){ // Vue aérienne
        //Table
        int table_x = static_cast<int>(WIDTH - (tab.longueur * 100 * CM_TO_PIXELS)) / 2;
        int table_y = static_cast<int>(HEIGHT - (tab.largeur * 100 * CM_TO_PIXELS)) / 2;
        int table_w = static_cast<int>(tab.longueur * 100 * CM_TO_PIXELS);
        int table_h = static_cast<int>(tab.largeur * 100 * CM_TO_PIXELS);
        
        SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255); // Vert clair pour la table
        SDL_Rect rect_table = {table_x, table_y, table_w, table_h};
        SDL_RenderFillRect(renderer, &rect_table);

        //Ligne centrale
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        int x1 = (WIDTH - (tab.longueur * 100 * CM_TO_PIXELS))/2;
        int y1 = HEIGHT/2;
        int x2 = x1 + (tab.longueur * 100 * CM_TO_PIXELS);
        int y2 = y1;
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

        //Filet
        int filet_x = static_cast<int>((WIDTH - (net.epaisseur * 100 * CM_TO_PIXELS)) / 2);
        int filet_y = static_cast<int>(HEIGHT - (tab.largeur * 100 * CM_TO_PIXELS)) / 2;
        int filet_w = static_cast<int>(net.epaisseur * 100 * CM_TO_PIXELS);
        int filet_h = static_cast<int>(net.largeur * 100 * CM_TO_PIXELS);

        SDL_Rect rect_filet = {filet_x, filet_y, filet_w, filet_h};
        SDL_RenderFillRect(renderer, &rect_filet);
    }
    
    // Dessiner la balle
    int ball_radius_cm = balle.rayon * 100 * CM_TO_PIXELS; // Convertir le rayon de m → cm
    int ball_x = static_cast<int>(balle.centre.x) * 100 * CM_TO_PIXELS;
    int ball_y = static_cast<int>(balle.centre.z) * 100 * CM_TO_PIXELS; 

    // Dessiner la balle correctement
    SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // Orange
    remplirCercle(renderer, ball_radius_cm, ball_x, ball_y);

    SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255); // Orange foncé
    tracerCercle(renderer, ball_radius_cm, ball_x, ball_y);

    SDL_RenderPresent(renderer);
}

// int main(int argc, char** argv) {
//     table tab;
//     filet net;
//     balle balle = {0.02, 0.0027, {0, 0, 0}, {-1, 0, 0}, {200, 0, 100}, {0, 0, 0}};

//     //Cette fonction doit etre initialise avant les autres fonctions de SDL
//     //car c'est elle qui charge ce que les autres fonctions ont besoin
//     //comme on veut des fenetres, on a besoin d'utiliser le systeme de gestion de rendu, 
//     //en l'occurence SDL_VIDEO
//     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//         std::cout << "SDL Video initialization failed" << std::endl;
//         std::cerr << "Erreur SDL_Init : " << SDL_GetError() << std::endl;
//         SDL_Quit();
//         return EXIT_FAILURE;
//     }

//     SDL_Window* window = SDL_CreateWindow("Simulateur de Ping-Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
//     SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

//     bool running = true;
//     SDL_Event e;

//     while(running){
//         while(SDL_PollEvent(&e)){
//             if(e.type == SDL_QUIT)
//                 running = false;

//             if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE){
//                 camera_angle = !camera_angle;
//             }
//         }
//         render(renderer, balle, tab, net);
//         SDL_Delay(16);
//     }
     
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();

//     return 0;
// }