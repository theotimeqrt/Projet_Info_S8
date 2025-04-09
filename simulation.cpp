// simulation.cpp corrigé pour suivre la logique de test_force sans écrire dans les fichiers
#include <SDL2/SDL_mixer.h>
#include "simulation.hpp"
#include "forces.hpp"
#include "physique.hpp"
#include "autopilote.hpp"
#include <iostream>

using namespace std;

extern Mix_Chunk* sonRebond;
extern Mix_Chunk* sonRaquette;

balle balle1;
table table1;
filet filet1;
raquette raquette1;
raquette raquette2;
coo fr = {0, 0, 0};
bool simulation_terminee = false;

void initialiser_simulation() {
    balle1.centre = {-1.27, 0, 0.75};
    balle1.v = {-1, 0, 0};
    balle1.a = {0, 0, 0};
    balle1.spin = {0, 0, 0};
    balle1.masse = 0.0027;
    balle1.rayon = 0.02;

    raquette1.centre = {-1.37, 0, 0};
    raquette2.centre = {1.37, 0, 0};

    simulation_terminee = false;
}

int mise_a_jour_balle(double dt, int &compteur) {
    
    if (simulation_terminee) return 0;

    static int t = 0;
    static int player = 0;
    static int dernier_player = 0;
    static int a_touche_table = -1;
    static int dem_coup = 1;

    t++;

    player = need_coup(balle1);
    if (player != 0) dernier_player = player;

    // fin par sol

    if (collision_sol(balle1)) {
        cout << "position balle à la fin: " << balle1.centre.x << " " << balle1.centre.y << " " << balle1.centre.z << endl;
        cout << "vitesse de la balle à la fin: " << balle1.v.x << " " << balle1.v.y << " " << balle1.v.z << endl;
        cout << "\nBalle au sol !" << endl;
        if (dernier_player == 1) cout << "Fin de jeu, bien joué au joueur 1 !" << endl;
        else cout << "Fin de jeu, bien joué au joueur 2 !" << endl;
        simulation_terminee = true;
        return 0;
    }

    // fin par filet

    if (collision_filet(balle1, filet1)) {
        cout << "\nBalle dans le filet !" << endl;
        if (dernier_player == 1) {
            cout << "Le joueur 2 a gagné !" << endl;
        } else {
            cout << "Le joueur 1 a gagné !" << endl;
        }
        simulation_terminee = true;
        return 0;
    }

    // comptage des rebonds sur la table

    if (collision_table(balle1, table1)) {
        Mix_PlayChannel(-1, sonRebond, 0);
        if(balle1.centre.x > 0) a_touche_table = a_touche_table + 1;
        else if( balle1.centre.x < 0) a_touche_table = a_touche_table -1;
    }

    // Bouger les raquettes pour qu'elles suivent la balle une fois le rebond passé

    if (a_touche_table == -1) {
        move_raquettes1(balle1, raquette1);
    }
    else if (((a_touche_table == 1) && (compteur > 1)) || ((a_touche_table == 0 ) && (compteur == 1))) {
        move_raquettes2(balle1, raquette2);
    }
    



    if (player == 1 && dem_coup == 1 && a_touche_table == -1) {
        
        cout << "Coup player 1 " << endl;
        cout << "a_touche table = " << a_touche_table << endl;
        cout << "compteur = " << compteur << endl;
        compteur += 1;

        fr = coup (balle1, player); // fonction pour renvoyer
        Mix_PlayChannel(-1, sonRaquette, 0);
        dem_coup = 0;
        a_touche_table = 0; 
        cout << "a_touche table = " << a_touche_table << endl;
        cout << "compteur = " << compteur << endl;

        dernier_player = 1;

    } 

    else if ((player == 2 && dem_coup == 1 )&& (((a_touche_table == 1) && (compteur > 1)) || ((a_touche_table == 0 ) && (compteur == 1))) ) {
        
        cout << "Coup player 2 " << endl;
        cout << "a_touche table = " << a_touche_table << endl;
        cout << "compteur = " << compteur << endl;
        compteur += 1;
        
        fr = coup (balle1, player); // fonction pour renvoyer
        Mix_PlayChannel(-1, sonRaquette, 0);
        fr.x = -fr.x;
        dem_coup = 0;
        a_touche_table = 0; 

        dernier_player = 2;


    } 

    else if (player == 0) {
        dem_coup = 1;
        fr = balle1.v;
    }
    
    coo new_acc = new_a(balle1.v, balle1.spin, 1.2, balle1, table1, raquette1, raquette2, filet1, fr);
    balle1.a = new_acc;
    balle1.v = new_v(balle1.a, balle1.v, dt, balle1, table1, raquette1, raquette2, fr);
    balle1.centre = new_coo(balle1.centre, balle1.v, dt);

    return 1;


} 
