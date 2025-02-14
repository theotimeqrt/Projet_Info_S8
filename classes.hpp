#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <string>

using namespace std;

struct coo {
    float x;
    float y;
    float z;
};

struct vitesse {
    float vx;
    float vy;
    float vz;
};

struct acc {
    float ax;
    float ay;
    float az;
};

class jeu_ping_pong {
    public:
        int j_a;
        int j_b;
        int score_a;
        int score_b;
        int serveur;
};

class table {
    public:
        int largeur = 183; // taille officielle en cm
        int longueur = 274; // cm
        coo centre_table = {0,0,0};
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
        vitesse vitesse_raquette;
};

class balle {
    public:
        int rayon = 2; // cm
        int acceleration = 0;
        vitesse vitesse_balle;
        coo centre_balle;
};


#endif // CLASSES_H
