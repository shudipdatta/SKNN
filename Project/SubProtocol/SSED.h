#ifndef SSED_H_
#define SSED_H_

#include <iostream>
#include "../Key/Public_Key.h"
#include "../Key/Private_Key.h"
#include "../Utility/Random_Generator.h"
#include "Paillier.h"
#include "SM.h"
#include <gmpxx.h>

using namespace std;

void ssed_step_1(mpz_t** e_uv, mpz_t* e_u, mpz_t* e_v, int size, mpz_t g, mpz_t N);

void ssed_step_2(mpz_t ** e_uv2 , mpz_t*  e_uv, int size, mpz_t g,  mpz_t N, mpz_t mu, mpz_t lambda);

void ssed_step_3(mpz_t ssed , mpz_t*  e_uv2, int size);

void SSED_tester();

#endif
