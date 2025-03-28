#ifndef PHYSIQUE_HPP
#define PHYSIQUE_HPP

#include <iostream>
#include <string>
#include "classes.hpp"

using namespace std;



coo new_a(coo v, coo spin, double ro, balle &b, table t, raquette r1, raquette r2, filet f, coo fr);

coo new_v(coo a, coo old_v, double dt, balle &b, table t, raquette r1, raquette r2, coo fr);

coo new_coo(coo old_pos, coo v, double dt);



#endif