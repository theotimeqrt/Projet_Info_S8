#include <iostream>
#include <string>
#include "classes.hpp"
#include "forces.hpp"
#include <chrono>

using namespace std;

// ========================= Détection joueur qui joue ==============================

int need_coup(balle b) { // O personne 1 joueur gauche 2 joueur droite

    if (b.centre.x <= -1.37) { // joueur gauche doit jouer
        return 1;
    }
    else if (b.centre.x >= 1.37) {
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

// ========================= Calcul de la force d'un coup =======================

//Type de coup
// Fonction a appellé seulement lorsque la balle est proche de la raquette
coo coup (balle &b1, int player){

    coo fr;
    int type_coup;
    cout << "entrer un coup (1, 2, 3, 4): ";
    cin >> type_coup;
    cout << endl;

    switch (type_coup) {
       
        case 1: //Renvoi simple
            b1.spin.x = 0.5 * b1.spin.x;
            b1.spin.y = 0.5 * b1.spin.y;
            b1.spin.z = 0.5 * b1.spin.z;
            
            fr.x = -b1.a.x * b1.masse;
            fr.y = -b1.a.y * b1.masse;
            fr.z = -b1.a.z * b1.masse;
            break;
        
        case 2: // Lift
            if (player == 1){
                b1.spin.x = 0;
                b1.spin.y = -4000;
                b1.spin.z = 0;
            }
            else if (player == 2){
                b1.spin.x = 0;
                b1.spin.y = 4000;
                b1.spin.z = 0;
            }
            
            fr.x = -b1.a.x * b1.masse * 2;
            fr.y = -b1.a.y * b1.masse * 2;
            fr.z = -b1.a.z * b1.masse * 2;
            break;
        
        case 3: // Slice
            if (player == 1){
                b1.spin.x = 0;
                b1.spin.y = 6000;
                b1.spin.z = 0;
            }
            else if (player == 2){
                b1.spin.x = 0;
                b1.spin.y = -6000;
                b1.spin.z = 0;
            }
         
            fr.x = -b1.a.x * b1.masse * 2;
            fr.y = -b1.a.y * b1.masse * 2;
            fr.z = -b1.a.z * b1.masse * 2;
            break;
        
        case 4: // Smash de fou
            b1.spin.x = 6000;
            b1.spin.y = 6000;
            b1.spin.z = 6000;
            
            fr.x = -b1.a.x * b1.masse * 27.8;
            fr.y = -b1.a.y * b1.masse * 27.8;
            fr.z = -b1.a.z * b1.masse * 27.8;
            break;
        default:
            fr.x = 0.0;
            fr.y = 0.0;
            break;
    }
    

    return fr;
}