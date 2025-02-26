#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <string>
#include "classes.hpp"
#include "fonctions.hpp"
#include <chrono>


const float GRAVITY = 9.81; 

constexpr std::chrono::milliseconds pas_t(1); // pas de 1ms


using namespace std;


// ==================== OBJETS =======================

jeu_ping_pong jeu1;
balle balle1;
table table1;
filet filet1;
raquette raquette1;
raquette raquette2;


// ================== ACCELERATION =======================

// Calcul de la nouvelle acceleration
coo new_a(int masse, coo v, coo spin, float ro) {
    coo a;
    coo ft = force_frottement(v, ro);
    coo fm = force_magnus(v, spin);
    a.x = (ft.x + fm.x) / masse;
    a.y = (ft.y + fm.y) / masse;
    a.z = (ft.z + fm.z - masse * GRAVITY) / masse;
    return a;
}


// ================= EULER ====================
// comparer

// Calcul de la nouvelle vitesse
coo new_v(coo a, coo old_v, float dt) { 

    coo new_v;
    new_v.x = old_v.x +  a.x*dt ;
    new_v.y = old_v.y +  a.y*dt ;
    new_v.z = old_v.z +  a.z*dt ;

    return new_v;
}


// Calcul des nouvelles coordonnées
coo new_coo(coo old_pos, coo v, float dt) { 

    coo new_coo;
    new_coo.x = old_pos.x + v.x * dt;
    new_coo.y = old_pos.y + v.y * dt;
    new_coo.z = old_pos.z + v.z * dt;

    return new_coo;
}


// ============================= MAIN ===============================



int main() {

    // Paramètres initiaux
    coo old_pos = {0, 0, 2000};   
    coo v = {0, 0, 0}; 
    coo a = {0, 0, 0};
    coo spin = {0, 0, 0};

    // paramètres de  temps
    //float dt = std::chrono::duration_cast<std::chrono::milliseconds>(pas_t).count() / 1000.0; // Pas de temps de 1 ms
    float dt = std::chrono::duration_cast<std::chrono::milliseconds>(pas_t).count() / 1.0 ; // Pas de temps de 1 s
    int simulation_time_ms = 10; // combien de step de simulation

    // Afficher les résultats au début
    std::cout << "Début de la simulation:" << std::endl;
    std::cout << "Acceleration initiale: (" << a.x << ", " << a.y << ", " << a.z << ")" << std::endl;
    std::cout << "Position initiale: (" << old_pos.x << ", " << old_pos.y << ", " << old_pos.z << ")" << std::endl;
    std::cout << "Vitesse initiale: (" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;

    // Simulation sur 30ms
    for (int t = 1; t <= simulation_time_ms; ++t) {

        //calculer la nouvelle acceleration
        coo new_acc = new_a(balle1.masse, v, spin,1.2);
        a = new_acc;


        // Calculer la nouvelle vitesse
        coo new_vi = new_v(a, v, dt);
        v = new_vi;

        // Calculer les nouvelles coordonnées
        coo new_pos = new_coo(old_pos, v, dt);
        old_pos = new_pos;
        
        

        // Affichage des résultats
        std::cout << "Temps: " << t << " step" << std::endl;
        std::cout << "Nouvelle position: (" << new_pos.x << ", " << new_pos.y << ", " << new_pos.z << ")" << std::endl;
        std::cout << "Nouvelle vitesse: (" << new_vi.x << ", " << new_vi.y << ", " << new_vi.z << ")" << std::endl;
        std::cout << std::endl;

        
        
    }

    std::cout << "Fin de la simulation." << std::endl;
    return 0;
}

