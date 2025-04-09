#include <math.h>
#include "forces.hpp"
#include "physique.hpp"
#include "autopilote.hpp"

using namespace std;

// ================== FORCES ========================

coo force_frottement(coo v, double ro) {

    coo ft;
    double Cx = 0.47 ; // coefficient de trainée
    double pi = 3.14159265;
    double r = 0.02;
    double S = pi*r*r; // surface frontale sphere

    ft.x = -0.5*Cx*ro*S*v.x*abs(v.x);
    ft.y = -0.5*Cx*ro*S*v.y*abs(v.y);
    ft.z = -0.5*Cx*ro*S*v.z*abs(v.z);
    return ft ;

}

// Calcul de la force Magnus
coo force_magnus(coo v, coo spin, double ro) {

    coo fm;
    double Cm = 0.3; // approx à une cst
    double r = 0.02;
    
    // Calcul du produit vectoriel : spin x v
    double S = Cm * ro * r * r * r;
    fm.x = S * (spin.y * v.z - spin.z * v.y);
    fm.y = S * (spin.z * v.x - spin.x * v.z);
    fm.z = S * (spin.x * v.y - spin.y * v.x);
    return fm;

}


coo force_normale(coo pos, double masse, coo ft,coo fm) {

    coo N;
    N.z = -(ft.z + fm.z - masse * GRAVITY) * INVERSE_SUR_MASSE;
    return N;
}


coo force_frottement_rebond(coo v, coo spin, double masse, double mu) {
    coo fr = {0, 0, 0};
    double r = 0.02; // Rayon balle ping-pong

    // Vitesse tangentielle au point de contact
    coo v_contact;
    v_contact.x = v.x - r * spin.y;
    v_contact.y = v.y + r * spin.x;

    // Appliquer le frottement si la balle glisse
    fr.x = -mu * masse * GRAVITY * (v_contact.x / (abs(v_contact.x) + 1e-6)); 
    fr.y = -mu * masse * GRAVITY * (v_contact.y / (abs(v_contact.y) + 1e-6));
    fr.z = 0;

    return fr;
}


void appliquer_frottement_au_rebond(coo* v, coo* spin, coo fr, double masse, double dt_contact) {
    // Mise à jour de la vitesse horizontale due au frottement
    v->x += (fr.x / masse) * dt_contact;
    v->y += (fr.y / masse) * dt_contact;

    // Mise à jour du spin (retour de couple inverse sur la balle)
    double I = (2.0/5.0) * masse * 0.02 * 0.02; // Moment d’inertie d’une sphère
    spin->y -= (fr.x * 0.02 / I) * dt_contact;  // frottement sur x => spin autour de y
    spin->x += (fr.y * 0.02 / I) * dt_contact;
}



// ================== COLLISIONS ========================


// on approxime la balle à juste son centre pour l'instant
bool collision_filet(balle b, filet f) {
    //test filet
    if (b.centre.x >= -0.01 && b.centre.x <= 0.01) { // 1cm de chaque côté du filet
        if (b.centre.z >= 0 && b.centre.z <= f.hauteur) {
            if (b.centre.y >= -(f.largeur / 2) && b.centre.y <= (f.largeur / 2)) {
                return 1;
            }
        }
    }
    return 0;
}

bool collision_table(balle b, table t) {
    //test table
    if (b.centre.x >= -t.longueur / 2 && b.centre.x <= t.longueur / 2) {
        if (b.centre.y >= -t.largeur / 2 && b.centre.y <= t.largeur / 2) {
            if (b.centre.z <= 0) {
                //Forcer la position de la balle à 0
                return 1;
            }
        }
    }
    return 0;
}

bool collision_raquette(balle b, raquette r) {
    //test raquette
    if (b.centre.z >= (r.centre.z - 0.01 ) && b.centre.z <= (r.centre.z + 0.01 )) {
        if (b.centre.y >= (r.centre.y - 0.01 ) && b.centre.y <= (r.centre.y + 0.01 )) {
            if (b.centre.x >= (r.centre.x - 0.01) && b.centre.x <= (r.centre.x + 0.01)) { 
                return 1;
            }
        }
    }

    return 0;
}

bool collision_sol(balle b) {
    //hors jeu si 1m en dessous
    if (b.centre.z <= -1) { 
                return 1;
            }
    return 0;
}

// ==================== Fonction test =====================

