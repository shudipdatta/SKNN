#ifndef PAILLIER_H_
#define PAILLIER_H_

#include <iostream>
#include "../Utility/Random_Generator.h"
#include <gmpxx.h>

using namespace std;

void encrypt(mpz_t cipher, mpz_t msg, mpz_t g, mpz_t N);

void decrypt(mpz_t msg, mpz_t cipher, mpz_t N, mpz_t mu, mpz_t lambda);

#endif


