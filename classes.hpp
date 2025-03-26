#ifndef CLASSES_HPP
#define CLASSES_HPP

#include <iostream>
#include <string>

using namespace std;

struct coo {
    double x,y,z;
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
        double largeur = 1.83; // taille officielle en m
        double longueur = 2.74; // en m 
        double epaisseur = 0.03; // 3cm
        coo centre_table = {0,0,0};
};

class meteo {
    public:
        double norme_vent ;
        double sens_vent ;
        int direction_vent ; // Si on veut avoir plus que des vents de face oiu dos
};

class filet {
    public:
        double hauteur = 0.1525; // 15.25cm
        double largeur = 1.83; // 183cm
        double epaisseur = 0.009 ; // 9mm
        coo centre_filet = {0,0,7.625}; // centre de la table etant origine du repere
};

class raquette {
    public:
        double largeur = 0.15; // 15cm
        double hauteur = 0.25; // 25cm
        double epaisseur = 0.02; // 2cm
        coo centre;
        coo v;
};

class balle {
    public:
        double rayon = 0.02; // 2cm
        double masse = 0.0027; // 2.7g
        coo a;
        coo v;
        coo centre;
        coo spin; // rad.s-1
};

#endif // CLASSES_HPP 