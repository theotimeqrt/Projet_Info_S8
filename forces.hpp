#ifndef FONCTIONS_HPP
#define FONCTIONS_HPP
#include <iostream>
#include "classes.hpp"
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

const double GRAVITY = 9.81; 
const double INVERSE_SUR_MASSE = 370.370; //  (1/2.7g) en kg

constexpr std::chrono::milliseconds pas_t(1); // ms

using namespace std;


coo force_frottement(coo v, double ro);

coo force_magnus(coo v, coo spin, double ro);

coo force_frottement_rebond(coo v, coo spin, double masse, double mu);

void appliquer_frottement_au_rebond(coo* v, coo* spin, coo fr, double masse, double dt_contact);

bool collision_raquette(balle b, raquette r);

bool collision_table(balle b, table t);

bool collision_filet(balle b, filet f);

bool collision_sol(balle b);

void test_force(bool gravite, bool frottement, bool magnus, balle &balle1, table table1, raquette &raquette1, raquette &raquette2, filet filet1 );


#endif // FONCTIONS_HPP