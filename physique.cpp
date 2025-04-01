#include <iostream>
#include <string>
#include "classes.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <string>
#include "forces.hpp"
#include <iomanip>
#include <chrono>
#include <vector>


using namespace std;


// =======================================================
// ================== ACCELERATION =======================
// =======================================================

// Calcul de la nouvelle acceleration
coo new_a(coo v, coo spin, double ro, balle &b, table t, raquette r1, raquette r2, filet f, coo fr) {
    
    coo a;
    coo ft = force_frottement(v, ro);
    coo fm = force_magnus(v, spin, ro);

    if(collision_filet(b,f)){ // hors jeu filet
        cout << "Collision avec le filet" << endl;
        a = {0,0,0};
        b.v = {0,0,0};
        b.spin = {0,0,0};
        b.centre = {0,0,0};
    }

    else if(collision_sol(b)){ // hors jeu sol
        a = {0,0,0};
        b.v = {0,0,0};
        b.spin = {0,0,0};
        b.centre = {0,0,0};
    }


    else{
    a.x = (ft.x + fm.x) * INVERSE_SUR_MASSE;
    a.y = (ft.y + fm.y) * INVERSE_SUR_MASSE;
    a.z = (ft.z + fm.z - b.masse * GRAVITY) * INVERSE_SUR_MASSE; 

    }
    

    
    return a;
}

// ========================================================
// ================= EULER ================================
// ========================================================


// Calcul de la nouvelle vitesse
coo new_v(coo a, coo old_v, double dt, balle &b, table t, raquette r1, raquette r2, coo fr) { 

    coo new_v;
    new_v.x = old_v.x +  a.x*dt ;
    new_v.y = old_v.y +  a.y*dt ;
    new_v.z = old_v.z +  a.z*dt ;

    if(collision_raquette(b, r1) || collision_raquette(b, r2)){ 
        new_v.x = fr.x;
        new_v.y = fr.y;
        new_v.z = fr.z;
    }

    if (collision_table(b, t)) {
        b.centre.z = 0;
        coo fr = force_frottement_rebond(b.v, b.spin, b.masse, 0.3);
        appliquer_frottement_au_rebond(&b.v, &b.spin, fr, b.masse, 0.001);
        
        // Rebond vertical (coefficient de restitution e)
        new_v.z = -new_v.z;
    }
    

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
