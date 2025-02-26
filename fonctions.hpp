#ifndef FONCTIONS_HPP
#define FONCTIONS_HPP

#include <iostream>
#include <string>
#include "classes.hpp"

using namespace std;


coo force_frottement(coo v, float ro);

coo new_v(coo a, coo old_v, float dt);

coo new_coo(coo old_pos, coo v, float dt);

coo force_magnus(coo v, coo spin, float ro);

coo force_frottement_rebond(coo v, coo spin, float masse, float mu);

coo force_normale(coo v, float masse, float e);

#endif // FONCTIONS_HPP