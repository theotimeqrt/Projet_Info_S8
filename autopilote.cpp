#include <iostream>
#include <string>
#include "classes.hpp"
#include "forces.hpp"
#include <chrono>

using namespace std;


int need_coup(balle b) { // O personne 1 joueur droite 2 joueur gauche
    if (b.centre.x >= 1.37) { // joueur droite doit jouer
        return 1;
    }
    else if (b.centre.x <= -1.37) {
        return 2;
    }
    else {
        return 0;
    }
    
}

void placer_raquette(balle b, raquette &r1, raquette &r2) {
    if (need_coup(b) == 1) { // joueur droite doit jouer
        r1.centre = {1.37, 0, 1};
    }
    else if (need_coup(b) == 2) {
        r2.centre = {-1.37, 0, 1};
    }
    else {
        r1.centre = {1.37, 0, 1};
        r2.centre = {-1.37, 0, 1};
    }
}




coo coup (balle b1){
    coo fr;
    int type_coup;
    cout << "entrer un coup (1, 2, 3, 4): ";
    cin >> type_coup;
    cout << endl;

    switch (type_coup) {
        case 1:
            b1.spin = {0, 0, 0};
            break;
        case 2:
            
            break;
        case 3:
            
            break;
        case 4:
            
            break;
        default:
            fr.x = 0.0;
            fr.y = 0.0;
            break;
    }
    

    return fr;
}