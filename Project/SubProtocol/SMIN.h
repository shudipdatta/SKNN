#ifndef SMIN_H_
#define SMIN_H_

#include <iostream>
#include "../Key/Public_Key.h"
#include "../Key/Private_Key.h"
#include "../Utility/Random_Generator.h"
#include "Paillier.h"
#include "SM.h"
#include "SBD.h"
#include <gmpxx.h>

using namespace std;


void smin_step_1(mpz_t** L , mpz_t**  Gam, mpz_t** r, mpz_t f, size_t m, mpz_t* e_ui, mpz_t* e_vi, mpz_t g, mpz_t N, mpz_t mu, mpz_t lambda);

void smin_step_2(mpz_t** M , mpz_t e_alp, size_t m, mpz_t* Gam, mpz_t* L, mpz_t g,  mpz_t N, mpz_t mu, mpz_t lambda);

void smin_step_3(mpz_t** e_mini , mpz_t*  M, mpz_t* r, mpz_t e_alp, size_t m, mpz_t f, mpz_t* e_ui, mpz_t* e_vi, mpz_t N);

void SMIN(mpz_t **e_mini, size_t m, mpz_t *e_ui, mpz_t *e_vi, mpz_t g, mpz_t N, mpz_t mu, mpz_t lambda);

void SMIN_tester();

#endif
