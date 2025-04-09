// SDL.cpp modifié et corrigé
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <chrono>
#include "classes.hpp"
#include "simulation.hpp"


Mix_Chunk* sonRebond = nullptr;
Mix_Chunk* sonRaquette = nullptr;

//Creation d'une structure pour les differents etats de l'application
enum AppState {
    MENU,
    JOUER,
    COMMENT_JOUER,
    QUITTER
};

AppState appState = MENU;

//Dimensions de la fenetre
const int WIDTH = 1280;
const int HEIGHT = 720;

/*Justification de notre facteur de conversion*/
//on veut que la longueur de notre table prenne 3/4 de la largeur de la fenetre en pixel
//on sait que la longueur de la table est de 274 cm et que la largeur de notre fenetre est de 1280 pixels.
//1 cm --> x pixels
//274 cm --> 960 pixels (3/4 de 1280)
//x = (1*960)/274
const double CM_TO_PIXELS = 3.503649635;

//on veut que la hauteur de notre table prenne 1/4 de la largeur de la fenetre en pixel
const int Z_ZERO_PIXEL = HEIGHT * 3 / 4; // z = 0 physique aligné à ce pixel
int camera_angle = 0; // 0 = arbitre (x, z), 1 = aerien (x, y)

extern balle balle1;
extern table table1;
extern filet filet1;
extern raquette raquette1;
extern raquette raquette2;

void tracerPixel(SDL_Renderer* renderer, int x, int y) {
    SDL_RenderDrawPoint(renderer, x, y);
}

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
        //Trace les 8 points symétriques du cercle
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

