#ifndef AUTOPILOTE_HPP
#define AUTOPILOTE_HPP


#include <iostream>
#include "classes.hpp"
#include <chrono>
#include <vector>
#include <string>
#include "classes.hpp"


int need_coup(balle b);
void move_raquettes(balle b, raquette &r1, raquette &r2);
coo coup (balle &b1, int player);
bool fin_jeu(balle b, filet f);
float alea_y(balle b, int risque);
void move_raquettes1(balle b, raquette &r);
void move_raquettes2(balle b, raquette &r);

#endif