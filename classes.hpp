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
        int largeur = 183; // taille officielle en cm
        int longueur = 274; // cm
        coo centre_table = {0,0,0};

        //idee de chatgpt : mettre un constructeur par defaut
        //table() : largeur(183), longueur(274), ) {}
        void sayHello();
};

class meteo {
    public:
        int norme_vent ;
        int sens_vent ;
        // int direction_vent ; // Si on veut avoir plus que des vents de face oiu dos
};

class filet {
    public:
        float hauteur = 15.25; // cm
        int largeur = 183; // cm
        coo centre_filet = {0,0,7.625}; // centre de la table etant origine du repere
};

class raquette {
    public:
        int largeur = 15; // cm
        int hauteur = 25; // cm
        coo centre_raquette;
        coo vitesse_raquette;
};

class balle {
    public:
        int rayon = 2; // cm
        float masse = 2.7; // g
        coo acceleration_balle;
        coo vitesse_balle;
        coo centre_balle;
        coo spin;
};

#endif // CLASSES_HPP