#ifndef FONCTIONS_HPP
#define FONCTIONS_HPP

#include <iostream>
#include <string>
#include "classes.hpp"

using namespace std;


coo force_frottement(coo v, float ro);

coo new_v(coo a, coo old_v, float dt);

coo new_coo(coo old_pos, coo v, float dt);

coo force_magnus(coo v, coo spin);

#endif // FONCTIONS_HPP