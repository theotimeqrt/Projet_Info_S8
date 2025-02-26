#ifndef CLASSES_HPP
#define CLASSES_HPP

#include <iostream>
#include <string>

using namespace std;

struct coo {
    float x,y,z;
};


class jeu_ping_pong {
    public:
        int joueur_a;
        int joueur_b;
        int score_a;
        int score_b;
        int serveur; // 0 pour joueur a, 1 pour joueur b
};

class table {
    public:
        int largeur = 1.83; // taille officielle en m
        int longueur = 2.74; // en m 
        coo centre_table = {0,0,0};
};

class meteo {
    public:
        int norme_vent ;
        int sens_vent ;
        // int direction_vent ; // Si on veut avoir plus que des vents de face oiu dos
};

class filet {
    public:
        float hauteur = 0.1525; // 15.25cm
        int largeur = 1.83; // 183cm
        coo centre_filet = {0,0,7.625}; // centre de la table etant origine du repere
};

class raquette {
    public:
        float largeur = 0.15; // 15cm
        float hauteur = 0.25; // 25cm
        coo centre_raquette;
        coo vitesse_raquette;
};

class balle {
    public:
        float rayon = 0.02; // 2cm
        float masse = 0.0027; // 2.7g
        coo a;
        coo v;
        coo centre;
        coo spin; // rad.s-1
};

#endif // CLASSES_HPP