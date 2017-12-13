#ifndef SM_H_
#define SM_H_

#include <iostream>
#include "../Key/Public_Key.h"
#include "../Key/Private_Key.h"
#include "../Utility/Random_Generator.h"
#include "Paillier.h"
#include <gmpxx.h>

using namespace std;

void step_1(mpz_t a_, mpz_t b_, mpz_t e_a, mpz_t e_b, mpz_t r_a, mpz_t r_b, mpz_t g, mpz_t N);

void step_2(mpz_t h_, mpz_t a_, mpz_t b_, mpz_t g, mpz_t mu, mpz_t lambda, mpz_t N);

void step_3(mpz_t e_ab, mpz_t h_,  mpz_t e_a, mpz_t e_b, mpz_t r_a, mpz_t r_b, mpz_t g, mpz_t N );

void secure_multiplication(mpz_t e_ab, mpz_t e_a, mpz_t e_b, mpz_t g, mpz_t N, mpz_t mu, mpz_t lambda);

void SM_tester();

#endif