//prompt ChatGPT : "Est-il possible de remplir un cercle en utilisant l'algorithme de Bresenham ?"
void remplirCercle(SDL_Renderer* renderer, int rayon, int x_centre, int y_centre) {
    int x = 0, y = rayon, m = 5 - 4 * rayon;
    while (x <= y) {
        //Dessine les scanlines entre les points symétriques
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


void render(SDL_Renderer* renderer) {
    //les deux cameras ont un fond bleu 
    SDL_SetRenderDrawColor(renderer, 0, 0, 150, 255);
    SDL_RenderClear(renderer);

    if (camera_angle == 0) { // arbitre : plan (x, z)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //blanc
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
        //Dessin de la table
        int table_x = static_cast<int>(WIDTH - (table1.longueur * 100 * CM_TO_PIXELS)) / 2;
        int table_y = static_cast<int>(HEIGHT - (table1.largeur * 100 * CM_TO_PIXELS)) / 2;
        int table_w = static_cast<int>(table1.longueur * 100 * CM_TO_PIXELS);
        int table_h = static_cast<int>(table1.largeur * 100 * CM_TO_PIXELS);
        SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255); // Vert clair pour la table
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

    // Dessin des raquettes
    dessinerRaquette(renderer, raquette1);
    dessinerRaquette(renderer, raquette2);

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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cout << "Erreur SDL_Init: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    if (TTF_Init() != 0) {
        std::cout << "Erreur SDL_ttf: " << TTF_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
        std::cout << "Erreur IMG_Init: " << IMG_GetError() << std::endl;
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Erreur initialisation SDL_mixer :" << Mix_GetError() << std::endl;
        return 1;
    }

    sonRebond = Mix_LoadWAV("assets/son/rebond_v2.wav");
    
    if (!sonRebond) {
        std::cout << "Erreur chargement son du rebond: " << Mix_GetError() << std::endl;
        return 1;
    }

    sonRaquette = Mix_LoadWAV("assets/son/son_raquette.wav");
    if (!sonRaquette) {
        std::cout << "Erreur chargement son de la raquette: " << Mix_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Ping Pong Physics Simulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Police
    TTF_Font* font1 = TTF_OpenFont("Fonts/Pricedown Bl.otf", 64);
    TTF_Font* font2 = TTF_OpenFont("Fonts/arial.ttf", 28);
    if (!font1 || !font2) {
        std::cout << "Erreur chargement police: " << TTF_GetError() << std::endl;
        return 1;
    }

    // Titre
    SDL_Color red = {255, 0, 0, 255};
    SDL_Surface* titreSurface = TTF_RenderText_Blended(font1, "Ping Pong Physics Simulator", red);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, titreSurface);
    SDL_Rect textRect = {(1280 - titreSurface->w) / 2, 20, titreSurface->w, titreSurface->h};

    // Image de fond
    SDL_Surface* bgSurface = IMG_Load("assets/images/background.png");
    if (!bgSurface) {
        std::cout << "Erreur chargement image: " << IMG_GetError() << std::endl;
        return 1;
    }
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    //Image pour le bouton "Comment jouer ?"
    SDL_Surface* commentJouerSurface = IMG_Load("assets/images/comment_jouer.png");
    if (!commentJouerSurface) {
        std::cout << "Erreur chargement image: " << IMG_GetError() << std::endl;
        return 1;
    }
    SDL_Texture* commentJouerTexture = SDL_CreateTextureFromSurface(renderer, commentJouerSurface);
    SDL_FreeSurface(commentJouerSurface);

    // ---------- BOUTONS ----------
    
    const int button_height = 60;
    const int espace = 40; // espace entre boutons
    int jouer_width = 200;
    int comment_width = 320;
    int quitter_width = 200;

    int total_width = jouer_width + comment_width + quitter_width + 2 * espace;
    int end_y = 720 - button_height - 40; //on laisse une marge de 40 pixels pour eviter que le bouton touche le rebord
    int start_x = (1280 - total_width) / 2; //on divise par 2 pour centrer les boutons dans la fenetre

    // Créer les surfaces
    SDL_Surface* jouerSurface = TTF_RenderText_Blended(font2, "JOUER", red);
    SDL_Surface* commentSurface = TTF_RenderText_Blended(font2, "COMMENT JOUER ?", red);
    SDL_Surface* quitterSurface = TTF_RenderText_Blended(font2, "QUITTER", red);

    // Créer les textures
    SDL_Texture* jouerTexture = SDL_CreateTextureFromSurface(renderer, jouerSurface);
    SDL_Texture* commentTexture = SDL_CreateTextureFromSurface(renderer, commentSurface);
    SDL_Texture* quitterTexture = SDL_CreateTextureFromSurface(renderer, quitterSurface);

    // Créer les rectangles
    SDL_Rect jouerRect = {start_x, end_y, jouer_width, button_height};
    SDL_Rect commentRect = {start_x + jouer_width + espace, end_y, comment_width, button_height};
    SDL_Rect quitterRect = {start_x + jouer_width + espace + comment_width + espace, end_y, quitter_width, button_height};

    // Positionner le texte au centre de chaque bouton
    SDL_Rect jouerTextRect = {
        jouerRect.x + (jouer_width - jouerSurface->w) / 2,
        jouerRect.y + (button_height - jouerSurface->h) / 2,
        jouerSurface->w, jouerSurface->h
    };
    SDL_Rect commentTextRect = {
        commentRect.x + (comment_width - commentSurface->w) / 2,
        commentRect.y + (button_height - commentSurface->h) / 2,
        commentSurface->w, commentSurface->h
    };
    SDL_Rect quitterTextRect = {
        quitterRect.x + (quitter_width - quitterSurface->w) / 2,
        quitterRect.y + (button_height - quitterSurface->h) / 2,
        quitterSurface->w, quitterSurface->h
    };

    initialiser_simulation();
    int compteur = 0;

    bool running = true;
    SDL_Event e;
    auto last_time = std::chrono::high_resolution_clock::now();

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                camera_angle = !camera_angle;
            }
            if (appState == MENU && e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;
                SDL_Point clickPoint = {x, y};

                if (SDL_PointInRect(&clickPoint, &jouerRect)) {
                    appState = JOUER;
                } else if (SDL_PointInRect(&clickPoint, &commentRect)) {
                    appState = COMMENT_JOUER;
                } else if (SDL_PointInRect(&clickPoint, &quitterRect)) {
                    appState = QUITTER;
                }
            }
        }
        if (appState == QUITTER) {
            running = false;
        } else if (appState == JOUER) {
            auto current_time = std::chrono::high_resolution_clock::now();
            double dt = 0.0005; // pas de discretisation

            int fin = mise_a_jour_balle(dt, compteur);
            if (fin == 0) {
                std::cout << "Nombre d'échanges : " << (compteur / 2) << std::endl;
                SDL_Delay(1500);
                running = false;
            }
            render(renderer);
            SDL_Delay(1); // délai à chaque calculs
        }else if (appState == COMMENT_JOUER) {
            // Affichage de l'image "Comment jouer ?"
            SDL_RenderCopy(renderer, commentJouerTexture, nullptr, nullptr);
            SDL_RenderPresent(renderer);

            bool attendreRetour = true;
            while (attendreRetour) {
                while (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                        running = false;
                        attendreRetour = false;
                    } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                        appState = MENU;
                        attendreRetour = false;
                    }
                }
                SDL_Delay(10); // petite pause pour ne pas surcharger le CPU
            }
        } else if (appState == MENU) {
            SDL_RenderCopy(renderer, bgTexture, nullptr, nullptr);
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &jouerRect);
            SDL_RenderFillRect(renderer, &commentRect);
            SDL_RenderFillRect(renderer, &quitterRect);

            SDL_RenderCopy(renderer, jouerTexture, nullptr, &jouerTextRect);
            SDL_RenderCopy(renderer, commentTexture, nullptr, &commentTextRect);
            SDL_RenderCopy(renderer, quitterTexture, nullptr, &quitterTextRect);

            SDL_RenderPresent(renderer);
        }
    }

    if (bgTexture) SDL_DestroyTexture(bgTexture);
    if (commentJouerTexture) SDL_DestroyTexture(commentJouerTexture);
    if (textTexture) SDL_DestroyTexture(textTexture);
    if (titreSurface) SDL_FreeSurface(titreSurface);
    if (jouerTexture) SDL_DestroyTexture(jouerTexture);
    if (jouerSurface) SDL_FreeSurface(jouerSurface);
    if (commentTexture) SDL_DestroyTexture(commentTexture);
    if (commentSurface) SDL_FreeSurface(commentSurface);
    if (quitterTexture) SDL_DestroyTexture(quitterTexture);
    if (quitterSurface) SDL_FreeSurface(quitterSurface);

    TTF_CloseFont(font1);
    TTF_CloseFont(font2);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}