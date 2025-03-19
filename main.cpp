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
#include <fstream>
#include <filesystem>

constexpr std::chrono::milliseconds pas_t(1); // pas de 1ms
//constexpr double pas_t = 1.0; // 1 seconde

namespace fs = std::filesystem;
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
     /*if (masse == 0) {
         std::cerr << "Erreur: masse ne peut pas être 0 !" << std::endl;
         exit(1);
     }*/
    
    coo a;
    coo ft = force_frottement(v, ro);
    //std::cout << "frottement " << ft.z << std::endl;
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
    double dt = std::chrono::duration_cast<std::chrono::milliseconds>(pas_t).count() / 1000.0;
    //double dt = pas_t;
    
    //Afficher les coordonnées initiales de la balle
    cout << "Position initiale: (" << balle1.centre.x << ", " << balle1.centre.y << ", " << balle1.centre.z << ")" << endl;
    cout << "Vitesse initiale: (" << balle1.v.x << ", " << balle1.v.y << ", " << balle1.v.z << ")" << endl;
    cout << "Acceleration initiale: (" << balle1.a.x << ", " << balle1.a.y << ", " << balle1.a.z << ")\n\n";

    // Ouvrir les fichiers pour l'écriture
    ofstream pos_x_file("courbes/simulation_data_p_x.txt");
    ofstream vel_x_file("courbes/simulation_data_v_x.txt");
    ofstream acc_x_file("courbes/simulation_data_a_x.txt");
    ofstream pos_y_file("courbes/simulation_data_p_y.txt");
    ofstream vel_y_file("courbes/simulation_data_v_y.txt");
    ofstream acc_y_file("courbes/simulation_data_a_y.txt");
    ofstream pos_z_file("courbes/simulation_data_p_z.txt");
    ofstream vel_z_file("courbes/simulation_data_v_z.txt");
    ofstream acc_z_file("courbes/simulation_data_a_z.txt");

    // Vérification si les fichiers sont bien ouverts
    if (!pos_x_file.is_open() || !vel_x_file.is_open() || !acc_x_file.is_open() || !pos_y_file.is_open() || !vel_y_file.is_open() || !acc_y_file.is_open() || !pos_z_file.is_open() || !vel_z_file.is_open() || !acc_z_file.is_open()) {
        cout << "Erreur lors de l'ouverture des fichiers!" << endl;
        return;
    }

    // Simulation de 100 étapes (100ms)
    for (int t = 1; t <= 5000; t++) {
        coo new_acc = {0, 0, 0};
        coo new_acc_g = {0, 0, 0};
        coo new_acc_f = {0, 0, 0};
        coo new_acc_m = {0, 0, 0};
        
        if (gravite) {
            new_acc_g = new_a(balle1.masse, balle1.v, {0, 0, 0}, 0); // Gravité
        }

        if (frottement) {
            new_acc_f = new_a(0, balle1.v, {0, 0, 0}, 1.2); // Frottement de l'air
        }

        if (magnus) {
            new_acc_m = new_a(0, balle1.v, balle1.spin, 1.2); // Force Magnus
        }

        //desactiver pour avoir les même resultat qu'en simulation
        new_acc.x = new_acc_g.x + new_acc_f.x + new_acc_m.x;
        new_acc.y = new_acc_g.y + new_acc_f.y + new_acc_m.y;
        new_acc.z = new_acc_g.z + new_acc_f.z + new_acc_m.z;

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
        // cout << "Temps: " << t << "ms | Position X: " << balle1.centre.x << " | Position Y: " << balle1.centre.y << " | Position Z: " << balle1.centre.z << "\n"
        // << "Vitesse X: " << balle1.v.x << " | Vitesse Y: " << balle1.v.y << " | Vitesse Z: " << balle1.v.z << "\n"
        // << "Acceleration X: " << balle1.a.x << " | Acceleration Y: " << balle1.a.y << " | Acceleration Z: " << balle1.a.z << "\n\n";

        // Écrire dans les fichiers.txt
        pos_z_file << t << " " << balle1.centre.z << endl;
        vel_z_file << t << " " << balle1.v.z << endl;
        acc_z_file << t << " " << balle1.a.z << endl;
        pos_x_file << t << " " << balle1.centre.x << endl;
        vel_x_file << t << " " << balle1.v.x << endl;
        acc_x_file << t << " " << balle1.a.x << endl;
        pos_y_file << t << " " << balle1.centre.y << endl;
        vel_y_file << t << " " << balle1.v.y << endl;
        acc_y_file << t << " " << balle1.a.y << endl;
        
    }

    cout << "Position finale: (" << balle1.centre.x << ", " << balle1.centre.y << ", " << balle1.centre.z << ")" << endl;
    cout << "Vitesse finale: (" << balle1.v.x << ", " << balle1.v.y << ", " << balle1.v.z << ")" << endl;
    cout << "Acceleration finale: (" << balle1.a.x << ", " << balle1.a.y << ", " << balle1.a.z << ")\n\n";

    pos_z_file.close();
    vel_z_file.close();
    acc_z_file.close();
    pos_x_file.close();
    vel_x_file.close();
    acc_x_file.close();
    pos_y_file.close();
    vel_y_file.close();
    acc_y_file.close();

    std::cout << "Fin de la simulation." << std::endl;



}

// ==================================================================
// ============================= MAIN ===============================
// ==================================================================

int main(int argc, char **argv) {

    test_force("Gravite seule", true, true, false, false);

    
    return 0;
}