void test_force(bool gravite, bool frottement, bool magnus, balle &balle1, table table1, raquette &raquette1, raquette &raquette2, filet filet1 ) {
 
    cout << "\n========= Début de la partie =========\n";


    double dt = std::chrono::duration_cast<std::chrono::milliseconds>(pas_t).count() / 1000.0; // ms

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

    int t = 0;
    int player = 0;
    int dem_coup= 1;
    int dernier_player = 0;
    int a_touche_table = 1;
    coo fr = {0,0,0};

    // Simulation de t tours
    while (1) { 

        t += 1;

        player = need_coup(balle1);
        if (player != 0){
            dernier_player = player;
        }

        if (collision_sol(balle1)) {
            cout << "Balle au sol ! " << endl;
            cout << "Fin de jeu, bien joué au joueur " << dernier_player << endl;
            break;
        }

        if (collision_filet(balle1, filet1)) {
            cout << "Balle dans le filet ! " << endl;
            if (dernier_player == 1) {
                cout << "Le joueur 2 a gagné !" << endl;
            } else {
                cout << "Le joueur 1 a gagné !" << endl;
            }
            break;
        }

        if (collision_table(balle1, table1)) {
            cout << "Rebond table" << endl;
            a_touche_table = 1;
        }


        move_raquettes(balle1, raquette1, raquette2); // se déplacent pour suivre la balle

        if (player == 1) { // gauche

            if(dem_coup == 1 && a_touche_table == 1) {
                cout << "Coup player 1 " << endl;
                fr = coup (balle1, player);
                dem_coup = 0;
                a_touche_table = 1;
                cout << "A t = " << t << " ms "<< endl;
                cout << "Position de la balle : " << balle1.centre.x << " , " << balle1.centre.y << " , " << balle1.centre.z << endl;
                cout << "Position de la raquette1 : " << raquette1.centre.x << " , " << raquette1.centre.y << " , " << raquette1.centre.z << endl;
                cout << "Position de la raquette2 : " << raquette2.centre.x << " , " << raquette2.centre.y << " , " << raquette2.centre.z << endl;
                cout << " Acceleration X balle = " << balle1.a.x << endl;
                cout << " Acceleration Y balle = " << balle1.a.y << endl;
                cout << " Acceleration Z balle = " << balle1.a.z << endl;
                cout << " Vitesse X balle = " << balle1.v.x << endl;
                cout << " Vitesse Y balle = " << balle1.v.y << endl;
                cout << " Vitesse Z balle = " << balle1.v.z << endl;
            }
            
            
            
        }
        else if (player == 2){// droite

            if(dem_coup == 1 && a_touche_table == 1) {
                cout << "Coup player 2 " << endl;
                fr = coup (balle1, player);
                fr.x = -fr.x;
                dem_coup = 0;
                a_touche_table = 1; 
                cout << "A t = " << t << " ms "<< endl;
                cout << "Position de la balle : " << balle1.centre.x << " , " << balle1.centre.y << " , " << balle1.centre.z << endl;
                cout << "Position de la raquette1 : " << raquette1.centre.x << " , " << raquette1.centre.y << " , " << raquette1.centre.z << endl;
                cout << "Position de la raquette2 : " << raquette2.centre.x << " , " << raquette2.centre.y << " , " << raquette2.centre.z << endl;
                cout << " Acceleration X balle = " << balle1.a.x << endl;
                cout << " Acceleration Y balle = " << balle1.a.y << endl;
                cout << " Acceleration Z balle = " << balle1.a.z << endl;
                cout << " Vitesse X balle = " << balle1.v.x << endl;
                cout << " Vitesse Y balle = " << balle1.v.y << endl;
                cout << " Vitesse Z balle = " << balle1.v.z << endl;
            }
            
            

        }
        else if(player == 0) {
            dem_coup = 1;
        }


        coo new_acc = {0, 0, 0};
        new_acc = new_a(balle1.v, balle1.spin, 1.2, balle1, table1, raquette1, raquette2, filet1, fr);

        // Mise à jour de l'accélération
        balle1.a = new_acc;

        // Mise à jour de la vitesse
        balle1.v = new_v(balle1.a, balle1.v, dt, balle1, table1, raquette1, raquette2, fr);

        // Mise à jour de la position
        balle1.centre = new_coo(balle1.centre, balle1.v, dt);

        
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
