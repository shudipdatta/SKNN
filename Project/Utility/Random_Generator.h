#ifndef RANDOM_GENERATOR_H_
#define RANDOM_GENERATOR_H_

#include <iostream>
#include <time.h>
#include <gmpxx.h>

using namespace std;

void generate_randomb(mpz_t rand, const mp_bitcnt_t bit_len);

void generate_randomm(mpz_t rand, mpz_t N);

#endif // RANDOM_GENERATOR_H_
