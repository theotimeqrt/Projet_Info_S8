#include <iostream>
//#include <GL/glut.h>
//#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <string>
#include "classes.hpp"
#include "forces.hpp"
#include <iomanip>
#include <chrono>
#include <vector>

// ==================== SDL Configuration =======================
// const int SCREEN_WIDTH = 1600;
// const int SCREEN_HEIGHT = 1200;
// const int GRAPH_MARGIN = 100;

// // Fonction pour dessiner une courbe SDL
// void drawGraph(SDL_Renderer* renderer, const vector<double>& data, SDL_Color color, int y_offset, double scale) {
//     int data_size = data.size();
//     if (data_size < 2) return;
//     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    
//     for (size_t i = 1; i < data_size; i++) {
//         SDL_RenderDrawLine(renderer, GRAPH_MARGIN + (i - 1) * 4, SCREEN_HEIGHT - (y_offset + data[i - 1] * scale),
//                            GRAPH_MARGIN + i * 4, SCREEN_HEIGHT - (y_offset + data[i] * scale));
//     }
// }

// // Fonction pour dessiner les axes et légendes
// void drawAxes(SDL_Renderer* renderer) {
//     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//     // Axe X (Temps)
//     SDL_RenderDrawLine(renderer, GRAPH_MARGIN, SCREEN_HEIGHT - GRAPH_MARGIN, SCREEN_WIDTH - GRAPH_MARGIN, SCREEN_HEIGHT - GRAPH_MARGIN);
//     // Axe Y (Valeurs)
//     SDL_RenderDrawLine(renderer, GRAPH_MARGIN, GRAPH_MARGIN, GRAPH_MARGIN, SCREEN_HEIGHT - GRAPH_MARGIN);
// }

// dans le forces.hpp
// const double GRAVITY = 9.81; 
// const double INVERSE_SUR_MASSE = 370.370; //  (1/2.7g) en kg

constexpr std::chrono::milliseconds pas_t(1); // pas de 1ms
//constexpr double pas_t = 1.0; // 1 seconde


using namespace std;


    // ==================== OBJETS =======================

    jeu_ping_pong jeu1;
    balle balle1;
    table table1;
    filet filet1;
    raquette raquette1;
    raquette raquette2;


// =======================================================
// ================== ACCELERATION =======================
// =======================================================

// Calcul de la nouvelle acceleration
coo new_a(double masse, coo v, coo spin, double ro) {
     if (masse == 0) {
         std::cerr << "Erreur: masse ne peut pas être 0 !" << std::endl;
         exit(1);
     }
    
    coo a;
    coo ft = force_frottement(v, ro);
    std::cout << "frottement " << ft.z << std::endl;
    coo fm = force_magnus(v, spin, ro);
    //std::cout << "fm z " << fm.z << std::endl;
    a.x = (ft.x + fm.x) * INVERSE_SUR_MASSE;
    a.y = (ft.y + fm.y) * INVERSE_SUR_MASSE;
    a.z = (ft.z + fm.z - masse * GRAVITY) * INVERSE_SUR_MASSE;
    return a;
}

// ========================================================
// ================= EULER ================================
// ========================================================

// comparer

// Calcul de la nouvelle vitesse
coo new_v(coo a, coo old_v, double dt) { 

    coo new_v;
    new_v.x = old_v.x +  a.x*dt ;
    new_v.y = old_v.y +  a.y*dt ;
    new_v.z = old_v.z +  a.z*dt ;

    return new_v;
}


// Calcul des nouvelles coordonnées
coo new_coo(coo old_pos, coo v, double dt) { 

    coo new_coo;
    new_coo.x = old_pos.x + v.x * dt;
    new_coo.y = old_pos.y + v.y * dt;
    new_coo.z = old_pos.z + v.z * dt;

    return new_coo;
}


// =============================================================
// ================== GRAPHIQUE 3D =============================
// =============================================================
// Fonction pour tester une force spécifique

