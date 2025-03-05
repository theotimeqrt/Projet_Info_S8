#include <iostream>
#include <string>
#include "classes.hpp"
#include "forces.hpp"
#include <chrono>

using namespace std;

const double GRAVITY = 9.81; 


coo force_frottement(coo v, double ro) {

    coo ft;
    double Cx = 0.5 ;
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

    coo N = {0, 0, 0};

    // Si la balle touche la table (z ≈ 0 ou légèrement négatif)
    if (pos.z < 0) { 
        N.z = masse * GRAVITY + ft.z + fm.z  ;  // Annule l'effet de la gravité
        // Gérer le rebond avec un coefficient de restitution
        //pos.z = -e * pos.z;  
    }

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

    return fr;
}


