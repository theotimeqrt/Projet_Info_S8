#include <iostream>
#include <string>
#include "classes.hpp"
#include <chrono>

constexpr std::chrono::milliseconds pas_t(1); // pas de 1ms


using namespace std;

// Fonction de test de positivité d'une valeur
int test_positif(int a) {
    if (a < 0) {
        cout << "Erreur : a doit être positif !" << endl;
        return 0;
    }
    return a;
}

// Calcul des nouvelles coordonnées
coo new_coo(coo pos, vitesse v, float dt) { // Euler

    if (test_positif(dt) == 0) return pos;

    coo new_coo;
    new_coo.x = pos.x + v.vx * dt;
    new_coo.y = pos.y + v.vy * dt;
    new_coo.z = pos.z + v.vz * dt;

    return new_coo;
}

// Calcul de la nouvelle vitesse
vitesse new_v(coo pos, coo old_pos, float dt) { 

    if (test_positif(dt) == 0) return {0, 0, 0};

    vitesse new_v;
    new_v.vx = (pos.x - old_pos.x) / dt;
    new_v.vy = (pos.y - old_pos.y) / dt;
    new_v.vz = (pos.z - old_pos.z) / dt;

    return new_v;
}

// Calcul de la nouvelle accélération
acc new_a(vitesse v, vitesse old_v, float dt) { 

    if (test_positif(dt) == 0) return {0, 0, 0};

    acc new_a;
    new_a.ax = (v.vx - old_v.vx) / dt;
    new_a.ay = (v.vy - old_v.vy) / dt;
    new_a.az = (v.vz - old_v.vz) / dt;

    return new_a;
}


int main() {

    coo position = {0, 0, 0}; 
    vitesse vitesse_balle = {5, 3, 0}; 
    float dt = 0.1; 

    position = new_coo(position, vitesse_balle, dt);

    std::cout << "Nouvelle position : (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;

    return 0;
    
}

