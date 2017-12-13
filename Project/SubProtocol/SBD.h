#ifndef SBD_H_
#define SBD_H_

#include <iostream>
#include "../Key/Public_Key.h"
#include "../Key/Private_Key.h"
#include "../Utility/Random_Generator.h"
#include "Paillier.h"
#include <gmpxx.h>

using namespace std;

void Binary(mpz_t **e_xi, mpz_t x, size_t m);

void Encrypted_LSB(mpz_t e_xi, mpz_t T, mpz_t g, mpz_t N);

void SVR(mpz_t lamb, mpz_t e_x, mpz_t *e_xi,  size_t m, mpz_t g, mpz_t N, mpz_t mu, mpz_t lambda);

void SBD(mpz_t** e_xi, mpz_t e_x, size_t m, mpz_t g, mpz_t N, mpz_t mu, mpz_t lambda);

void SBD_tester();

#endif
