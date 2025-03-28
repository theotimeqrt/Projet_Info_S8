#include <iostream>
#include <string>
#include "classes.hpp"
#include "forces.hpp"
#include <chrono>

using namespace std;

// ========================= Détection joueur qui joue ==============================

int need_coup(balle b) { // O personne 1 joueur gauche 2 joueur droite

    if (b.centre.x <= -1.35) { // joueur gauche doit jouer
        return 1;
    }
    else if (b.centre.x >= 1.35) {
        return 2;
    }
    else {
        return 0;
    }
    
}

// ========================= Déplacement des raquettes ===============================


void move_raquettes(balle b, raquette &r1, raquette &r2) {

        r1.centre = {-1.37, b.centre.y, b.centre.z};
        r2.centre = {1.37, b.centre.y, b.centre.z};

}

// ========================= Détection fin de jeu ========================

bool fin_jeu(balle b, filet f) {
    
    if (collision_sol(b)) {
        cout << "Fin de jeu, balle au sol" << endl;
        return 1;
    }
    else if (collision_filet(b,f)) {
        cout << "Fin de jeu, balle au filet" << endl;
        return 1;
    }

    return 0;
}


// ========================= Calcul de la force d'un coup =======================

// Fonction a appellé seulement lorsque la balle est proche de la raquette
coo coup (balle &b1, int player){

    coo fr;
    int type_coup;
    cout << "entrer un coup (1, 2, 3, 4): ";
    cin >> type_coup;
    cout << endl;

    switch (type_coup) {
       
        case 1: //Renvoi tout droit
            b1.spin.x = 0;
            b1.spin.y = 0;
            b1.spin.z = 0;
            
            fr.x = 6;
            fr.y = 0;
            fr.z = 2.5;
            break;
        
        case 2: // Lift
            if (player == 1){
                b1.spin.x = 0;
                b1.spin.y = -800; // 800 rad/s
                b1.spin.z = 0;   //      coo fr = force_fr
                // }
            
            }
            else if (player == 2){
                b1.spin.x = 0;
                b1.spin.y = 800;
                b1.spin.z = 0;
            }
            
            fr.x = 4.5;
            fr.y = 0;
            fr.z = 2;
            break;
        
        case 3: // Slice
            if (player == 1){
                b1.spin.x = 0;
                b1.spin.y = 600; // 600 rad/s
                b1.spin.z = 0;
            }
            else if (player == 2){
                b1.spin.x = 0;
                b1.spin.y = -600;
                b1.spin.z = 0;
            }
         
            fr.x = 4.5;
            fr.y = 0;
            fr.z = 4;
            break;
        
        case 4: // Smash de fou
            b1.spin.x = 6000;
            b1.spin.y = 6000;
            b1.spin.z = 6000;
            
            fr.x = 35;
            fr.y = 0;
            fr.z = 0.5;
            break;

        case 5: // filet
            b1.spin.x = 0;
            b1.spin.y = 0;
            b1.spin.z = 0;
            
            fr.x = 5;
            fr.y = 0;
            fr.z = 0.05;
            break;
            
        default:
            cout << "Coup non reconnu, coup basique" << endl;
            b1.spin.x = 0;
            b1.spin.y = 0;
            b1.spin.z = 0;
            
            fr.x = 7;
            fr.y = 0;
            fr.z = 1;
            break;
    }
    

    return fr;
}