void test_force(const string& name, bool gravite, bool frottement, bool magnus, bool rebond) {
    cout << "\n========= Test de la force: " << name << " =========\n";
    vector<double> position, vitesse, acceleration;
    // Initialisation de la balle
    balle balle1;
    balle1.centre = {0, 0, 1}; // 1 mètre au-dessus du sol
    balle1.v = {0, 0, 0};
    balle1.a = {0, 0, 0};
    balle1.spin = {0, 0, 0};
    balle1.masse = 0.0027;
    double dt = std::chrono::duration_cast<std::chrono::milliseconds>(pas_t).count() / 1.0;
    //double dt = pas_t;
    
    //Afficher les coordonnées initiales de la balle
    cout << "Position initiale: (" << balle1.centre.x << ", " << balle1.centre.y << ", " << balle1.centre.z << ")" << endl;
    cout << "Vitesse initiale: (" << balle1.v.x << ", " << balle1.v.y << ", " << balle1.v.z << ")" << endl;
    cout << "Acceleration initiale: (" << balle1.a.x << ", " << balle1.a.y << ", " << balle1.a.z << ")\n\n";


    // Simulation de 100 étapes (100ms)
    for (int t = 1; t <= 50; ++t) {
        coo new_acc = {0, 0, 0};
        
        if (gravite) {
            new_acc.z -= GRAVITY * balle1.masse; // Gravité
        }
        if (frottement) {
            coo ft = force_frottement(balle1.v, 1.2);
            cout << "frottement " << ft.z << std::endl;
            new_acc.x += ft.x;
            new_acc.y += ft.y;
            new_acc.z += ft.z;
        }
        if (magnus) {
            balle1.spin = {0, 0, 50}; // Spin sur Z
            coo fm = force_magnus(balle1.v, balle1.spin, 1.2);
            new_acc.x += fm.x;
            new_acc.y += fm.y;
            new_acc.z += fm.z;
        }
        //desactiver pour avoir les même resultat qu'en simulation
        new_acc.x = new_acc.x * INVERSE_SUR_MASSE;
        new_acc.y = new_acc.y * INVERSE_SUR_MASSE;
        new_acc.z = new_acc.z * INVERSE_SUR_MASSE;
        // Mise à jour de l'accélération
        balle1.a = new_acc;

        // Mise à jour de la vitesse
        balle1.v = new_v(balle1.a, balle1.v, dt);

        // Mise à jour de la position
        balle1.centre = new_coo(balle1.centre, balle1.v, dt);
    
        // Gestion du rebond
        if (rebond && (balle1.centre.z <= 0)) {
            //balle1.centre.z = 0;
            balle1.v.z = -balle1.v.z * 0.8; // Coefficient de restitution
        }

        position.push_back(balle1.centre.z);
        vitesse.push_back(balle1.v.z);
        acceleration.push_back(balle1.a.z);

        // Affichage
        cout << "Temps: " << t << "ms | Position X: " << balle1.centre.x << " | Position Y: " << balle1.centre.y << " | Position Z: " << balle1.centre.z << "\n"
        << "Vitesse X: " << balle1.v.x << " | Vitesse Y: " << balle1.v.y << " | Vitesse Z: " << balle1.v.z << "\n"
        << "Acceleration X: " << balle1.a.x << " | Acceleration Y: " << balle1.a.y << " | Acceleration Z: " << balle1.a.z << "\n\n";
    }

    // ================= SDL =================
    // SDL_Window* window = SDL_CreateWindow("Simulation Physique", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    // SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // bool running = true;
    // SDL_Event event;
    
    // while (running) {
    //     while (SDL_PollEvent(&event)) {
    //         if (event.type == SDL_QUIT) {
    //             running = false;
    //         }
    //     }
        
    //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //     SDL_RenderClear(renderer);
        
    //     SDL_Color red = {255, 0, 0, 255};
    //     SDL_Color blue = {0, 0, 255, 255};
    //     SDL_Color green = {0, 255, 0, 255};
        
    //     drawAxes(renderer);
    //     drawGraph(renderer, position, blue, 200, 50);
    //     drawGraph(renderer, vitesse, red, 300, 100);
    //     drawGraph(renderer, acceleration, green, 500, 1000);
        
    //     SDL_RenderPresent(renderer);
    //     SDL_Delay(100);
    // }
    
    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
}

// ==================================================================
// ============================= MAIN ===============================
// ==================================================================

int main(int argc, char **argv) {
    test_force("Gravite seule", true, false, false, true);
    //test_force("Frottement de l'air SEUL", false, true, false, false);
    /*
    // Paramètres initiaux
    balle1.centre = {0, 0, 2000};   
    balle1.v = {0, 0, 0}; 
    balle1.a = {0, 0, 0};
    balle1.spin = {0, 0, 0};
    balle1.masse = 0.0027; // 2.7g
    balle1.rayon = 0.02; // 2cm

    // paramètres de  temps
    //double dt = std::chrono::duration_cast<std::chrono::milliseconds>(pas_t).count() / 1000.0; // Pas de temps de 1 ms
    double dt = std::chrono::duration_cast<std::chrono::milliseconds>(pas_t).count() / 1.0 ; // Pas de temps de 1 s
    int simulation_time_ms = 10; // combien de step de simulation

   
    // Afficher les résultats au début
    std::cout << "Début de la simulation:" << std::endl;
    std::cout << "Acceleration initiale: (" << balle1.a.x << ", " << balle1.a.y << ", " <<  balle1.a.z << ")" << std::endl;
    std::cout << "Position initiale: (" << balle1.centre.x << ", " << balle1.centre.y << ", " << balle1.centre.z << ")" << std::endl;
    std::cout << "Vitesse initiale: (" << balle1.v.x << ", " << balle1.v.y << ", " << balle1.v.z << ")" << std::endl;

    // Simulation sur 30ms
    for (int t = 1; t <= simulation_time_ms; ++t) {

        //std::cout << std::fixed << std::setprecision(10) << masse << std::endl;
        
        // Affichage des résultats

        std::cout << std::endl;
        std::cout << "Temps: " << t << " step" << std::endl;

        //calculer la nouvelle acceleration
        coo new_acc = new_a(balle1.masse, balle1.v, balle1.spin, 1.2);
        balle1.a = new_acc;
        std::cout << "Acceleration: (" << balle1.a.x << ", " << balle1.a.y << ", " << balle1.a.z << ")" << std::endl;


        // Calculer la nouvelle vitesse
        coo new_vi = new_v(balle1.a, balle1.v, dt);
        balle1.v = new_vi;
        std::cout << "Vitesse: (" << balle1.v.x << ", " << balle1.v.y << ", " << balle1.v.z << ")" << std::endl;

        // Calculer les nouvelles coordonnées
        coo new_pos = new_coo(balle1.centre, balle1.v, dt);
        balle1.centre = new_pos;
        std::cout << "Position: (" << balle1.centre.x << ", " << balle1.centre.y << ", " << balle1.centre.z << ")" << std::endl;


    }
    */
    std::cout << "Fin de la simulation." << std::endl;
    return 0;
}



