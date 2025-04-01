// SDL.cpp modifié
#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>
#include "classes.hpp"
#include "simulation.hpp"
// SDL.cpp modifié avec alignement z=0

const int WIDTH = 1280;
const int HEIGHT = 720;
const double CM_TO_PIXELS = 3.503649635;
const int Z_ZERO_PIXEL = HEIGHT * 3 / 4; // z = 0 physique aligné à ce pixel
int camera_angle = 0; // 0 = arbitre (x, z), 1 = aerien (x, y)

extern balle balle1;
extern table table1;
extern filet filet1;

void tracerPixel(SDL_Renderer* renderer, int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}

void tracerLigneHorizontale(SDL_Renderer* renderer, int x1, int x2, int y) {
    SDL_RenderDrawLine(renderer, x1, y, x2, y);
}

void tracerCercle(SDL_Renderer* renderer, int rayon, int x_centre, int y_centre) {
    int x = 0, y = rayon, m = 5 - 4 * rayon;
    while (x <= y) {
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

void remplirCercle(SDL_Renderer* renderer, int rayon, int x_centre, int y_centre) {
    int x = 0, y = rayon, m = 5 - 4 * rayon;
    while (x <= y) {
        tracerLigneHorizontale(renderer, x_centre - x, x_centre + x, y_centre + y);
        tracerLigneHorizontale(renderer, x_centre - x, x_centre + x, y_centre - y);
        tracerLigneHorizontale(renderer, x_centre - y, x_centre + y, y_centre + x);
        tracerLigneHorizontale(renderer, x_centre - y, x_centre + y, y_centre - x);
        if (m > 0) { y--; m -= 8 * y; }
        x++; m += 8 * x + 4;
    }
}

void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 150, 255); // fond bleu
    SDL_RenderClear(renderer);

    if (camera_angle == 0) { // arbitre : plan (x, z)
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
    } else { // aérienne : plan (x, y)
        int table_x = static_cast<int>(WIDTH - (table1.longueur * 100 * CM_TO_PIXELS)) / 2;
        int table_y = static_cast<int>(HEIGHT - (table1.largeur * 100 * CM_TO_PIXELS)) / 2;
        int table_w = static_cast<int>(table1.longueur * 100 * CM_TO_PIXELS);
        int table_h = static_cast<int>(table1.largeur * 100 * CM_TO_PIXELS);
        SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
        SDL_Rect rect_table = {table_x, table_y, table_w, table_h};
        SDL_RenderFillRect(renderer, &rect_table);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, table_x, HEIGHT/2, table_x + table_w, HEIGHT/2);

        // Dessiner le filet (vu du dessus)
        int filet_x = static_cast<int>((WIDTH - (filet1.epaisseur * 100 * CM_TO_PIXELS)) / 2);
        int filet_y = static_cast<int>((HEIGHT - (table1.largeur * 100 * CM_TO_PIXELS)) / 2);
        int filet_w = static_cast<int>(filet1.epaisseur * 100 * CM_TO_PIXELS);
        int filet_h = static_cast<int>(table1.largeur * 100 * CM_TO_PIXELS); // largeur = axe y

        SDL_Rect rect_filet = {filet_x, filet_y, filet_w, filet_h};
        SDL_RenderFillRect(renderer, &rect_filet);

    }

    int radius = balle1.rayon * 100 * CM_TO_PIXELS;
    int x = balle1.centre.x * 100 * CM_TO_PIXELS + WIDTH/2;
    int y = (camera_angle == 0) ? Z_ZERO_PIXEL - balle1.centre.z * 100 * CM_TO_PIXELS
                                : balle1.centre.y * 100 * CM_TO_PIXELS + HEIGHT/2;

    SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
    remplirCercle(renderer, radius, x, y);
    SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255);
    tracerCercle(renderer, radius, x, y);

    SDL_RenderPresent(renderer);
}



int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Simulateur Ping Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    initialiser_simulation();

    bool running = true;
    SDL_Event e;
    auto last_time = std::chrono::high_resolution_clock::now();

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) camera_angle = !camera_angle;
        }

        auto current_time = std::chrono::high_resolution_clock::now();
        double dt = 0.001;
        last_time = current_time;

        int fin = mise_a_jour_balle(dt);
        if (fin == 0) {
            SDL_Delay(1500);
            running = false;
            break;
        }
        render(renderer);
        SDL_Delay(1.5);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}