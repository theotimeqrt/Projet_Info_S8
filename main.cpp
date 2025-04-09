#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include <vector>

#include "classes.hpp"
#include "forces.hpp"
#include "physique.hpp"
#include "autopilote.hpp"

using namespace std;


// ==================================================================
// ============================= MAIN ===============================
// ==================================================================

int main(int argc, char **argv) {

    // Initialisation de la balle
    balle balle1;
    balle1.centre = {0, 0, 1}; // 1 mètre au-dessus du sol
    balle1.v = {-2, 0, 0};
    balle1.a = {0, 0, 0};
    balle1.spin = {0, 0, 0}; 
    balle1.masse = 0.0027;


    table table1;
    filet filet1;

    raquette raquette1;
    raquette1.centre = {-1.37, 0, 20}; // bord de la table à droite

    raquette raquette2;
    raquette2.centre = {1.37, 0, 30}; // bord de la table à gauche


    test_force(true, true, false, balle1, table1, raquette1, raquette2, filet1);

    return 0;
}

