#include <iostream>
#include <string>
#include "classes.hpp"
#include "fonctions.hpp"
#include <chrono>

using namespace std;


coo force_frottement(coo v, float ro) {

    coo ft;
    float Cx = 0.5 ;
    float pi = 3.14159265;
    float r = 0.02;
    float S = pi*r*r;

    ft.x = -0.5*Cx*ro*S*v.x*abs(v.x);
    ft.y = -0.5*Cx*ro*S*v.y*abs(v.y);
    ft.z = -0.5*Cx*ro*S*v.z*abs(v.z);
    return ft ;

}

// Calcul de la force Magnus
coo force_magnus(coo v, coo spin) {

    coo fm;
    float C_m = 0.0004; // coef magnus
    // Calcul du produit vectoriel : spin x v
    fm.x = C_m * (spin.y * v.z - spin.z * v.y);
    fm.y = C_m * (spin.z * v.x - spin.x * v.z);
    fm.z = C_m * (spin.x * v.y - spin.y * v.x);
    return fm;
}
