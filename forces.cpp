#include <iostream>
#include <string>
#include "classes.hpp"
#include "forces.hpp"
#include <chrono>

using namespace std;

// ================== FORCES ========================

coo force_frottement(coo v, double ro) {

    coo ft;
    double Cx = 0.47 ;
    double pi = 3.14159265;
    double r = 0.02;
    double S = pi*r*r;

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

    coo fr;
    double r = 0.02; // Rayon de la balle de ping-pong

    // Calcul de la vitesse tangentielle au point de contact
    coo v_contact;
    v_contact.x = v.x - r * spin.y;
    v_contact.y = v.y + r * spin.x;


    // Appliquer le frottement si la balle glisse
    fr.x = -mu * masse * GRAVITY * (v_contact.x / (abs(v_contact.x) + 1e-6)); 
    fr.y = -mu * masse * GRAVITY * (v_contact.y / (abs(v_contact.y) + 1e-6));
    fr.z = 0;

    return fr;
}



// ================== COLLISIONS ========================


// on approxime la balle à juste son centre pour l'instant
bool collision_filet(balle b, filet f) {
    //test filet
    if (b.centre.x >= -0.01 && b.centre.x <= 0.01) { // 1cm de chaque côté du filet
        if (b.centre.z >= 0 && b.centre.z <= f.hauteur) {
            if (b.centre.y >= -(f.largeur / 2) && b.centre.y <= (f.largeur / 2)) {
                cout << "Collision avec le filet" << endl;
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
                cout << "Collision avec la table" << endl;
                return 1;
            }
        }
    }
    return 0;
}

bool collision_raquette(balle b, raquette r) {
    //test raquette
    if (b.centre.z >= (r.centre.z - 0.2 ) && b.centre.z <= (r.centre.z + 0.2)) {
        if (b.centre.y >= (r.centre.y - 0.2) && b.centre.y <= (r.centre.y + 0.2)) {
            if (b.centre.x >= (r.centre.x - 0.2) && b.centre.x <= (r.centre.x + 0.2)) {
                cout << "Collision avec la raquette" << endl;
                return 1;
            }
        }
    }
    return 0;
}

