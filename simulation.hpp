#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "classes.hpp"

void initialiser_simulation();
int mise_a_jour_balle(double dt, int &compteur);
extern balle balle1;
extern table table1;
extern filet filet1;

#endif
