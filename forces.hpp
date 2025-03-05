#ifndef FONCTIONS_HPP
#define FONCTIONS_HPP

#include <iostream>
#include <string>
#include "classes.hpp"

using namespace std;


coo force_frottement(coo v, double ro);

coo new_v(coo a, coo old_v, double dt);

coo new_coo(coo old_pos, coo v, double dt);

coo force_magnus(coo v, coo spin, double ro);

coo force_frottement_rebond(coo v, coo spin, double masse, double mu);

coo force_normale(coo v, double masse, double e);

#endif // FONCTIONS_HPP