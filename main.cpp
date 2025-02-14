#include <iostream>
#include <string>
#include "classes.hpp"
#include <chrono>

constexpr std::chrono::milliseconds pas_t(1); // pas de 1ms


using namespace std;

// Fonction de test de positivité d'une valeur
int test_positif(float a) {
    if (a < 0) {
        cout << "Erreur : a doit être positif !" << endl;
        return 0;
    }
    return 1;
}

// Calcul des nouvelles coordonnées
coo new_coo(coo pos, vitesse v, float dt) { // Euler

    coo new_coo;
    new_coo.x = pos.x + v.vx * dt;
    new_coo.y = pos.y + v.vy * dt;
    new_coo.z = pos.z + v.vz * dt;

    return new_coo;
}

// Calcul de la nouvelle vitesse
vitesse new_v(coo pos, coo old_pos, float dt) {  // il faut prendre en parametre l'ancienne pour la laisser au cas ou ça foire

    vitesse new_v;
    new_v.vx = (pos.x - old_pos.x) / dt;
    new_v.vy = (pos.y - old_pos.y) / dt;
    new_v.vz = (pos.z - old_pos.z) / dt;

    return new_v;
}

// Calcul de la nouvelle accélération
acc new_a(vitesse v, vitesse old_v, float dt) {  // il faut prendre en parametre l'ancienne pour la laisser au cas ou ça foire

    acc new_a;
    new_a.ax = (v.vx - old_v.vx) / dt;
    new_a.ay = (v.vy - old_v.vy) / dt;
    new_a.az = (v.vz - old_v.vz) / dt;

    return new_a;
}


int main() {

    // Paramètres de la simulation
    coo old_pos = {100, 100, 100};   // Position initiale
    vitesse old_v = {5, 100, 45678};  // Vitesse initiale (par exemple, 1 m/s en x et 2 m/s en y)
    float dt = std::chrono::duration_cast<std::chrono::milliseconds>(pas_t).count() / 1000.0; // Pas de temps de 1 ms
    int simulation_time_ms = 10; // Durée de la simulation en ms

    // Afficher les résultats au début
    std::cout << "Début de la simulation:" << std::endl;
    std::cout << "Position initiale: (" << old_pos.x << ", " << old_pos.y << ", " << old_pos.z << ")" << std::endl;
    std::cout << "Vitesse initiale: (" << old_v.vx << ", " << old_v.vy << ", " << old_v.vz << ")" << std::endl;

    // Simulation sur 30ms
    for (int t = 1; t <= simulation_time_ms; ++t) {
        // Calculer les nouvelles coordonnées
        coo new_pos = new_coo(old_pos, old_v, dt);
        
        // Calculer la nouvelle vitesse
        vitesse new_vi = new_v(new_pos, old_pos, dt);

        // Calculer la nouvelle accélération
        acc new_ac = new_a(new_vi, old_v, dt);

        // Affichage des résultats
        std::cout << "Temps: " << t << " ms" << std::endl;
        std::cout << "Nouvelle position: (" << new_pos.x << ", " << new_pos.y << ", " << new_pos.z << ")" << std::endl;
        std::cout << "Nouvelle vitesse: (" << new_vi.vx << ", " << new_vi.vy << ", " << new_vi.vz << ")" << std::endl;
        std::cout << "Nouvelle accélération: (" << new_ac.ax << ", " << new_ac.ay << ", " << new_ac.az << ")" << std::endl;
        std::cout << std::endl;

        // Mettre à jour la position et la vitesse pour la prochaine itération
        old_pos = new_pos;
        old_v = new_vi;
    }

    std::cout << "Fin de la simulation." << std::endl;

    return 0;
}

