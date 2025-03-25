#ifndef FONCTIONS_HPP
#define FONCTIONS_HPP
#include <iostream>
#include "classes.hpp"
#include <chrono>
#include <vector>
#include <string>
#include "classes.hpp"
#include <fstream>
#include <filesystem>

const double GRAVITY = 9.81; 
const double INVERSE_SUR_MASSE = 370.370; //  (1/2.7g) en kg

constexpr std::chrono::milliseconds pas_t(1); // ms

using namespace std;


coo force_frottement(coo v, double ro);

coo new_v(coo a, coo old_v, double dt);

coo new_coo(coo old_pos, coo v, double dt);

coo force_magnus(coo v, coo spin, double ro);

coo force_frottement_rebond(coo v, coo spin, double masse, double mu);

coo force_normale(coo v, double masse, double e);

bool collision_raquette(balle b, raquette r);

bool collision_table(balle b, table t);

bool collision_filet(balle b, filet f);

bool collision_sol(balle b);

void test_force(int pas, bool gravite, bool frottement, bool magnus, balle &balle1, table table1, raquette raquette1, filet filet1 );


#endif // FONCTIONS_HPP