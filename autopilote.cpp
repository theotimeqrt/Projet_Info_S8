#include <iostream>
#include <string>
#include "classes.hpp"
#include "forces.hpp"
#include <chrono>
#include <cstdlib>
#include <ctime>


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

    if (b.centre.z >= 0.25) { 
        r1.centre = {-1.37, b.centre.y, b.centre.z};
        r2.centre = {1.37, b.centre.y, b.centre.z};
    }
}

void move_raquettes1(balle b, raquette &r) {

    if (b.centre.z >= 0.25) { 
        r.centre = {-1.37, b.centre.y, b.centre.z};
    }
}

void move_raquettes2(balle b, raquette &r) {

    if (b.centre.z >= 0.25) { 
        r.centre = {1.37, b.centre.y, b.centre.z};
    }
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

// ========================= Aléatoire en y ========================

float alea_y(balle b, int risque) { // risque entre 0 et 2, retourne la force à mettre en y
    
    static bool initialized = false;  // Assure que srand n'est appelé qu'une seule fois
    if (!initialized) {
        std::srand(std::time(0));  // Initialisation du générateur aléatoire
        initialized = true;
    }

    float force_y = 0;

    if(b.centre.y >= 0.4575){
        if(risque == 0){
            force_y = -0.1 - (std::rand() % 90) / 100.0;  // entre -0.1 et -1.0
        }
        else if(risque == 1){
            force_y = 0.2 - (std::rand() % 121) / 100.0;  // entre 0.2 et -1.0
        }
        else if(risque == 2){
            force_y = 1.0 - (std::rand() % 301) / 100.0;  // entre 1.0 et -2.0
        }
    }

    else if(b.centre.y >= 0 && b.centre.y <= 0.4575 ){
        if(risque == 0){
            force_y = -0.5 - (std::rand() % 51) / 100.0;  // entre -0.5 et -1.0

        }
        else if(risque == 1){
            force_y = 1.0 - (std::rand() % 251) / 100.0;  // entre 1.0 et -1.5
        }
        else if(risque == 2){
            force_y = 1.5 - (std::rand() % 401) / 100.0;  // entre 1.5 et -2.5

        }
    }

    else if(b.centre.y <= 0 && b.centre.y >= -0.4575 ){
        if(risque == 0){
            force_y = 0.5 + (std::rand() % 51) / 100.0;  // entre 0.5 et 1.0
        }
        else if(risque == 1){
            force_y = -1.0 + (std::rand() % 251) / 100.0;  // entre -1.0 et 1.5

        }
        else if(risque == 2){
            force_y = -1.5 + (std::rand() % 401) / 100.0;  // entre -1.5 et 2.5
        }
    }

    else if(b.centre.y <= -0.4575){
        if(risque == 0){
            force_y = 0.1 + (std::rand() % 91) / 100.0;  // entre 0.1 et 1.0
        }
        else if(risque == 1){
            force_y = -2.0 + (std::rand() % 121) / 100.0;  // entre -2.0 et 1.0

        }
        else if(risque == 2){
            force_y = -1.0 + (std::rand() % 301) / 100.0;  // entre -1.0 et 2.0
        }
    }
    else{
        force_y = 0;
    };

    return force_y;
}


// ========================= Calcul de la force d'un coup =======================


// Fonction a appellé seulement lorsque la balle est proche de la raquette
coo coup (balle &b1, int player){

    coo fr;
    int type_coup;

    // === risque ===
    int risque = 0; // 0 : pas de risque, 1 : faible risque, 2 : moyen risque (autre : pas de force en y)
    // ==============

    cout << "====== Coups ======" << endl;
    cout << "1 : Renvoi tout droit" << endl;
    cout << "2 : Lift" << endl;
    cout << "3 : Slice" << endl;
    cout << "4 : Smash" << endl;
    cout << "5 : Ras du filet" << endl;
    cout << "6 : Lobe" << endl;
    cout << "7 : Service court" << endl; 
    cout << "8 : Service long" << endl;   
    cout << "===================" << endl;

    cout << "Entrer un coup à faire pour J" << player << " : ";
    cin >> type_coup;
    cout << endl;



    switch (type_coup) {
       
        case 1: //Renvoi tout droit
            b1.spin.x = 0;
            b1.spin.y = 0;
            b1.spin.z = 0;
            
            fr.x = 4;
            fr.y = alea_y(b1, risque);
            fr.z = 2;
            break;
        
        case 2: // Lift
            if (player == 1){
                b1.spin.x = 0;
                b1.spin.y = 800; // 800 rad/s
                b1.spin.z = 0;   //      coo fr = force_fr
                // }
            
            }
            else if (player == 2){
                b1.spin.x = 0;
                b1.spin.y = -800;
                b1.spin.z = 0;
            }
            
            fr.x = 4;
            fr.y = alea_y(b1, risque);
            fr.z = 2;
            break;
        
        case 3: // Slice
            if (player == 1){
                b1.spin.x = 0;
                b1.spin.y = -600; // 600 rad/s
                b1.spin.z = 0;
            }
            else if (player == 2){
                b1.spin.x = 0;
                b1.spin.y = 600;
                b1.spin.z = 0;
            }
         
            fr.x = 3;
            fr.y = alea_y(b1, risque);
            fr.z = 2.5;
            break;
        
        case 4: // Smash
            if (player == 1){
                b1.spin.x = 0;
                b1.spin.y = -800; // 800 rad/s
                b1.spin.z = 0;   //      coo fr = force_fr
            }
            else if (player == 2){
                b1.spin.x = 0;
                b1.spin.y = 800;
                b1.spin.z = 0;
            }
            
            fr.x = 9;
            fr.y = fr.y = alea_y(b1, risque);
            fr.z = -3;
            break;

        case 5: // filet
            b1.spin.x = 0;
            b1.spin.y = 0;
            b1.spin.z = 0;
            
            fr.x = 5;
            fr.y = fr.y = alea_y(b1, risque);
            fr.z = 0.05;
            break;

        case 6: // Lobe

            b1.spin.x = 0;
            b1.spin.y = 0; 
            b1.spin.z = 0; 

            fr.x = 3;
            fr.y = fr.y = alea_y(b1, risque);
            fr.z = 3;
            break;
        
        case 7: // Service court
            b1.spin.x = 0;
            b1.spin.y = 0;
            b1.spin.z = 0;
            
            fr.x = 3;
            fr.y = 0;
            fr.z = -2;
            break;

        case 8: // Service long
            b1.spin.x = 0;
            b1.spin.y = 0;
            b1.spin.z = 0;
            
            fr.x = 3;
            fr.y = 0;
            fr.z = -2.5;
            break;
            
        default:
            cout << "Coup non reconnu, coup basique" << endl;
            b1.spin.x = 0;
            b1.spin.y = 0;
            b1.spin.z = 0;
            
            fr.x = 7;
            fr.y = fr.y = alea_y(b1, risque);
            fr.z = 2;
            break;
    }
    

    return fr;
}